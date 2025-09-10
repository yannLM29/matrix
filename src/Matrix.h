#pragma once

#include <vector>
#include <stdexcept>
#include <functional>

template <typename T>
class Matrix
{
private:
    std::vector<T> mData;
    size_t mWidth, mHeight;

public:
    // Constructors
    Matrix(size_t width, size_t height) noexcept;
    Matrix(size_t width, size_t height, const T& default_value) noexcept;
    Matrix(std::initializer_list<std::initializer_list<T>> values) noexcept;
    // copy
    Matrix(const Matrix<T>& from) noexcept;
    Matrix& operator=(const Matrix<T>& from) noexcept;
    // move
    Matrix(Matrix<T>&& from) noexcept;
    Matrix& operator=(Matrix<T>&& from) noexcept;

    // Matrix(Image from)
    
    // Get
    size_t w() const noexcept {
        return mWidth;
    }

    size_t h() const noexcept {
        return mHeight;
    }

    size_t size() const noexcept {
        return mData.size();
    }
    
    // Operators
    T& operator()(size_t x, size_t y) {
        if (x < 0 || x >= mWidth) {
            throw std::runtime_error("Error: In Matrix(x,y): x is out of range");
        }
        if (y < 0 || y >= mHeight) {
            throw std::runtime_error("Error: In Matrix(x,y): y is out of range");
        }
        return mData[y * mWidth + x];
    }

    const T& operator()(size_t x, size_t y) const {
        if (x < 0 || x >= mWidth) {
            throw std::runtime_error("Error: In Matrix(x,y): x is out of range");
        }
        if (y < 0 || y >= mHeight) {
            throw std::runtime_error("Error: In Matrix(x,y): y is out of range");
        }
        return mData[y * mWidth + x];
    }

    // Iterators
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin() { return mData.begin(); }
    iterator end() { return mData.end(); }
    const_iterator cbegin() const { return mData.cbegin(); }
    const_iterator cend() const { return mData.cend(); }
};

template<typename T>
Matrix<T>::Matrix(size_t width, size_t height) noexcept 
: mData(width * height, static_cast<T>(0))
, mWidth(width)
, mHeight(height)
{
    
}

template<typename T>
Matrix<T>::Matrix(size_t width, size_t height, const T& default_value) noexcept
    : mData(width * height, default_value)
    , mWidth(width)
    , mHeight(height)
{

}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> values) noexcept
: mHeight(values.size()) 
{
    if (mHeight == 0) { 
        return; 
    }
    mWidth = (*values.begin()).size();

    for (const std::initializer_list<T>& row : values) {
        if (row.size() != mWidth) {
            throw std::invalid_argument("Error: In Matrix constructor: rows must all have the same size");
        }
        for (const T& v : row) {
            mData.emplace_back(v);
        }
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& from) noexcept 
: mData(from.mData)
, mWidth(from.mWidth)
, mHeight(from.mHeight) 
{
    
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& from) noexcept {
    mData = from.mData;
    mWidth = from.mWidth;
    mHeight = from.mHeight;

    return *this;
}

template<typename T>
Matrix<T>::Matrix(Matrix<T>&& from) noexcept 
: mData(std::move(from.mData))
, mWidth(from.mWidth)
, mHeight(from.mHeight)
{

}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& from) noexcept {
    mData = std::move(from.mData);
    mWidth = from.mWidth;
    mHeight = from.mHeight;

    return *this;
}


template<typename T>
Matrix<T> transposeMatrix(const Matrix<T>& matrix) {
    Matrix<T> out(matrix.h(), matrix.w());

    for (int i = 0; i < matrix.h(); i++) {
        for (int j = 0; j < matrix.w(); j++) {
            out(i, j) = matrix(j, i);
        }
    }

    return out;
}

