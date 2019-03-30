#include <climits>
#include <iostream>

#include "Matrix.hpp"

#ifndef SIZE
#define SIZE 300
#endif

int main() {
    for (int i = 0; i < 1000; i++) {
        Matrix<11> mat(i, 40);
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
