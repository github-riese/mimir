#include <QString>
#include <QtTest>

#include <deque>

#include "TestListener.h"

#include "tst_Models.h"

#include "../mimir/models/ProbabilityDistribution.h"
#include "../mimir/services/DataStore.h"
#include "../mimir/services/Evaluator.h"
#include "../mimir/services/NameResolver.h"

#include "../mimir/helpers/helpers.h"

using std::vector;

using mimir::models::ColumnNameValuePair;
using mimir::models::CPT;
using mimir::models::Evaluation;
using mimir::models::Probability;
using mimir::models::ProbabilityDistribution;
using mimir::models::ProbabilityWithPriors;
using mimir::models::ValueIndex;
using mimir::services::DataStore;
using mimir::services::Evaluator;
using mimir::services::NameResolver;

REGISTER_TEST(Models)

Models::Models() :
    _dataStore(_nameResolver)
{
}

void Models::initTestCase()
{
    #define mkValueIndex(name) ValueIndex name = _nameResolver.indexFromName(#name);
    ValueIndex kept = _nameResolver.indexFromName("kept");
    ValueIndex cancelled = _nameResolver.indexFromName("cancelled");
    ValueIndex returned = _nameResolver.indexFromName("returned");

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
    _dataStore.createDataSet({classification, type, colour, ccContact, hostSex});
    for (auto row : testData)
        _dataStore.addRow(row);
}

void Models::testDataStore()
{
    ValueIndex type = _nameResolver.indexFromName("type");
    ValueIndex colour = _nameResolver.indexFromName("colour");
    ValueIndex ccContact = _nameResolver.indexFromName("ccContact");
    ValueIndex ring = _nameResolver.indexFromName("ring");
    ValueIndex green = _nameResolver.indexFromName("green");
    ValueIndex noContact = _nameResolver.indexFromName("noContact");

    QVERIFY(_dataStore.columnCount() == 5);
    QVERIFY(_dataStore.rowCount() == 10);

    CPT cpt = _dataStore.createConditionalProbabilityTable({{type, colour, ccContact}});
    Probability probabilityOfRingGreenContact = cpt.probability(vector<ColumnNameValuePair>{{type, ring} , {colour, green}, {ccContact, noContact}});
    QVERIFY(probabilityOfRingGreenContact == .2_p);
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

void Models::testHelpers()
{
    vector<Probability> v1 = {
        (.1_p),
        (.2_p),
        (.3_p),
        (.4_p),
        (.5_p),
    };
    double mean = mimir::helpers::mean(v1);
    QCOMPARE(static_cast<double>(mean), ((.1 + .2 + .3 + .4 + .5)/5.));

    std::vector<Probability> v2;
    v2.insert(v2.end(), v1.begin(), v1.end());
    auto deviation = mimir::helpers::deviation(v2);
    QCOMPARE(deviation, .158113883008419);
    QCOMPARE(mimir::helpers::variance(v2), .025);
}

void Models::testCPT()
{
    ValueIndex classification = _nameResolver.indexFromName("classification");
    ValueIndex type = _nameResolver.indexFromName("type");
    ValueIndex colour = _nameResolver.indexFromName("colour");
    ValueIndex ccContact = _nameResolver.indexFromName("ccContact");
    ValueIndex ring = _nameResolver.indexFromName("ring");
    ValueIndex green = _nameResolver.indexFromName("green");
    ValueIndex noContact = _nameResolver.indexFromName("noContact");
    ValueIndex kept = _nameResolver.indexFromName("kept");
    ValueIndex cancelled = _nameResolver.indexFromName("cancelled");
    ValueIndex returned = _nameResolver.indexFromName("returned");

    CPT grandTable = _dataStore.createConditionalProbabilityTable();
    Probability p = grandTable.probability(vector<ColumnNameValuePair>{{type, ring}, {colour, green}});
    QVERIFY(p == .3_p);

    vector<ValueIndex> uniqueClasses = grandTable.distinctValues(classification);
    QCOMPARE(uniqueClasses, (vector<ValueIndex>{ kept, cancelled, returned }));

    ProbabilityDistribution distribution = grandTable.classify(classification, {{type, ring}, {ccContact, noContact }});
    ProbabilityDistribution dist2 = grandTable.classify(type, { {classification, kept}, {colour, green} });
    QCOMPARE(distribution.mostProbable(), kept);
    QCOMPARE(distribution.argMax(), .5_p);
    QCOMPARE(distribution.vectorLength(), .25);

    QCOMPARE((dist2.max()), (mimir::models::NamedProbability{ring, 1._p}));
}
