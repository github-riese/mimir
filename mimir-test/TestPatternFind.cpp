#include "TestPatternFind.h"
#include <QDebug>

#include "../mimir/services/Sampler.h"
#include "../mimir/services/Evaluator.h"

using std::vector;

using mimir::models::ValueIndex;
using mimir::services::Sampler;
using mimir::models::Sample;
using mimir::models::Evaluation;
using mimir::services::Evaluator;
using mimir::services::NameResolver;

#define mkValueIndex(name) ValueIndex name = _nameResolver.indexFromName(NameResolver::NameSource::Value, #name);
TestPatternFind::TestPatternFind()
{

}

void TestPatternFind::initTestCase()
{
    ValueIndex kept = _nameResolver.indexFromName(NameResolver::NameSource::Classification, "kept");
    ValueIndex cancelled = _nameResolver.indexFromName(NameResolver::NameSource::Classification, "cancelled");
    ValueIndex returned = _nameResolver.indexFromName(NameResolver::NameSource::Classification, "returned");

    mkValueIndex(ring);
    mkValueIndex(collier);
    mkValueIndex(brooch);

    mkValueIndex(blue);
    mkValueIndex(red);
    mkValueIndex(green);

    mkValueIndex(hadContact);
    mkValueIndex(noContact);

    mkValueIndex(hostMale);
    mkValueIndex(hostFemale);

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
        {cancelled, ring, blue, hadContact, hostFemale}
    };

    _testSamplers.reserve(4);
    _testSamplers.push_back(Sampler(_nameResolver.indexFromName(NameResolver::NameSource::Sampler, "type")));
    _testSamplers.push_back(Sampler(_nameResolver.indexFromName(NameResolver::NameSource::Sampler, "colour")));
    _testSamplers.push_back(Sampler(_nameResolver.indexFromName(NameResolver::NameSource::Sampler, "agentContact")));
    _testSamplers.push_back(Sampler(_nameResolver.indexFromName(NameResolver::NameSource::Sampler, "presenterSex")));

    for (auto row : testData) {
        for (size_t i = 1; i < row.size(); ++i) {
            _testSamplers[i - 1].addSample(Sample(row[0], row[i]));
        }
    }
}

void TestPatternFind::testPreCheckAssumptionThatDataTurnOutAOne()
{
    Evaluator e;
    Evaluation typeEvaluation = e.evaluate(_testSamplers[0], _nameResolver.indexFromName(NameResolver::NameSource::Value, "ring"));
    Evaluation colourEvaluation = e.evaluate(_testSamplers[1], _nameResolver.indexFromName(NameResolver::NameSource::Value, "green"));

    QVERIFY(typeEvaluation.probabilityByClassification(ValueIndex(0)).probability() < 1.L);
    QVERIFY(colourEvaluation.probabilityByClassification(ValueIndex(0)).probability() < 1.L);

    Evaluation crossRingGreen = e.evaluate({typeEvaluation, colourEvaluation});
    QVERIFY(crossRingGreen.probabilityByClassification(ValueIndex(0)) == 1.L);
}

