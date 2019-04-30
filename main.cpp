#include <chrono>
#include <iostream>
#include <omp.h>
#include <string>

#include "Matrix.hpp"

int main(int argc, char **argv) {
    size_t size;
    int threads, seed;

    if (argc == 4) {
        try {
            size = (size_t) std::stoi(std::string(argv[1]));
            threads = std::stoi(std::string(argv[2]));
            seed = std::stoi(std::string(argv[3]));
        } catch (std::exception &e) {
            std::cerr << "Error parsing argument " << e.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Not enough arguments: supply size, number of threads, and seed for rng" << std::endl;
        return 1;
    }

    omp_set_num_threads(threads);

    //Keep increasing the seed until a solvable matrix is generated
    for (;;) {
        Matrix mat(size, seed);
        const Matrix original(mat);

        auto start = std::chrono::system_clock::now();
        bool solved = mat.solve();
        auto end = std::chrono::system_clock::now();

        if (solved) {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << duration.count() << std::endl;
            return mat.checkSolution(original) ? 0 : 1;
        } else {
            seed++;
        }
    }
}
