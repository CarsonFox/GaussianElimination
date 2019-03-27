#include <climits>
#include <iostream>

#include "matrix.hpp"

#ifndef SIZE
#define SIZE 1000
#endif

void system3();

int main() {
    for (int i = 0; i < INT_MAX; i++) {
        Matrix<SIZE, SIZE + 1> mat(i);
        const auto original = mat.copy();

        std::cout << "Matrix " << i << ": ";

        if (mat.solve()) {
            if (mat.checkSolution(original)) {
                std::cout << "Solved" << std::endl;
            } else {
                std::cout << "Incorrect solution for seed " << i << '!' << std::endl
                          << std::endl;
                std::cout << mat << std::endl
                          << original << std::endl;
                break;
            }
        }
    }
}
