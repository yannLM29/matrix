//
// Created by YannLM on 10/09/2025.
//
#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <iostream>

#include "Matrix.h"

TEST_CASE( "Create matrix by providing sizes", "[constructors]" ) {
    Matrix<int> m1(6,8);
    const size_t m1_size = 6*8;
    REQUIRE( m1.w() == 6 );
    REQUIRE( m1.h() == 8 );
    REQUIRE( m1.size() == m1_size);
    REQUIRE( m1.cend() - m1.cbegin() == m1_size);

    std::for_each(m1.cbegin(), m1.cend(), [](int v) {
        REQUIRE( v == 0 );
    });
}

TEST_CASE( "Create matrix by providing initializer list", "[constructors]" ) {
    Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });

    const size_t m1_size = 4*3;
    REQUIRE( m1.w() == 4 );
    REQUIRE( m1.h() == 3 );
    REQUIRE( m1.size() == m1_size);
    REQUIRE( m1.cend() - m1.cbegin() == m1_size);

    int i = 1;
    std::for_each(m1.cbegin(), m1.cend(), [&i](int v) {
        REQUIRE( v == i++ );
    });
}

TEST_CASE( "Create matrix by copying matrix", "[constructors]" ) {
    Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });

    auto m2(m1);
    REQUIRE( m1.w() == m2.w() );
    REQUIRE( m1.h() == m2.h() );
    REQUIRE( m1.size() == m2.size());
    for (int i = 0; i < m1.h(); ++i) {
        for (int j = 0; j < m1.w(); ++j) {
            REQUIRE(m1(j,i) == m2(j,i));
        }
    }

    auto m3 = m1;
    REQUIRE( m1.w() == m3.w() );
    REQUIRE( m1.h() == m3.h() );
    REQUIRE( m1.size() == m3.size());
    for (int i = 0; i < m1.h(); ++i) {
        for (int j = 0; j < m1.w(); ++j) {
            REQUIRE(m1(j,i) == m3(j,i));
        }
    }
}

TEST_CASE( "Create matrix by moving matrix", "[constructors]" ) {
    Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });

    auto m2(std::move(m1));
    const size_t m1_size = 4*3;
    REQUIRE( m2.w() == 4 );
    REQUIRE( m2.h() == 3 );
    REQUIRE( m2.size() == m1_size);
    REQUIRE( m2.cend() - m2.cbegin() == m1_size);

    int i = 1;
    std::for_each(m2.cbegin(), m2.cend(), [&i](int v) {
        REQUIRE( v == i++ );
    });


    auto m3 = std::move(m2);
    REQUIRE( m3.w() == 4 );
    REQUIRE( m3.h() == 3 );
    REQUIRE( m3.size() == m1_size);
    REQUIRE( m3.cend() - m3.cbegin() == m1_size);

    i = 1;
    std::for_each(m3.cbegin(), m3.cend(), [&i](int v) {
        REQUIRE( v == i++ );
    });
}

TEST_CASE( "Addition operator", "[operators]" ) {
    const Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });
    const auto m2 = m1 + m1;

    REQUIRE( m2.size() == 4*3);
    int i = 1;
    std::for_each(m2.cbegin(), m2.cend(), [&i](int v) {
        REQUIRE( v == i * 2 );
        i++;
    });
}

TEST_CASE( "Substraction operator", "[operators]" ) {
    const Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });
    const auto m2 = m1 - m1;

    REQUIRE( m2.size() == 4*3);
    std::for_each(m2.cbegin(), m2.cend(), [](int v) {
        REQUIRE( v == 0 );
    });
}

TEST_CASE( "Equality operator", "[operators]" ) {
    const Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });
    REQUIRE( m1 == m1);
}

TEST_CASE( "Multiply operator", "[operators]" ) {
    const Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });
    const Matrix<int> m2({
        {1,2,3},
        {4,5,6},
        {7,8,9},
        {10,11,12}
    });
    const Matrix<int> m3 = m1 * m2;
    const Matrix<int> expected = {
        {70, 80, 90},
        {158, 184, 210},
        {246, 288, 330}
    };
    REQUIRE(m3 == expected);
}

TEST_CASE( "Transpose Matrix", "[functions]" ) {
    const Matrix<int> m1({
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    });

    const Matrix<int> expected({
        {1,5,9},
        {2,6,10},
        {3,7,11},
        {4,8,12}
    });

    REQUIRE(transposeMatrix(m1) == expected);
}

TEST_CASE( "Get Matrix determinant", "[functions]" ) {
    const Matrix<int> m1({
        {1,2,3},
        {5,6,7},
        {9,10,11}
    });
    REQUIRE(determinantMatrix(m1) == 0);

    const Matrix<int> m2({
        {7,61},
        {80,23},
    });
    REQUIRE(determinantMatrix(m2) == -4719);
}