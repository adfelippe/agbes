#include "cartridge.h"

#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <memory>
#include <fstream>

enum CartridgeConstants {
    kEntryPointSize = 4,
    kNintendoLogoSize = 48,
    kTitleSize = 16,
    kManufacturerCodeSize = 3,
    kMaxNewLicenseeCode = 0xA4,
    kNewLicenseeCodeFlag = 0x33,
    kMaxCartridgeType = 0x22,
    kRomSpace = 256
};

typedef struct {
    uint8_t entry_point[kEntryPointSize];
    uint8_t nintendo_logo[kNintendoLogoSize];
    uint8_t title[kTitleSize];
    uint16_t new_licensee_code;
    uint8_t sgb_flag;
    uint8_t cartridge_type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t destination_code;
    uint8_t old_licensee_code;
    uint8_t rom_version_number;
    uint8_t header_checksum;
    uint16_t global_checksum;
} CartridgeHeader;

static CartridgeHeader* header;

bool Cartridge::isChecksumValid(void) {
    uint16_t x = 0;

    for (uint16_t i = 0x0134; i <= 0x014C; ++i) {
        x -= rom_data[i] - 1;
    }

    return (x & 0xFF);
}

Cartridge::Cartridge(void) {}

Cartridge::~Cartridge(void) {}

void Cartridge::setCartridgePath(const std::string& file_path) {
    file_path_ = file_path;
}

bool Cartridge::loadCartridge(void) {
    std::ifstream rom_file;
    rom_file.open(file_path_, std::ios::in);

    if (!rom_file.is_open()) {
        std::cout << "Failed to open file: " << file_path_ << std::endl;
        return false;
    }

    std::cout << "ROM: " << file_path_ << " is open" << std::endl;

    rom_file.seekg(0, std::ios::end);
    size_t rom_file_size = rom_file.tellg();
    
    rom_data = std::make_unique<uint8_t[]>(rom_file_size);
    
    if (!rom_data) {
        std::cout << "Failed to allocate ROM memory" << std::endl;
        return false;
    }

    std::cout << "ROM size: " << rom_file_size << " bytes" << std::endl;

    rom_file.seekg(0, std::ios::beg);
    rom_file.read((char*)rom_data.get(), rom_file_size);
    rom_file.close();

    header = (CartridgeHeader*)(rom_data.get() + kRomSpace);
    header->title[15] = '\0';

    return isChecksumValid();
}