template<typename T>
T determinantMatrix(const Matrix<T>& matrix) {

    if (matrix.w() != matrix.h()) {
        throw std::runtime_error("Error: In determinantMatrix: Matrix must be squared");
    }

    // lambda return det of 2 dimension matrix
    auto det_dim_2 = [](const Matrix<T>& m) {
        return m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1);
    };

    // if dim 2 it's easy
    if (matrix.w() == 2 && matrix.h() == 2) {
        return det_dim_2(matrix);
    }

    // lambda creating a new matrix with a row and a column removed
    auto remove_row_and_column = [&matrix](size_t row, size_t column) {
        Matrix<T> out(matrix.w() - 1, matrix.h() - 1);
        for (int i = 0; i < matrix.h(); i++) {
            for (int j = 0; j < matrix.w(); j++) {
                if (i != row && j != column) {
                    out(j > column ? j - 1 : j, i > row ? i - 1 : i) = matrix(j, i);
                }
            }
        }
        return out;
    };

    std::function<T(const Matrix<T>&)> compute_det;
    compute_det = [&](const Matrix<T>& m) {
        if (m.w() > 2) {
            T det = static_cast<T>(0);
            for (int j = 0; j < m.w(); j++) {
                auto smaller_mat = remove_row_and_column(0, j);
                auto sign = (0 + j) % 2 == 0 ? static_cast<T>(1) : static_cast<T>(-1);
                det += sign * m(j,0) * compute_det(smaller_mat);
            }
            
            return det;
        }
        else {
            return det_dim_2(m);
        }
    };

    // Apply cofactor method for every value of the matrix
    return compute_det(matrix);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat) {
    for (int y = 0; y < mat.h(); y++) {
        os << "[";
        for (int x = 0; x < mat.w(); x++) {
            os << mat(x, y);
            if (x != mat.w() - 1) { os << ", "; }
        }
        os << "]\n";
    }
    return os;
}

template<typename T>
bool operator==(Matrix<T>& a, const Matrix<T>& b) {
    if (a.w() != b.w() || a.h() != b.h()) {
        return false;
    }

    for (int i = 0; i < a.h(); i++) {
        for (int j = 0; j < a.w(); j++) {
            if (a(j, i) = a(j, i) != b(j, i)) {
                return false;
            }
        }
    }

    return true;
}

template<typename T>
Matrix<T>& operator+=(Matrix<T>& a, const Matrix<T>& b) {
    if (a.w() != b.w() || a.h() != b.h()) {
        throw std::runtime_error("Error: In Matrix addition: Trying to add different size of matrix together");
    }

    for (int i = 0; i < a.h(); i++) {
        for (int j = 0; j < a.w(); j++) {
            a(j, i) = a(j, i) + b(j, i);
        }
    }

    return a;
}

template<typename T>
Matrix<T> operator+(Matrix<T> a, const Matrix<T>& b) {
    a += b;
    return a;
}

template<typename T>
Matrix<T>& operator-=(Matrix<T> a, const Matrix<T>& b) {
    if (a.w() != b.w() || a.h() != b.h()) {
        throw std::runtime_error("Error: In Matrix addition: Trying to add different size of matrix together");
    }

    for (int i = 0; i < a.h(); i++) {
        for (int j = 0; j < a.w(); j++) {
            a(j, i) = a(j, i) - b(j, i);
        }
    }

    return a;
}

template<typename T>
Matrix<T> operator-(Matrix<T> a, const Matrix<T>& b) {
    a -= b;
    return a;
}

template<typename T>
Matrix<T> operator*(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.w() != b.h()) {
        throw std::runtime_error("Error: In Matrix multiplication: Those matrix can't be multiplied because of their sizes");
    }

    Matrix<T> out(b.w(), a.h());
    for (int y = 0; y < out.h(); y++) {
        for (int x = 0; x < out.w(); x++) {
            for (int i = 0; i < a.w(); i++) {
                out(x,y) += a(i,y) * b(x,i);
            }
        }
    }
    return out;
}