#include "TestHelpers.h"

#include <QDebug>

#include <iterator>
#include <valarray>
#include "helpers/math.h"

using namespace mimir::helpers::math;

REGISTER_TEST(TestHelpers)

template <typename T>
static bool valArraysEqual(std::valarray<T> const &left, std::valarray<T> const &right)
{
    if (left.size() != right.size()) {
        return false;
    }
    return (left == right).min() == true;
}

TestHelpers::TestHelpers(QObject *parent) : QObject(parent)
{

}

void TestHelpers::testDiffVectors()
{
    std::valarray<double> left{10,  9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::valarray<double> right{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::valarray<double> expect{9, 7, 5, 3, 1, -1, -3, -5, -7, -9};
    auto result = left - right;
    QVERIFY((expect == result).min());
}

void TestHelpers::testMatrix()
{
    std::vector<std::valarray<double>> testee = {
        {1,  2,  3,  4},
        {5,  6,  7,  8},
        {9, 10, 11, 12}};
    auto transposed = mimir::helpers::math::transposeMatrix(testee);
    QVERIFY(transposed.size() == 4);
    QVERIFY(valArraysEqual(transposed[0], {1., 5.,  9.}));
    QVERIFY(valArraysEqual(transposed[1], {2., 6., 10.}));
    QVERIFY(valArraysEqual(transposed[2], {3., 7., 11.}));
    QVERIFY(valArraysEqual(transposed[3], {4., 8., 12.}));
}

void TestHelpers::testDot()
{
    std::valarray<double> left{1, 2, 3, 4};
    std::vector<double> right{1, 2, 3, 4};

    double result = left * right;
    QVERIFY(result == 30.);
}

void TestHelpers::testVector()
{
    std::vector<double> left {1, 2, 3, 4, 5};
    auto maxElem = *std::max_element(left.begin(), left.end());
    QVERIFY(maxElem == 5.);
    left -= maxElem;
    QVERIFY((left == std::vector<double>{-4., -3., -2., -1., 0.}));
    auto added = left + 5.;
    QVERIFY((added == std::vector<double>{1, 2, 3, 4, 5}));

    auto subbed = added - 5.;
    QVERIFY(subbed == left);
}




