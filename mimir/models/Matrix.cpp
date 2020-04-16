#include "Matrix.h"

#include <algorithm>
#include <numeric>

#include "helpers/helpers.h"

namespace mimir {
namespace models {


Matrix::Matrix(const std::vector<std::valarray<float> > &values) noexcept :
    _rows(values.size()),
    _cols(values.front().size())
{
    _data.assign(_rows * _cols, 0);
    auto offset = _data.begin();
    for(auto value : values) {
        std::copy(std::begin(value), std::end(value), offset);
        std::advance(offset, static_cast<long>(_cols));
    }
}

Matrix::Matrix(const std::vector<float> &vector) noexcept :
    _rows(vector.size()),
    _cols(1)
{
    _data.assign(vector.begin(), vector.end());
}

Matrix::Matrix(const std::valarray<float> &array) noexcept :
    _rows(1),
    _cols(array.size())
{
    _data.assign(std::begin(array), std::end(array));
}

Matrix::Matrix(size_t rows, size_t colums, float initalValue) noexcept :
    _rows(rows),
    _cols(colums)
{
    _data.assign(_rows * _cols, initalValue);
}

Matrix::Matrix(size_t rows, size_t columns, std::function<float(size_t, size_t)> setter) :
    _rows(rows),
    _cols(columns)
{
    size_t elements = _rows * _cols;
    _data.reserve(elements);
    size_t offset = 0;
    while (offset < elements) {
        _data.push_back(setter(offset/_cols, offset%_cols));
        ++offset;
    }
}

Matrix Matrix::transposed() const
{
    return Matrix{*this}.transpose();
}

std::vector<std::valarray<float>> Matrix::data() const
{
    std::vector<std::valarray<float>> out;
    auto pointer = _data.begin();
    for (auto row = 0ul; row < _rows; ++row) {
        out.push_back({&(*pointer), _cols});
        std::advance(pointer, static_cast<long>(_cols));
    }
    return out;
}

std::vector<float> Matrix::column(size_t column) const
{
    if (column >= _cols) {
        throw std::out_of_range("No such column.");
    }
    if (_cols == 1 && column == 0) {
        return _data;
    }
    std::vector<float> result;
    result.reserve(_rows);
    auto pointer = _data.begin();
    while (pointer != _data.end()) {
        result.push_back(*pointer);
        std::advance(pointer, static_cast<long>(_cols));
    }
    return result;
}

std::valarray<float> Matrix::row(size_t row) const
{
    if (row >= _rows) {
        throw std::out_of_range("No such row.");
    }
    if (_rows == 1 && row == 0) {
        return mimir::helpers::toArray(_data);
    }
    std::valarray<float> result(_cols);
    long start = static_cast<long>(row * _cols);
    std::copy(_data.begin() + start, _data.begin() + start + static_cast<long>(_cols), std::begin(result));
    return result;
}

void Matrix::addColumn(size_t before, const std::vector<float> &values)
{
    if (before != -1u && before > _cols) {
        throw new std::logic_error("Cant' add a column behind the end of the matrix.");
    }
    if (!values.empty() && values.size() != _rows) {
        throw std::logic_error("Can't add column: wrong size of column vector.");
    }
    auto value = values.rbegin();
    _data.resize(_data.size() + _rows);
    auto read = _data.rbegin() + static_cast<long>(_rows);
    auto write = _data.rbegin();
    if (before <= _cols) {
        auto n = _cols - before;
        while(n-- > 0) {
            *write++ = *read++;
        }
        *write++ = (value == values.rend()) ? 0. : *value++;
    }
    while(read != _data.rend()) {
        for (auto n = 0u; n < _cols && read != _data.rend(); ++n) {
            *write++ = *read++;
        }
        if (write == _data.rend()) {
            break;
        }
        *write++ = (value == values.rend()) ? 0. : *value++;
    }
    ++_cols;
}

void Matrix::addRow(const std::valarray<float> &row)
{
    if (_rows != 0 && _cols != row.size()) {
        throw std::logic_error("Matrix value size mismatch.");
    } else if (_rows == 0) {
        _cols = row.size();
    }
    _data.insert(_data.end(), std::begin(row), std::end(row));
    ++_rows;
}

Matrix &Matrix::operator*=(const Matrix &rhs)
{
    if (cols() != rhs.rows()) {
        throw std::logic_error("can't multiply matrices where left number of columns doesn't match right number of rows.");
    }
    Matrix rightColumns = rhs.transposed();
    std::vector<float> result;
    result.assign(_rows * rhs._cols, 0);
    auto resultIterator = result.begin();
    auto leftIterator = _data.begin();
    for (size_t i = 0; i < rows(); ++i) {
        auto rightIterator = rightColumns._data.begin();
        for (size_t j = 0; j < rhs.cols(); ++j) {
            *(resultIterator + static_cast<long>(j)) = std::inner_product(leftIterator, leftIterator + static_cast<long>(_cols), rightIterator, 0.);
            std::advance(rightIterator, static_cast<long>(rightColumns._cols));
        }
        resultIterator += static_cast<long>(rhs._cols);
        leftIterator += static_cast<long>(_cols);
    }
    _data = result;
    _cols = rhs.cols();
    return *this;
}

Matrix Matrix::operator *(const Matrix &rhs) const
{
    return Matrix{*this} *= rhs;
}

Matrix Matrix::operator *(const std::vector<float> &vector) const
{
    return Matrix{*this} *= Matrix{vector};
}

Matrix Matrix::operator *(const std::valarray<float> &array) const
{
    return Matrix{*this} *= array;
}

Matrix Matrix::operator *(float v) const
{
    return Matrix{*this} *= v;
}

Matrix &Matrix::operator *=(const std::vector<float> &vector)
{
    return *this *= Matrix {vector};
}

Matrix &Matrix::operator *=(const std::valarray<float> &valarray)
{
    return *this *= Matrix{{valarray}};
}

Matrix &Matrix::operator *=(float value)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [value](float v) -> float { return v * value; });
    return *this;
}

