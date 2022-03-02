#include <cstdio>
#include <iostream>

#include "cartridge.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Wrong number of arguments: " + std::to_string(argc) << std::endl;
        std::cout << "Usage: " << std::endl;
        std::cout << "agbes path/to/rom/rom.gb " << std::endl;
        exit(EXIT_FAILURE);
    }

    Cartridge rom(argv[1]);

    bool result = rom.loadCartridge();

    std::cout << "Result: " << result << std::endl;

    exit(EXIT_SUCCESS);
}
