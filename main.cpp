#include <chrono>
#include <string>
#include <iostream>

#include "Matrix.hpp"

int main(int argc, char **argv) {
    //4300 takes about a minute on my laptop
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
        auto original(mat);

        auto start = std::chrono::system_clock::now();
        bool solved = mat.solve();
        auto end = std::chrono::system_clock::now();

        if (solved) {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << duration.count() << std::endl;
            std::cout << std::boolalpha << mat.checkSolution(original) << std::endl;
            break;
        }
    }
}