mimir::models::Matrix &mimir::models::Matrix::pieceWiseMultiply(const mimir::models::Matrix &rhs)
{
    if (_rows != rhs._rows || _cols != rhs._cols) {
        throw std::logic_error("Can't piecewise multiply two matrices of unequal size.");
    }
    std::transform(_data.begin(), _data.end(), rhs._data.begin(), _data.begin(), [](auto left, auto right) {
        return left * right;
    });
    return *this;
}

Matrix &Matrix::operator /=(float value)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [value](float v) -> float { return v / value; });
    return *this;
}

Matrix Matrix::operator -(const Matrix &rhs) const
{
    return Matrix{*this} -= rhs;
}

Matrix &Matrix::operator -=(const Matrix &rhs)
{
    if (cols() != rhs.cols() && rhs.cols() == 1 && rows() == rhs.rows()) {
        return operator -=(rhs.column(0));
    }
    if (cols() != rhs.cols() || rows() != rhs.rows()) {
        throw std::logic_error("Can't piecewise substract matrices of unequal size.");
    }
    std::transform(_data.begin(), _data.end(), rhs._data.begin(), _data.begin(), [] (float left, float right) -> float { return left - right; });
    return *this;
}

Matrix &Matrix::operator -=(const std::vector<float> &rhs)
{
    if (rows() != rhs.size()) {
        throw std::logic_error("Size mismatch. Subtractor vector must have as many elements as the matrix has rows.");
    }
    auto pointer = _data.begin();
    for (auto value : rhs) {
        std::transform(pointer, pointer + static_cast<long>(_cols), pointer, [value](float current) -> float { return current - value; });
        std::advance(pointer, static_cast<long>(_cols));
    }
    return *this;
}

Matrix &Matrix::operator -=(const std::valarray<float> &sub)
{
    if (sub.size() != cols()) {
        throw std::logic_error("Rowise add needs an array as wide as the matrix.");
    }
    auto result = _data.begin();
    auto lastElement = std::end(sub);
    auto lastRow = _data.end();
    while (result != lastRow) {
        auto element = std::begin(sub);
        while (element != lastElement) {
            *result -= *element;
            ++element; ++result;
        }
    }
    return *this;
}

