#include <QString>
#include <QtTest>

#include <deque>

#include "TestListener.h"

#include "tst_Models.h"

#include "../mimir/models/ValueType.h"
#include "../mimir/services/DataStore.h"
#include "../mimir/services/Evaluator.h"
#include "../mimir/services/NameResolver.h"

#include "../mimir/helpers/helpers.h"

using std::vector;

using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::ProbabilityWithPriors;
using mimir::models::ValueType;
using mimir::models::ValueIndex;
using mimir::services::DataStore;
using mimir::services::Evaluator;
using mimir::services::NameResolver;

REGISTER_TEST(Models)

Models::Models()
{
}

void Models::testSamplerBasics()
{
    NameResolver resolver;
    ValueIndex testeeNameIndex = resolver.indexFromName("testee");
}

void Models::testDataStore()
{
    NameResolver nameResolver;
    #define mkValueIndex(name) ValueIndex name = nameResolver.indexFromName(#name);
    ValueIndex kept = nameResolver.indexFromName("kept");
    ValueIndex cancelled = nameResolver.indexFromName("cancelled");
    ValueIndex returned = nameResolver.indexFromName("returned");

    mkValueIndex(ring);
    mkValueIndex(collier);
    mkValueIndex(brooch);

    mkValueIndex(blue);
    mkValueIndex(red);
    mkValueIndex(green);
    mkValueIndex(yellow);

    mkValueIndex(hadContact);
    mkValueIndex(noContact);

    mkValueIndex(hostMale);
    mkValueIndex(hostFemale);

    mkValueIndex(classification);
    mkValueIndex(type);
    mkValueIndex(colour);
    mkValueIndex(ccContact);
    mkValueIndex(hostSex);

    #undef mkValueIndex

    vector<vector<ValueIndex>> testData = {
        {cancelled, ring, blue, noContact, hostMale},
        {kept, ring, green, hadContact, hostFemale},
        {kept, ring, green, noContact, hostMale},
        {kept, brooch, red, hadContact, hostFemale},
        {kept, brooch, blue, noContact, hostMale},
        {returned, collier, green, hadContact, hostFemale},
        {returned, ring, red, noContact, hostMale},
        {cancelled, brooch, green, hadContact, hostFemale},
        {kept, ring, green, noContact, hostMale},
        {cancelled, ring, yellow, hadContact, hostFemale}
    };

    DataStore dataStore(nameResolver);
    QVERIFY(dataStore.columnCount() == 0);
    QVERIFY(dataStore.rowCount() == 0);
    dataStore.createDataSet({classification, type, colour, ccContact, hostSex}, classification);
    QVERIFY(dataStore.columnCount() == 5);
    QVERIFY(dataStore.rowCount() == 0);

    for (auto row : testData)
        dataStore.addRow(row);
    QVERIFY(dataStore.columnCount() == 5);
    QVERIFY(dataStore.rowCount() == 10);

    vector<vector<ValueIndex>> intersectExpectation = {
        { ring, green, hadContact },
        { ring, green, noContact },
        { collier, green, hadContact },
        { brooch, green, hadContact },
        { ring, green, noContact}
    };
    DataStore intersection = dataStore.intersect({type, colour, ccContact}, colour, green);
    QCOMPARE(intersection.rowCount(), 5ul);
    QCOMPARE(intersection.columnCount(), 3ul);
}

void Models::testNameLookup()
{
    NameResolver testee;
    ValueIndex vKept = testee.indexFromName("kept"); // #0
    ValueIndex vCancelled = testee.indexFromName("cancelled"); // #1
    ValueIndex vReturned = testee.indexFromName("returned"); // #2

    ValueIndex vPropertyColour = testee.indexFromName("colour"); // #3
    ValueIndex vPropertySize = testee.indexFromName("size"); // #4

    ValueIndex vValueBlue = testee.indexFromName("blue"); // #5

    ValueIndex vKept2 = testee.indexFromName("kept"); // #0 (created above)

    ValueIndex vSampler1 = testee.indexFromName("Sampler1"); // #6
    ValueIndex vColor = testee.indexFromName("colour"); // #3

    QCOMPARE(vKept, ValueIndex(0));
    QCOMPARE(vKept2, ValueIndex(0));
    QCOMPARE(vCancelled, ValueIndex(1));
    QCOMPARE(vReturned, ValueIndex(2));
    QCOMPARE(vPropertyColour, ValueIndex(3));
    QCOMPARE(vPropertySize, ValueIndex(4));
    QCOMPARE(vValueBlue, ValueIndex(5));
    QVERIFY(vSampler1 == 6_vi);
    QVERIFY(vColor == 3_vi);

    QVERIFY(testee.nameFromIndex(3_vi) == "colour");
}

void Models::testProbabilator()
{

}

void Models::testHelpers()
{
    vector<Probability> v1 = {
        (.1_p),
        (.2_p),
        (.3_p),
        (.4_p),
        (.5_p),
    };
    long double mean = mimir::helpers::mean(v1);
    QCOMPARE(static_cast<double>(mean), ((.1 + .2 + .3 + .4 + .5)/5.));

    std::deque<Probability> d1;
    d1.insert(d1.end(), v1.begin(), v1.end());
    auto deviation = mimir::helpers::deviation(d1);
    QCOMPARE(static_cast<double>(deviation), std::sqrt(((.1 *.1 + .2 * .2 + .3 *.3 +  .4 * .4 + .5 * .5) - ((.1 + .2 + .3 + .4 + .5)*(.1 + .2 + .3 + .4 + .5))/5) / 4));
}
