#include "TestMatrix.h"

#include "TestListener.h"

#include "models/Matrix.h"

REGISTER_TEST(TestMatrix)

using mimir::models::Matrix;

static bool valarraysEqual(const std::valarray<double> &left, const std::valarray<double> &right)
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
    m1.addRow({1, 2, 3});
    m1.addRow({4, 5, 6});

    m2.addRow({1, 2});
    m2.addRow({3, 4});
    m2.addRow({5, 6});

    Matrix resultMatrix = m1 * m2;
    auto result = resultMatrix.data();
    QVERIFY(result.size() == 2);
    QVERIFY(valarraysEqual({22, 28},result[0]));
    QVERIFY(valarraysEqual({49, 64},result[1]));

    Matrix m3{std::vector<std::valarray<double>>{{1, 2}}};
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
    std::vector<double> vector{2, 2, 2};
    m1 *= vector;
    QVERIFY(m1 == Matrix(std::vector<std::valarray<double>>{{12},{30},{48}}));
    m1 = Matrix(std::vector<std::valarray<double>>{{1},{3},{9}});
    std::valarray<double> array{2, 2, 2};
    m1 *= array;
    QVERIFY(m1 == Matrix({{2,2,2},{6,6,6},{18,18,18}}));
    m1 *= .5;
    QVERIFY(m1 == Matrix({{1,1,1},{3,3,3},{9,9,9}}));
}
