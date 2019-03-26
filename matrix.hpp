#pragma once

#include <array>
#include <iostream>
#include <random>

#include "modular-arithmetic.hpp"

/*
 * Simple matrix class. Uses modular arithmetic.
 */

template <size_t N, size_t M>
class Matrix {
private:
    std::array<std::array<long, M>, N> data;

public:
    Matrix()
    {
        std::random_device rand;
        std::uniform_int_distribution<long> dist(0, P - 1);

        for (size_t i = 0; i < data.size(); i++)
            for (size_t j = 0; j < data[0].size(); j++)
                data[i][j] = dist(rand);
    }

    Matrix operator+(const Matrix<N, M>& other)
    {
        Matrix<N, M> result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < M; j++) {
                result.data[i][j] = mAdd(this->data[i][j], other.data[i][j]);
            }
        }
        return result;
    }

    // Matrix multiply:
    // result[i][j] = sum from k = 0 -> (M-1) of a[i][k] * b[k][j]
    Matrix<N, P> operator*(const Matrix<M, P>& other)
    {
        Matrix<N, P> result;
        for (auto i = 0; i < N; i++) {
            for (auto j = 0; j < P; j++) {
                long sum = 0;
                for (auto k = 0; k < M; k++)
                    sum += mMul(this->data[i][k], other.data[k][j]);
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os,
        const Matrix<N, M>& matrix)
    {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < M; j++) {
                os << matrix.data[i][j] << "   ";
            }
            os << std::endl;
        }
        return os;
    }

    static_assert(N > 0 && M > 0,
        "Cannot make a matrix/vector with zero rows or columns");
};
