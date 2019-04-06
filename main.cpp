#include <chrono>
#include <iostream>
#include <omp.h>
#include <string>

#include "Matrix.hpp"

int main(int argc, char **argv) {
    size_t size = 4300;

    if (argc == 2) {
        try {
            size = (size_t) std::stoi(std::string(argv[1]));
        } catch (std::exception &e) {
            std::cerr << "Error parsing argument " << argv[1] << ": " << e.what() << std::endl;
            return 1;
        }
    }

    for (;;) {
        Matrix<982451653> mat(size);
        const auto original(mat);

        auto start = std::chrono::system_clock::now();
        bool solved = mat.solve();
        auto end = std::chrono::system_clock::now();

        if (solved) {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << duration.count() << std::endl;
            return mat.checkSolution(original) ? 0 : 1;
        }
    }
}
