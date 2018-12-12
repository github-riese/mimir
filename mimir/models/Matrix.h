#ifndef MATRIX_H
#define MATRIX_H

#include <valarray>
#include <vector>

namespace mimir {
namespace models {

class Matrix
{
public:
    Matrix() = default;
    Matrix(std::vector<std::valarray<double>> const &);
    Matrix(std::vector<double> const&);
    Matrix(std::valarray<double> const&);
    Matrix(size_t rows, size_t colums, double initalValue = 1.);
    Matrix &transpose();
    Matrix transposed() const;
    std::vector<std::valarray<double>>const & data() const;
    std::vector<double> column(size_t column) const;
    void addRow(std::valarray<double> const &);
    Matrix & operator*=(const Matrix &rhs);
    Matrix operator *(const Matrix &rhs);
    Matrix &operator *=(std::vector<double> const&);
    Matrix &operator *=(std::valarray<double> const &);
    Matrix &operator *=(double);
    Matrix &operator -=(Matrix const &);
    size_t cols() const;
    size_t rows() const;
    double value(size_t row, size_t column) const;
    void setValue(size_t row, size_t column, double value);
    bool operator==(Matrix const&) const;
    bool operator!=(Matrix const&) const;
private:
    std::valarray<double> array() const;
    using Column = std::valarray<double>;
    std::vector<Column> _rows;
};

} // namespace models
} // namespace mimir

#endif // MATRIX_H
