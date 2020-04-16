#include "TestMatrix.h"

#include "TestListener.h"

#include "models/Matrix.h"

REGISTER_TEST(TestMatrix)

using mimir::models::Matrix;

static bool valarraysEqual(const std::valarray<float> &left, const std::valarray<float> &right)
{
    return (left == right).min() == true;
}

TestMatrix::TestMatrix()
{

}

void TestMatrix::testEquals()
{
    Matrix m1({
                  {1, 2, 3},
                  {4, 5, 6}
              });
    Matrix m2({
                  {1, 2},
                  {3, 4},
                  {5, 6}
              });
    Matrix m3(m1);
    Matrix m4({
                  {1, 2, 3},
                  {4, 5, 7}
              });

    QVERIFY(!(m1 == m2));
    QVERIFY(m1 != m2);
    QVERIFY(m1 == m3);
    QVERIFY(!(m1 != m3));
    QVERIFY(m1 != m4);

}

void TestMatrix::testTranspose()
{
    Matrix m1;
    m1.addRow({1, 2, 3});
    m1.addRow({4, 5, 6});
    m1.transpose();
    auto result = m1.data();
    QVERIFY(m1 == Matrix({{1,4},{2,5},{3,6}}));
}

void TestMatrix::testMultiply()
{
    Matrix m1;
    Matrix m2;
    m1.addRow({1,  2,  3,  4});
    m1.addRow({5,  6,  7,  8});
    m1.addRow({9, 10, 11, 12});

    m2.addRow({ 1,   2,  3});
    m2.addRow({ 4,   5,  6});
    m2.addRow({ 7,   8,  9});
    m2.addRow({10,  11, 12});

    Matrix resultMatrix = m1 * m2;
    auto result = resultMatrix.data();
    QVERIFY(result.size() == 3);
    QVERIFY(valarraysEqual({ 70,  80,  90},result[0]));
    QVERIFY(valarraysEqual({158, 184, 210},result[1]));
    QVERIFY(valarraysEqual({246, 288, 330},result[2]));

    Matrix m3{std::vector<std::valarray<float>>{{1, 2}}};
    QVERIFY_EXCEPTION_THROWN(m1*m3, std::logic_error);
}

void TestMatrix::testOtherMultiplications()
{
    Matrix m1{
        {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        }
    };
    std::vector<float> vector{2, 2, 2};
    m1 *= vector;
    QVERIFY(m1 == Matrix(std::vector<std::valarray<float>>{{12},{30},{48}}));
    m1 = Matrix(std::vector<std::valarray<float>>{{1},{3},{9}});
    std::valarray<float> array{2, 2, 2};
    m1 *= array;
    QVERIFY(m1 == Matrix({{2,2,2},{6,6,6},{18,18,18}}));
    m1 *= .5;
    QVERIFY(m1 == Matrix({{1,1,1},{3,3,3},{9,9,9}}));
    m1 = Matrix({{.9, .1, .2}, {.9, .1, .2}, {.9, .1, .2}});
    Matrix m2 = m1 * std::vector<float>{.2, .2, .2};
    QVERIFY(m2 == Matrix(std::vector<std::valarray<float>>{{.24},{.24},{.24}}));
}

void TestMatrix::testDiff()
{
    Matrix m1({
                {1, 2, 3},
                {4, 5, 6}
              });
    m1 -= Matrix({
        { 3, 3, 3},
        { 3, 3, 3}
    });
    QVERIFY(m1 == Matrix({
                             { -2, -1, 0},
                             {  1,  2, 3}
                         }));
    m1 = Matrix({{1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1}});
    m1 -= Matrix(std::vector<float>{.1, .1, .1, .1, .1});
    QVERIFY(m1 == Matrix({
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                         }));
    m1 = Matrix({{1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1}});
    m1 -= std::vector<float>{.1, .1, .1, .1, .1};
    QVERIFY(m1 == Matrix({
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                             { .9, .9, .9, .9, .9},
                         }));
    m1 = Matrix({
                    {1, 2, 3},
                    {4, 5, 6}
                  });
    m1 -= std::valarray<float>{1, 2, 3};
    QVERIFY(m1 == Matrix ({{0, 0, 0},
                           {3, 3, 3}}));
}

void TestMatrix::testCallbackConstructor()
{
    float vals[2][3] = {{1, 2, 3}, {4, 5, 6}};
    auto setter = [&vals](size_t row, size_t col) -> float { return vals[row][col];};
    Matrix cbAssigned(2, 3, setter);
    QVERIFY(cbAssigned == Matrix({{1, 2, 3}, {4, 5, 6}}));
}

void TestMatrix::testInsertRow()
{
    Matrix m({
                 {2, 4},
                 {7, 9},
                 {12, 14}
             });
    Matrix expect({
                      { 1,  2,  3,  4,  5},
                      { 6,  7,  8,  9, 10},
                      {11, 12, 13, 14, 15}
                  });
    m.addColumn(0, {1, 6, 11}); // front insert
    m.addColumn(2, {3, 8, 13}); // somewhere in the middle insert
    m.addColumn(4, {5, 10, 15}); // past end. append that column
    QVERIFY(m == expect);
}
