#include "NameResolveSerializer.h"

#include <istream>
#include <ostream>

namespace mimir {
namespace services {

#define FILE_MAGIC "mimir::words"
union serializableSizeT {
    size_t value;
    char serializable[sizeof (value)];
};

inline void writeSize(std::ostream &stream, size_t size)
{
    serializableSizeT writeSize;
    writeSize.value = ntohll(size);
    stream.write(writeSize.serializable, sizeof (serializableSizeT));
}

inline size_t readSize(std::istream &stream)
{
    serializableSizeT readSize;
    stream.read(readSize.serializable, sizeof (serializableSizeT));
    return ntohll(readSize.value);
}

inline std::string readWord(std::istream &stream)
{
    size_t length = readSize(stream);
    char *buffer = new char[length + 1];
    buffer[length] = 0;
    stream.read(buffer, static_cast<long>(length));
    std::string result(buffer);
    delete [] buffer;
    return result;
}

void NameResolveSerializer::serialize(std::ostream &stream, const NameResolver &resolver) const
{
    auto names = resolver.wordList();
    stream << FILE_MAGIC;
    writeSize(stream, names.size());
    for (auto name : names) {
        writeSize(stream, name.size());
        stream << name;
    }
}

NameResolver NameResolveSerializer::deserialize(std::istream &stream) const
{
    if (!checkMagic(stream)) {
        return {};
    }
    size_t words = readSize(stream);
    std::vector<std::string> wordList;
    for (auto n = 0u; n < words; ++n)
    {
        wordList.push_back(readWord(stream));
    }
    NameResolver result;
    result.setWordList(wordList);
    return result;
}

bool NameResolveSerializer::checkMagic(std::istream &stream) const
{
    auto current = stream.tellg();
    char buffer[sizeof(FILE_MAGIC)] = {};
    stream.read(buffer, sizeof (buffer)-1);
    if (std::string(buffer) == FILE_MAGIC) {
        return true;
    }
    stream.seekg(current);
    return false;
}


} // namespace services
} // namespace mimir