Matrix Matrix::operator +(const Matrix &rhs) const
{
    if (cols() != rhs.cols() || rows() != rhs.rows()) {
        throw std::logic_error("Can't piecewise add matrices of unequal size.");
    }
    return Matrix{*this} += rhs;
}

Matrix Matrix::operator +(float v) const
{
    return Matrix{*this} += v;
}

Matrix &Matrix::operator +=(float v)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [v](float oldVal) -> float { return oldVal + v; });
    return *this;
}

mimir::models::Matrix &mimir::models::Matrix::operator -=(float v)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [v](float oldVal) -> float { return oldVal - v; });
    return *this;
}

Matrix &Matrix::operator +=(const Matrix &rhs)
{
    if (cols() != rhs.cols() || rows() != rhs.rows()) {
        throw std::logic_error("Can't piecewise add matrices of unequal size.");
    }
    std::transform(_data.begin(), _data.end(), rhs._data.begin(), _data.begin(), [] (float left, float right) -> float { return left + right; });
    return *this;
}

Matrix &Matrix::operator +=(const std::valarray<float> &add)
{
    if (add.size() != cols()) {
        throw std::logic_error("Rowise add needs an array as wide as the matrix.");
    }
    auto result = _data.begin();
    auto lastElement = std::end(add);
    auto lastRow = _data.end();
    while (result != lastRow) {
        auto element = std::begin(add);
        while (element != lastElement) {
            *result += *element;
            ++element; ++result;
        }
    }
    return *this;
}

Matrix Matrix::operator +(const std::valarray<float> &add) const
{
    return Matrix{*this} += add;
}

size_t Matrix::cols() const
{
    return _cols;
}

size_t Matrix::rows() const
{
    return _rows;
}

float Matrix::value(size_t row, size_t column) const
{
    if (row >= _rows || column >= _cols) {
        throw std::out_of_range("Requested value out of bounds of matrix.");
    }
    return _data.at(row * _cols + column);
}

void Matrix::setValue(size_t row, size_t column, float value)
{
    if (row >= _rows || column >= _cols) {
        throw std::out_of_range("Requested value out of bounds of matrix.");
    }
    _data[row * _cols + column] = value;
}

void Matrix::fill(float value)
{
    _data.assign(_data.size(), value);
}

Matrix &Matrix::makeIdentity()
{
    auto entries = std::min(_cols, _rows);
    fill(0.);
    for (auto n = 0u; n < entries; ++n) {
        setValue(n, n, 1.);
    }
    return *this;
}

float Matrix::sum() const
{
    return std::accumulate(_data.begin(), _data.end(), 0.);
}

float Matrix::avg() const
{
    return sum() / static_cast<float>(_data.size());
}

float Matrix::mse() const
{
    auto avarage = avg();
    auto distance = std::accumulate(_data.begin(), _data.end(), 0., [avarage](auto init, auto value) -> auto {
        auto diff = value - avarage;
        return init + (diff*diff);
    });
    return distance;
}

bool Matrix::operator==(const Matrix &rhs) const
{
    if (rows() != rhs.rows()) {
        return false;
    }
    if (cols() != rhs.cols()) {
        return false;
    }
    return std::equal(_data.begin(), _data.end(), rhs._data.begin(),
            [](float left, float right) -> bool {
                return std::abs(left-right) < .00000000001;
            }
    );
}

bool Matrix::operator!=(const Matrix &rhs) const
{
    return !(*this == rhs);
}

Matrix& Matrix::transpose()
{
    if (_rows == 0) {
        return *this;
    }
    std::vector<float> tmp(_data.size());
    /*
     *  1  2  3     1  4
     *  4  5  6     2  5
     *              3  6
     *
     * 1  2  3  4  5  6    1  4  2  5  3  6
     *  swap cols < -- > rows
     *
     */
    size_t s = _data.size();
    for (size_t x = 0; x < s; ++x) {
        size_t i = x / _rows;
        size_t j = x % _rows;
        tmp[x] = _data[j * _cols + i];
    }
    std::swap(_rows, _cols);
    _data = tmp;
    return *this;
}

} // namespace models
} // namespace mimir
