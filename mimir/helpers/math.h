#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <functional>
#include <numeric>
#include <valarray>

namespace mimir {
namespace helpers {
namespace math {

template <class T, typename Container>
T dotProduct(Container const &left, Container const &right)
{
    typename Container::value_type result = std::inner_product(std::begin(left), std::end(left), std::begin(right), 0);
    return result;
}

template <class T>
typename std::enable_if<std::is_same<T, typename std::valarray<typename T::value_type>>::value, T>::type
 prepareResult(T const &_x)
{
    return T(typename T::value_type(0), _x.size());
}

template<class T>
typename std::enable_if<!std::is_same<T, typename std::valarray<typename T::value_type>>::value, T>::type
 prepareResult(T const &_x)
{
    return T(_x.size(), typename T::value_type(0));
}


template <typename Container>
Container crossProduct(Container const &left, Container const &right)
{
    size_t last = left.size();
    auto result = prepareResult<Container>(right);
    size_t idx1;
    size_t idx2;
    for (size_t i = 0; i < last; ++i) {
        typename Container::value_type v = 0;
        size_t c = 1;
        while (c < last) {
            idx1 = c++ + i;
            idx2 = c++ + i;
            if (idx1 >= last) idx1 -= last;
            if (idx2 >= last) idx2 -= last;
            v += left[idx1] * right[idx2] - left[idx2]*right[idx1];
        }
        result[i] = v;
    }
    return result;
}

template <typename Container>
Container matrixStyleMult(Container const &left, Container const &right)
{
    Container result = prepareResult(left);
    for (size_t i = 0; i < left.size(); ++i) {
        result += right * left[i];
    }
    return result;
}

template<typename T>
std::vector<std::valarray<T>> transposeMatrix(std::vector<std::valarray<T>> const &m)
{
    if (m.size() == 0) {
        return std::vector<std::valarray<T>>();
    }
    size_t columns = m.at(0).size();
    size_t rows = m.size();
    std::vector<std::valarray<T>> result(columns, std::valarray<T>(rows));
    size_t row, column;
    for (row = 0; row < rows; ++row) {
        for (column = 0; column < columns; ++column) {
            result[column][row] = m[row][column];
        }
    }
    return result;
}

}
}
}

template <class T>
std::vector<T> operator -(std::vector<T> const&left, std::vector<T> const &right)
{
    std::vector<T> result;
    result.reserve(left.size());
    auto l = left.begin();
    auto r = right.begin();
    while (l != left.end()) {
        result.push_back(*l - *r);
        ++l; ++r;
    }
    return result;
}

template <class T>
std::vector<T> operator *(std::vector<T> const &left, T const &right)
{
    std::vector<T> result;
    result = left;
    std::transform(left.begin(), left.end(), result.begin(), std::bind(std::multiplies<T>(), std::placeholders::_1, right));
    return result;
}

#endif // MATH_H
