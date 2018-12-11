#include "Matrix.h"

namespace mimir {
namespace models {


Matrix::Matrix(const std::vector<std::valarray<double> > &values) :
    _rows(values)
{

}

Matrix::Matrix(const std::vector<double> &vector)
{
    for(auto value : vector) {
        addRow({value});
    }
}

Matrix::Matrix(const std::valarray<double> &array) :
    _rows({array})
{
}

Matrix Matrix::transposed() const
{
    if (_rows.size() == 0) {
        return *this;
    }
    size_t columns = _rows.front().size();
    size_t rows = _rows.size();
    std::vector<std::valarray<double>> result(columns, std::valarray<double>(rows));
    size_t row, column;
    for (row = 0; row < rows; ++row) {
        for (column = 0; column < columns; ++column) {
            result[column][row] = _rows[row][column];
        }
    }
    Matrix out;
    out._rows = result;
    return out;
}

std::vector<std::valarray<double>>const &Matrix::data() const
{
    return _rows;
}

void Matrix::addRow(const std::valarray<double> &row)
{
    if (_rows.size() && _rows.front().size() != row.size()) {
        throw std::logic_error("matrix value size mismatch");
    }
    _rows.push_back(row);
}

Matrix &Matrix::operator*=(const Matrix &rhs)
{
    if (rhs.rows() != cols()) {
        throw std::logic_error("can't multiply matrices where left number of columns doesn't match right number of rows.");
    }
    auto leftArray = array();
    auto rightArray = rhs.array();
    std::valarray<double> result(0., rows() * rhs.cols());
    auto lcols = cols();
    auto lrows = rows();
    auto rcols = rhs.cols();
    for (size_t i = 0; i < lrows; ++i) {
        for (size_t j = 0; j < rcols; ++j) {
            for (size_t k = 0; k < lcols; ++k) {
                result[i * rcols + j] += leftArray[i * lcols + k] * rightArray[rcols * k + j ];
            }
        }
    }
    auto cpBegin = std::begin(result);
    auto cpStride = cpBegin + rcols;
    _rows.clear();
    auto rows = result.size()/rcols;
    while (rows > 0) {
        std::valarray<double> tmp(rcols);
        std::copy(cpBegin, cpStride, std::begin(tmp));
        _rows.push_back(tmp);
        cpBegin += rcols;
        cpStride += rcols;
        --rows;
    }
    return *this;
}

Matrix Matrix::operator *(const Matrix &rhs)
{
    Matrix result = *this;
    return result *= rhs;
}

Matrix &Matrix::operator *=(const std::vector<double> &vector)
{
    Matrix t{vector};
    return *this *= t;
}

Matrix &Matrix::operator *=(const std::valarray<double> &valarray)
{
    Matrix t{{valarray}};
    return *this *= t;
}

Matrix &Matrix::operator *=(double value)
{
    for(auto &row : _rows) {
        row *= value;
    }
    return *this;
}

size_t Matrix::cols() const
{
    return _rows.size() > 0 ? _rows.front().size() : 0;
}

size_t Matrix::rows() const
{
    return _rows.size();
}

bool Matrix::operator==(const Matrix &rhs) const
{
    if (rows() != rhs.rows()) {
        return false;
    }
    if (cols() != rhs.cols()) {
        return false;
    }
    return (array() == rhs.array()).min() == true;
}

bool Matrix::operator!=(const Matrix &rhs) const
{
    return !(*this == rhs);
}

std::valarray<double> Matrix::array() const
{
    if (0 == _rows.size()) {
        return std::valarray<double>();
    }
    auto columns = cols();
    std::valarray<double> result(columns * rows());
    auto resultIterator = std::begin(result);
    for (auto row : _rows) {
        std::copy(std::begin(row), std::end(row), resultIterator);
        resultIterator += columns;
    }
    return result;
}

Matrix& Matrix::transpose()
{
    auto tmp = transposed();
    _rows = tmp._rows;
    return *this;
}

} // namespace models
} // namespace mimir
