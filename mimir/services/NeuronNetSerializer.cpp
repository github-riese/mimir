#include "NeuronNetSerializer.h"

#include "ActivationsManager.h"
#include "helpers/helpers.h"

#include <istream>
#include <ostream>

namespace mimir {
namespace services {

const static unsigned short littleEndian = 0xf00f;
const static unsigned short bigEndian = 0x0ff0;

union u16Write {
    uint16_t v;
    char writeable[sizeof (uint16_t)];
};

union u32Write {
    uint32_t v;
    char writeable[sizeof (uint32_t)];
};

union u64Write {
    uint64_t v;
    char writeable[sizeof (uint64_t)];
};

union sizeTWrite {
    size_t v;
    char writeable[sizeof (size_t)];
};

NeuronNetSerializer::NeuronNetSerializer()
{
    if (!isBigEndian()) {
        _changeEndianness = true;
    }
}

int NeuronNetSerializer::serialize(const mimir::services::NeuronNet &net, std::ostream &out)
{
    writeFileHeader(out);
    writeNetHeader(out, net);
    for (auto layer : net.layers()) {
        writeLayer(out, layer);
    }
    return 0;
}

int NeuronNetSerializer::deserialize(NeuronNet &net, std::istream &in)
{
    FileHeader header = readHeader(in);
    if (std::string(header.magic) != std::string(NET_FILE_MAGIC)) {
        return -1;
    }
    if (header.endianness == bigEndian) {
        _changeEndianness = true;
    }
    if (header.version != _version) {
        return -1;
    }
    NetHeader netHeader = readNetHeader(in);
    if (std::string(netHeader.magic) != std::string(NET_MAGIC)) {
        return -1;
    }
    std::string netName = readName(in, netHeader.nameLength);
    net.setName(netName);
    if (!rebuildLayers(in, netHeader, net)) {
        return -1;
    }
    return 0;
}

bool NeuronNetSerializer::canRead(std::istream &in) const
{
    FileHeader header;
    auto current = in.tellg();
    header = readHeader(in);
    in.seekg(current);
    if (header.magic != NET_FILE_MAGIC) {
        return false;
    }
    if (header.version != _version) {
        return false;
    }
    return true;
}

NeuronNetSerializer::FileHeader NeuronNetSerializer::readHeader(std::istream &in) const
{
    FileHeader header;
    in.read(header.magic, sizeof (header.magic));
    u16Write rawEndianness;
    in.read(rawEndianness.writeable, sizeof (rawEndianness));
    header.endianness = rawEndianness.v;
    header.version = readUInt32(in);
    return header;
}

NeuronNetSerializer::NetHeader NeuronNetSerializer::readNetHeader(std::istream &in) const
{
    NetHeader header;
    in.read(header.magic, sizeof (header.magic));
    header.layers = readUInt32(in);
    header.inputs = readUInt32(in);
    header.outputs = readUInt32(in);
    header.nameLength = readUInt32(in);
    return header;
}

NeuronNetSerializer::LayerHeader NeuronNetSerializer::readLayerHeader(std::istream &in) const
{
    LayerHeader header;
    in.read(header.magic, sizeof (header.magic));
    header.nodes = readSizeT(in);
    header.nextLayerNodes = readSizeT(in);
    header.activationNameLength = readUInt32(in);
    return header;
}

std::string NeuronNetSerializer::readName(std::istream &in, uint32_t length) const
{
    uint32_t paddedLength = length + length%4;
    auto buffer = new char[paddedLength];
    in.read(buffer, paddedLength);
    std::string name = buffer;
    delete [] buffer;
    return name;
}

std::vector<double> NeuronNetSerializer::readBiases(std::istream &in, size_t nodes) const
{
    std::vector<double> result(nodes);
    helpers::UnpackDouble unpack;
    std::transform(result.begin(), result.end(), result.begin(), [&unpack, this, &in](auto) {
        auto packed = readUInt64(in);
        return unpack(packed);
    });
    return result;
}

models::Matrix NeuronNetSerializer::readWeights(std::istream &in, size_t rows, size_t columns) const
{
    models::Matrix result;
    std::vector<uint64_t> rawRow(columns);
    std::valarray<double> row(columns);
    helpers::UnpackDouble unpack;
    while (rows-- > 0) {
        std::transform(rawRow.begin(), rawRow.end(), rawRow.begin(), [this, &in](auto) {
            return readUInt64(in);
        });
        std::transform(rawRow.begin(), rawRow.end(), std::begin(row), unpack);
        result.addRow(row);
    }
    return result;
}

bool NeuronNetSerializer::rebuildLayers(std::istream &in, const NeuronNetSerializer::NetHeader &header, NeuronNet &net)
{
    struct LayerDescription {
        LayerHeader header;
        std::string activationName;
        std::vector<double> biases;
        models::Matrix weights;
    };
    std::vector<LayerDescription> layerDesc;
    for (auto n = 0u; n < header.layers; ++n) {
        LayerDescription desc;
        desc.header = readLayerHeader(in);
        if (std::string(desc.header.magic) != std::string(LAYER_MAGIC)) {
            return false;
        }
        desc.activationName = readName(in, desc.header.activationNameLength);
        if (n > 0) {
            desc.biases = readBiases(in, desc.header.nodes);
        }
        if (n < header.layers - 1) {
            desc.weights = readWeights(in, desc.header.nodes, desc.header.nextLayerNodes);
        }
        layerDesc.push_back(desc);
    }
    for (auto layer : layerDesc) {
        net.appendLayer(layer.header.nodes, layer.activationName);
    }
    net.layer(0).setIsInput(true);
    net.connect();
    size_t layerNumber = 0;
    for (auto layer : layerDesc) {
        if (layerNumber > 0)
            net.setBiases(layerNumber, layer.biases);
        if (!net.layer(layerNumber).isOutputLayer())
            net.setWeigths(layerNumber, layer.weights);
         ++layerNumber;
    }
    return true;
}

void NeuronNetSerializer::writeFileHeader(std::ostream &out)
{
    FileHeader header {NET_FILE_MAGIC, _changeEndianness ? bigEndian : littleEndian, _version};
    out.write(header.magic, sizeof (header.magic));
    writeUShort(out, littleEndian);
    writeUInt32(out, _version);
}

void NeuronNetSerializer::writeNetHeader(std::ostream &out, const NeuronNet &net)
{
    NetHeader header {
        NET_MAGIC,
        static_cast<uint32_t>(net.numberOfLayers()),
        static_cast<uint32_t>(net.sizeOfLayer(0)),
        static_cast<uint32_t>(net.sizeOfLayer(-1u)),
        static_cast<uint32_t>(net.name().size())
    };
    out.write(header.magic, sizeof (header.magic));
    writeUInt32(out, header.layers);
    writeUInt32(out, header.inputs);
    writeUInt32(out, header.outputs);
    writeUInt32(out, header.nameLength);
    out << net.name();
    char padding[4] = {};
    int paddingLength = net.name().size()%4;
    out.write(padding, paddingLength);
}

void NeuronNetSerializer::writeLayer(std::ostream &out, const models::AbstractLayer &layer)
{
    auto activation = layer.activation();
    std::string activationName = activation != nullptr ? activation->name() : "";
    LayerHeader header{
        LAYER_MAGIC,
        layer.size(),
        layer.nextSize(),
        static_cast<uint32_t>(activationName.size() + (4-activationName.size()%4))
    };
    out.write(header.magic, sizeof (header.magic));
    writeSizeT(out, header.nodes);
    writeSizeT(out, header.nextLayerNodes);
    writeUInt32(out, header.activationNameLength);
    out << activationName;
    char padding[4] = {};
    long paddingLength = static_cast<long>(header.activationNameLength - activationName.size());
    out.write(padding, paddingLength);
    helpers::PackDouble pack;
    if (!layer.isInputLayer()) {
        std::vector<uint64_t> transportable(layer.size());
        auto biases = layer.biases();
        std::transform(biases.begin(), biases.end(), transportable.begin(), pack);
        std::for_each(transportable.begin(), transportable.end(), [this, &out](uint64_t bias) { writeUInt64(out, bias); });
    }
    if (!layer.isOutputLayer()) {
        auto weights = layer.weights();
        auto matrixData = weights.data();
        for (auto row : matrixData) {
            std::vector<uint64_t> transportable(row.size());
            std::transform(std::begin(row), std::end(row), transportable.begin(), pack);
            std::for_each(transportable.begin(), transportable.end(), [this, &out](uint64_t bias) { writeUInt64(out, bias); });
        }
    }
}

void NeuronNetSerializer::writeUInt64(std::ostream &out, uint64_t value)
{
    if (_changeEndianness) {
        value = htonll(value);
    }
    u64Write write;
    write.v = value;
    out.write(write.writeable, sizeof (uint64_t));
}

void NeuronNetSerializer::writeUInt32(std::ostream &out, uint32_t value)
{
    if (_changeEndianness) {
        value = htonl(value);
    }
    u32Write write;
    write.v = value;
    out.write(write.writeable, sizeof (uint32_t));
}

void NeuronNetSerializer::writeUShort(std::ostream &out, uint16_t value)
{
    if (_changeEndianness) {
        value = htons(value);
    }
    u16Write write;
    write.v = value;
    out.write(write.writeable, sizeof (uint16_t));
}

void NeuronNetSerializer::writeSizeT(std::ostream &out, size_t value)
{
    if (_changeEndianness) {
        value = htonll(value);
    }
    sizeTWrite write;
    write.v = value;
    out.write(write.writeable, sizeof (write));
}

uint64_t NeuronNetSerializer::readUInt64(std::istream &in) const
{
    u64Write read;
    in.read(read.writeable, sizeof (read));
    if (_changeEndianness) {
        read.v = ntohll(read.v);
    }
    return read.v;
}

uint32_t NeuronNetSerializer::readUInt32(std::istream &in) const
{
    u32Write read;
    in.read(read.writeable, sizeof (read));
    if (_changeEndianness) {
        read.v = ntohl(read.v);
    }
    return read.v;
}

uint16_t NeuronNetSerializer::readUShort(std::istream &in) const
{
    u16Write read;
    in.read(read.writeable, sizeof (read));
    if (_changeEndianness) {
        read.v = ntohs(read.v);
    }
    return read.v;
}

size_t NeuronNetSerializer::readSizeT(std::istream &in) const
{
    sizeTWrite read;
    in.read(read.writeable, sizeof (read));
    if (_changeEndianness) {
        read.v = ntohll(read.v);
    }
    return read.v;
}

bool NeuronNetSerializer::isBigEndian() const
{
    union checker {
        unsigned char asUChar[2];
        unsigned short asUshort;
    };
    checker test;
    test.asUshort = littleEndian;
    return test.asUChar[0] == 0xf0;
}

} // namespace services
} // namespace mimir
