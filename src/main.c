#include <stdio.h>

#include "cartridge.h"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        loadCartridge(argv[1]);
        return 0;
    }

    printf("Command failed!\n");
    return -1;
}
