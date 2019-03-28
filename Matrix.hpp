#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <random>

#include "ModInt.hpp"

template<size_t N, size_t M, long P>
class Matrix {
private:
    std::unique_ptr<std::unique_ptr<ModInt<P>[]>[]> data;

    Matrix() = default;

public:
    explicit Matrix(int seed)
    {
        data = std::make_unique<std::unique_ptr<ModInt<P>[]>[]>(N);

        std::minstd_rand rand;
        rand.seed(seed);
        std::uniform_int_distribution<long> dist(0, P - 1);

        for (size_t i = 0; i < N; i++) {
            data[i] = std::make_unique<ModInt<P>[]>(M);
            for (size_t j = 0; j < M; j++) {
                data[i][j] = ModInt<P>(dist(rand));
            }
        }
    }

    bool solve()
    {
        // For every element A[i, i] (the diagonal)
        for (size_t i = 0; i < N; i++) {
            // If A[i][i] = 0, rows need to be swapped to get information about variable i
            if (data[i][i] == 0) {
                bool swapped = false;
                for (size_t s = i + 1; s < N; s++) {
                    if (data[s][i] != 0) {
                        std::swap(data[i], data[s]);
                        swapped = true;
                    }
                }
                if (!swapped) {
                    std::cout << "Impossible matrix given!" << std::endl;
                    return false;
                }
            }

            // For every row j > i
            for (size_t j = i + 1; j < N; j++) {
                // Find the factor by which row i must be multiplied, such that A[i, i] = A[i, j]
                auto factor = data[j][i] / data[i][i];
                // Then subtract the row i * factor from row j
                for (size_t k = i; k < M; k++) {
                    data[j][k] -= factor * data[i][k];
                }
            }
        }

        //The last row is solved
        data[N - 1][M - 1] = data[N - 1][M - 1] / data[N - 1][M - 2];

        //Starting from the bottom, substitute values back up the rows
        for (long i = N - 2; i >= 0; i--) {
            //A[i, i] is being solved. Column j from [i+1, M - 1) is the range of known values
            for (size_t j = (size_t)i + 1; j < M - 1; j++) {
                data[i][M - 1] -= data[i][j] * variable(j);
            }
            data[i][M - 1] /= data[i][i];
        }

        return true;
    }

    bool checkSolution(const Matrix& original)
    {
        /*
         * Compute each row by multiplying the coefficients from the original matrix
         * by the value of each variable, and check the solution
         */
        bool solved = true;

        for (size_t i = 0; i < N; i++) {
            ModInt<P> sum(0);
            for (size_t j = 0; j < M - 1; j++) {
                sum += original.data[i][j] * variable(j);
            }
            if (sum != original.variable(i)) {
                std::cout << "Row " << i << " adds to " << sum << ", expected " << original.variable(i) << std::endl;
                solved = false;
            }
        }

        return solved;
    }

    inline ModInt<P> variable(size_t i) const
    {
        return data[i][M - 1];
    }

    Matrix copy() const
    {
        Matrix ret;
        ret.data = std::make_unique<std::unique_ptr<ModInt<P>[]>[]>(N);

        for (size_t i = 0; i < N; i++) {
            ret.data[i] = std::make_unique<ModInt<P>[]>(M);
            for (size_t j = 0; j < M; j++) {
                ret.data[i][j] = this->data[i][j];
            }
        }

        return ret;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const Matrix<N, M, P> &matrix)
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
