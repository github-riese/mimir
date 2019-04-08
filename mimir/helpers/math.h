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

template <typename T>
T sum(std::vector<T> const &v) noexcept
{
    return std::accumulate(v.begin(), v.end(), 0.);
}

}
}
} // end namespace mimir

template <class T>
std::vector<T> operator -(std::vector<T> const&left, std::vector<T> const &right)
{
    std::vector<T> result(left.size());
    std::transform(left.begin(), left.end(), right.begin(), result.begin(), [](auto l, auto r) -> auto {return l-r;});
    return  result;
}

template <class T>
std::vector<T>& operator -=(std::vector<T> &left, std::vector<T> const &right)
{
    std::transform(left.begin(), left.end(), right.begin(), left.begin(), [](auto l, auto r) -> auto {return l-r;});
    return  left;
}

template <class T>
std::vector<T> operator -(std::vector<T> const&left, T right)
{
    std::vector<T> result(left.size());
    std::transform(left.begin(), left.end(), result.begin(), [right](auto l) noexcept -> T { return l - right; });
    return result;
}

template <class T>
std::vector<T> &operator -=(std::vector<T> &left, T right)
{
    std::transform(left.begin(), left.end(), left.begin(), [right](auto l) noexcept -> T { return l - right; });
    return left;
}

template<typename T>
std::vector<T> operator +(std::vector<T> const &left, std::vector<T> const &right)
{
    std::vector<T> result(left.size());
    std::transform(left.begin(), left.end(), right.begin(), result.begin(), [](auto l, auto r) -> auto {return l+r;});
    return  result;
}

template <class T>
std::vector<T>& operator +=(std::vector<T> &left, std::vector<T> const &right)
{
    std::transform(left.begin(), left.end(), right.begin(), left.begin(), [](auto l, auto r) -> auto {return l+r;});
    return  left;
}

template <class T>
std::vector<T> operator +(std::vector<T> const&left, T right)
{
    std::vector<T> result(left.size());
    std::transform(left.begin(), left.end(), result.begin(), [right](auto l) noexcept -> T { return l + right; });
    return result;
}

template <class T>
std::vector<T> &operator +=(std::vector<T> &left, T right)
{
    std::transform(left.begin(), left.end(), left.begin(), [right](auto l) noexcept -> T { return l + right; });
    return left;
}

template<typename T>
std::vector<T> operator*(T const &left, std::vector<T> const &right)
{
    std::vector<T> result(right);
    std::transform(result.begin(), result.end(), result.begin(), [left](T const &v) -> T { return  left * v;});
    return result;
}

template<typename T>
std::vector<T> operator*(std::vector<T> const &left, T const &right)
{
    return right * left;
}

template<typename T>
std::vector<T> operator*(std::vector<T> const &left, std::vector<T> const &right)
{
    std::vector<T> result(right);
    std::transform(left.begin(), left.end(), right.begin(), result.begin(), [](T const &l, T const &r) -> T { return  l * r;});
    return result;
}

template<typename T>
std::vector<T>& operator*=(std::vector<T> &left, std::vector<T> const &right)
{
    std::transform(left.begin(), left.end(), right.begin(), left.begin(), [](T const &l, T const &r) -> T { return  l * r;});
    return left;
}

template<typename T>
std::vector<T> &operator*=(std::vector<T> &left, T const &right)
{
    std::transform(left.begin(), left.end(), left.begin(), [&right](T const &v) noexcept -> T { return v * right; });
    return left;
}

template<typename T>
std::vector<T> operator*(std::vector<T> const &left, std::valarray<T> const &right)
{
    std::vector<T> result(right.size());
    std::transform(left.begin(), left.end(), std::begin(right), result.begin(), [](T const &l, T const &r) -> T { return  l * r;});
    return result;
}

template<typename T>
std::vector<T> operator*(T const &left, std::valarray<T> const &right)
{
    std::vector<T> result(right.size());
    std::transform(std::begin(right), std::end(right), result.begin(), [left](T const &v) -> T { return  left * v;});
    return result;
}

template <typename T>
std::vector<T> operator-(std::vector<T> const &in)
{
    std::vector<T> result(in.size());
    std::transform(in.begin(), in.end(), result.begin(), [](T const &v) { return  -v; });
    return result;
}

/**
 * The dot product line vector times row vector
 */
template <typename T>
T operator *(std::valarray<T> const & left, std::vector<T> const &right)
{
    if (left.size() != right.size()) {
        throw std::logic_error("Need same size of column vector and row vector for dot product.");
    }
    T result = 0;
    auto lfirst = std::begin(left);
    auto llast = std::end(left);
    auto rfirst = right.begin();
    for (; lfirst != llast; ++lfirst, ++rfirst) {
        result += *lfirst * *rfirst;
    }

    return result;
}

#endif // MATH_H
