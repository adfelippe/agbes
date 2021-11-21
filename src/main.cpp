#include <cstdio>
#include <iostream>

#include "cartridge.h"

int main(int argc, char* argv[]) {
    Cartridge rom(argv[1]);

    bool result = rom.loadCartridge();

    std::cout << "Result: " << result << std::endl;
}
