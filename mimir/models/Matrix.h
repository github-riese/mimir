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
    Matrix(std::vector<std::valarray<double>> const &) noexcept;
    explicit Matrix(std::vector<double> const&) noexcept;
    explicit Matrix(std::valarray<double> const&) noexcept;
    Matrix(size_t rows, size_t colums, double initalValue = 1.) noexcept;
    explicit Matrix(size_t rows, size_t columns, std::function<double(size_t, size_t)> setter);
    Matrix &transpose();
    Matrix transposed() const;
    std::vector<std::valarray<double> > data() const;
    std::vector<double> column(size_t column) const;
    void addColumn(size_t before = -1u, std::vector<double> const &values = {});
    void addRow(std::valarray<double> const &);
    Matrix & operator*=(const Matrix &rhs);
    Matrix operator *(const Matrix &rhs) const;
    Matrix operator *(std::vector<double> const&) const;
    Matrix operator *(std::valarray<double> const&) const;
    Matrix operator *(double) const;
    Matrix &operator *=(std::vector<double> const&);
    Matrix &operator *=(std::valarray<double> const &);
    Matrix &operator *=(double);
    Matrix &pieceWiseMultiply(Matrix const &);
    Matrix &operator /=(double);
    Matrix operator -(Matrix const &) const;
    Matrix &operator -=(Matrix const &);
    Matrix &operator -=(std::vector<double> const&);
    Matrix &operator -=(std::valarray<double> const &);
    Matrix operator +(Matrix const &) const;
    Matrix operator +(double) const;
    Matrix &operator +=(double);
    Matrix &operator -=(double);
    Matrix& operator +=(Matrix const &);
    Matrix& operator += (const std::valarray<double> &);
    Matrix operator +(const std::valarray<double> &) const;
    size_t cols() const;
    size_t rows() const;
    double value(size_t row, size_t column) const;
    void setValue(size_t row, size_t column, double value);
    void fill(double value);
    Matrix &makeIdentity();
    double sum() const;
    double avg() const;
    double mse() const;
    bool operator==(Matrix const&) const;
    bool operator!=(Matrix const&) const;
private:
    std::valarray<double> array() const;
    using Column = std::valarray<double>;
    size_t _rows = 0;
    size_t _cols = 0;
    std::vector<double> _data;
};

Matrix operator*(std::vector<double> const &vect, Matrix const &matrix);
Matrix operator*(std::valarray<double> const &arr, Matrix const &matrix);
Matrix operator*(double d, Matrix const &matrix);

} // namespace models
} // namespace mimir

#endif // MATRIX_H
