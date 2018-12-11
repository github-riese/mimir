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
    Matrix &transpose();
    Matrix transposed() const;
    std::vector<std::valarray<double>>const & data() const;
    void addRow(std::valarray<double> const &);
    Matrix & operator*=(const Matrix &rhs);
    Matrix operator *(const Matrix &rhs);
    Matrix &operator *=(std::vector<double> const&);
    Matrix &operator *=(std::valarray<double> const &);
    Matrix &operator *=(double);
    size_t cols() const;
    size_t rows() const;
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
