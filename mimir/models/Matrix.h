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
    Matrix(std::vector<std::valarray<float>> const &) noexcept;
    explicit Matrix(std::vector<float> const&) noexcept;
    explicit Matrix(std::valarray<float> const&) noexcept;
    Matrix(size_t rows, size_t colums, float initalValue = 1.) noexcept;
    explicit Matrix(size_t rows, size_t columns, std::function<float(size_t, size_t)> setter);
    Matrix &transpose();
    Matrix transposed() const;
    std::vector<std::valarray<float> > data() const;
    std::vector<float> column(size_t column) const;
    std::valarray<float> row(size_t row) const;
    void addColumn(size_t before = -1u, std::vector<float> const &values = {});
    void addRow(std::valarray<float> const &);
    Matrix & operator*=(const Matrix &rhs);
    Matrix operator *(const Matrix &rhs) const;
    Matrix operator *(std::vector<float> const&) const;
    Matrix operator *(std::valarray<float> const&) const;
    Matrix operator *(float) const;
    Matrix &operator *=(std::vector<float> const&);
    Matrix &operator *=(std::valarray<float> const &);
    Matrix &operator *=(float);
    Matrix &pieceWiseMultiply(Matrix const &);
    Matrix &operator /=(float);
    Matrix operator -(Matrix const &) const;
    Matrix &operator -=(Matrix const &);
    Matrix &operator -=(std::vector<float> const&);
    Matrix &operator -=(std::valarray<float> const &);
    Matrix operator +(Matrix const &) const;
    Matrix operator +(float) const;
    Matrix &operator +=(float);
    Matrix &operator -=(float);
    Matrix& operator +=(Matrix const &);
    Matrix& operator += (const std::valarray<float> &);
    Matrix operator +(const std::valarray<float> &) const;
    size_t cols() const;
    size_t rows() const;
    float value(size_t row, size_t column) const;
    void setValue(size_t row, size_t column, float value);
    void fill(float value);
    Matrix &makeIdentity();
    float sum() const;
    float avg() const;
    float mse() const;
    bool operator==(Matrix const&) const;
    bool operator!=(Matrix const&) const;
private:
    std::valarray<float> array() const;
    using Column = std::valarray<float>;
    size_t _rows = 0;
    size_t _cols = 0;
    std::vector<float> _data;
};

} // namespace models
} // namespace mimir

#endif // MATRIX_H
