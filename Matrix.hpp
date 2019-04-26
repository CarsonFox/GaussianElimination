#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <random>

#include "ModInt.hpp"

template<size_t P>
class Matrix {
private:
    const size_t N, M;
    std::unique_ptr<std::unique_ptr<ModInt<P>[]>[]> data;

public:
    explicit Matrix(size_t size) : N(size), M(size + 1) {
        if (size == 0) {
            std::cerr << "Cannot create and array with zero size" << std::endl;
            std::exit(1);
        }

        data = std::make_unique<std::unique_ptr<ModInt<P>[]>[]>(N);

        std::random_device dev;
        std::default_random_engine rand(dev());
        std::uniform_int_distribution<long> dist(0, P - 1);

        for (size_t i = 0; i < N; i++) {
            data[i] = std::make_unique<ModInt<P>[]>(M);
            for (size_t j = 0; j < M; j++) {
                //Disallow 0s on the diagonals to avoid an inconsistent system
                do {
                    data[i][j] = ModInt<P>(dist(rand));
                } while (j == i && data[i][i] == 0);
            }
        }
    }

    Matrix(const Matrix &other) : N(other.N), M(other.M) {
        data = std::make_unique<std::unique_ptr<ModInt<P>[]>[]>(N);

        for (size_t i = 0; i < N; i++) {
            data[i] = std::make_unique<ModInt<P>[]>(M);
            for (size_t j = 0; j < M; j++) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    bool solve() {
        // For every element A[i, i] (the diagonal)
        for (size_t i = 0; i < N; i++) {
            // If A[i][i] = 0, rows need to be swapped to get information about variable i
            if (data[i][i] == 0) {
                bool swapped = false;

                for (size_t s = i + 1; s < N && !swapped; s++) {
                    if (data[s][i] != 0) {
                        std::swap(data[i], data[s]);
                        swapped = true;
                    }
                }

                //System is inconsistent
                if (!swapped)
                    return false;
            }

            const auto RANGE = N - i;
            const auto MID = RANGE / 2 + 1;

#pragma omp parallel for schedule(static)
            for (size_t j = 1; j < MID; j++) {
                subtractRow(i, i + j);
                if (N - j > i + j) {
                    subtractRow(i, N - j);
                }
            }
        }

        //The last row is solved
        data[N - 1][M - 1] = data[N - 1][M - 1] / data[N - 1][M - 2];

        //Starting from the bottom, substitute values back up the rows
        for (long i = N - 2; i >= 0; i--) {
            //A[i, i] is being solved. Column j from [i+1, M - 1) is the range of known values
            for (size_t j = (size_t) i + 1; j < M - 1; j++) {
                data[i][M - 1] -= data[i][j] * variable(j);
            }
            data[i][M - 1] /= data[i][i];
        }

        return true;
    }

    inline void subtractRow(size_t i, size_t j) {
        // Find the factor by which row i must be multiplied, such that A[i, i] = A[i, j]
        auto factor = data[j][i] / data[i][i];
        // Then subtract the row i * factor from row j
        for (size_t k = i; k < M; k++) {
            data[j][k] -= factor * data[i][k];
        }
    }

    bool checkSolution(const Matrix &original) {
        /*
         * Compute each row by multiplying the coefficients from the original matrix
         * by the value of each variable, and check the solution
         */
        bool solved = true;

#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < N; i++) {
            ModInt<P> sum(0);
            for (size_t j = 0; j < M - 1; j++) {
                sum += original.data[i][j] * variable(j);
            }
            if (sum != original.variable(i)) {
                solved = false;
            }
        }

        return solved;
    }

    inline ModInt<P> variable(size_t i) const {
        return data[i][M - 1];
    }

    friend std::ostream &operator<<(std::ostream &os,
                                    const Matrix<P> &matrix) {
        for (size_t i = 0; i < matrix.N; i++) {
            for (size_t j = 0; j < matrix.M; j++) {
                os << matrix.data[i][j] << "   ";
            }
            os << std::endl;
        }
        return os;
    }
};
