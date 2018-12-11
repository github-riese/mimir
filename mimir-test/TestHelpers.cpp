#include "TestHelpers.h"

#include <QDebug>

#include <iterator>
#include <valarray>
#include "helpers/math.h"

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

void TestHelpers::testMultiply()
{
    std::valarray<int> left {1, 2, 3};
    std::valarray<int> result1 = left * 4;
    std::vector<int> cmp(std::begin(result1), std::end(result1));
    QCOMPARE((std::vector<int>{4, 8, 12}), cmp);
    std::valarray<int> right {4, 5, 6};
    std::vector<int> expect {-3, 6, -3};
    auto result = mimir::helpers::math::crossProduct(left, right);
    cmp.clear();
    for(auto v : result) cmp.push_back(v);
    QCOMPARE(cmp, expect);

    left = {1, 2 ,3, 4, 5, 6, 7, 8, 9, 10};
    right = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    result = mimir::helpers::math::crossProduct(left, right);
    std::vector<int> out(std::begin(result), std::end(result));
    QCOMPARE(out, (std::vector<int>{55, -55, 55, -55, 55, -55, 55, -55, 55, -55}));

    std::valarray<int> matrixResult = mimir::helpers::math::matrixStyleMult(std::valarray{1,2, 3}, std::valarray{7, 8, 9});
    out.clear();
    for(auto v : matrixResult) out.push_back(v);
    qDebug() << out;

    auto resultInvokedAdVector = mimir::helpers::math::crossProduct(expect, out);
    QCOMPARE((std::vector<int>{468, 36, -396}), resultInvokedAdVector);
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


