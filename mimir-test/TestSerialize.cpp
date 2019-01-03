#include "TestSerialize.h"
#include "TestListener.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

#include "services/NeuronNetSerializer.h"
#include "services/NameResolveSerializer.h"
#include "services/ActivationsManager.h"
#include "helpers/helpers.h"

REGISTER_TEST(TestSerialize)

TestSerialize::TestSerialize(QObject *parent) : QObject(parent)
{

}

void TestSerialize::testActivationManager()
{
    mimir::services::ActivationsManager manager;
    mimir::helpers::RectifiedLinear rectifiedLinear;
    size_t activationIndexRectLinear = manager.indexOf(&rectifiedLinear);
    QVERIFY(activationIndexRectLinear == 1);
    mimir::models::Layer layer(nullptr);
    QVERIFY(manager.indexOf(layer.activation()) == -1ul);
    layer.setActivation(&rectifiedLinear);
    QVERIFY(manager.indexOf(layer.activation()) == 1);
}

void TestSerialize::testPackUnpack()
{
    auto expect = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
    mimir::helpers::PackDouble pack;
    mimir::helpers::UnpackDouble unpack;
    auto packed = pack(expect);
    auto unpacked = unpack(packed);
    // using "==" here 'cause it actually should result in the same bit mask.
    QVERIFY(unpacked == expect);
}

void TestSerialize::testSerialize()
{
    mimir::services::NeuronNetSerializer serializer;
    mimir::services::NeuronNet net(2, 1, "rectifiedLinear");
    net.addHiddenLayer(4);
    net.connect();
    net.layers().front().setWeights({{{1, 2, 3, 4},{5, 6, 7, 8}}});
    net.layers()[1].setWeights(mimir::models::Matrix{std::vector<double>{4, 3, 2, 1}});
    net.setBias(1, 0, 1);
    net.setBias(1, 1, -1);
    net.setBias(1, 2, 1);
    net.setBias(1, 3, -1);
    net.setBias(2, 0, -5);
    net.setName("serializeTest");
    std::stringstream buffer;
    serializer.serialize(net, buffer);
    mimir::services::NeuronNet rereadAndCopied;
    {
        mimir::services::NeuronNet reread;
        buffer.seekg(0);
        serializer.deserialize(reread, buffer);
        QVERIFY(reread.name() == net.name());
        QVERIFY(reread.layers().size() == net.layers().size());
        rereadAndCopied = reread;
    }

    QVERIFY(net.run({0, 1}) == rereadAndCopied.run({0, 1}));
}

void TestSerialize::testNameResolveSerialize()
{
    mimir::services::NameResolver resolver;
    resolver.indexFromName("Hello");
    resolver.indexFromName("World");
    resolver.indexFromName("42");
    resolver.indexFromName("3.1459");
    std::stringstream stream;
    mimir::services::NameResolveSerializer serializer;
    serializer.serialize(stream, resolver);
    stream.seekg(0);
    auto restored = serializer.deserialize(stream);
    QVERIFY(restored.wordList() == resolver.wordList());
}
