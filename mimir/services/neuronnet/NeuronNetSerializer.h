#ifndef NEURONNETSERIALIZER_H
#define NEURONNETSERIALIZER_H

#include <ios>

#include "NeuronNet.h"

namespace mimir {
namespace services {
namespace neuronnet {

#define NET_FILE_MAGIC "mimir::data"
#define NET_MAGIC "mimir::neuronnet"
#define LAYER_MAGIC "mimir::layer"

class NeuronNetSerializer
{
    struct FileHeader {
        char magic[((sizeof (NET_FILE_MAGIC)/4)+1)*4];
        uint16_t endianness;
        uint32_t version;
    };

    struct NetHeader {
        char magic[((sizeof (NET_MAGIC)/4)+1)*4];
        uint32_t layers;
        uint32_t inputs;
        uint32_t outputs;
        uint32_t nameLength;
    };

    struct LayerHeader {
        char magic[((sizeof (LAYER_MAGIC)/4)+1)*4];
        size_t nodes;
        size_t nextLayerNodes;
        uint32_t activationNameLength;
    };

public:
    NeuronNetSerializer();
    int serialize(NeuronNet const &, std::ostream &);
    int deserialize(NeuronNet &, std::istream &);
    bool canRead(std::istream &) const;
private:
    FileHeader readHeader(std::istream &) const;
    NetHeader readNetHeader(std::istream &) const;
    LayerHeader readLayerHeader(std::istream &) const;
    std::string readName(std::istream &, uint32_t length) const;
    std::vector<double> readBiases(std::istream &, size_t) const;
    models::Matrix readWeights(std::istream &, size_t, size_t) const;
    bool rebuildLayers(std::istream &, NetHeader const&, NeuronNet &);
    void writeFileHeader(std::ostream &);
    void writeNetHeader(std::ostream &, NeuronNet const&);
    void writeLayer(std::ostream &, models::Layer const &);
    void writeUInt64(std::ostream &, uint64_t);
    void writeUInt32(std::ostream &, uint32_t);
    void writeUShort(std::ostream &, uint16_t);
    void writeSizeT(std::ostream &, size_t);
    uint64_t readUInt64(std::istream &) const;
    uint32_t readUInt32(std::istream &) const;
    uint16_t readUShort(std::istream &) const;
    size_t readSizeT(std::istream &) const;
    bool isBigEndian() const;
private:
    unsigned int _version = 0x0001000;
    bool _changeEndianness = false;
};

} // namespace neuronnet
} // namespace services
} // namespace mimir

#endif // NEURONNETSERIALIZER_H
