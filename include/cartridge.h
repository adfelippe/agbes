#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include <string>
#include <cstdint>
#include <memory>

bool loadCartridge(const char* cartridge);

class Cartridge { 
private:
    std::string file_path_;
    std::unique_ptr<uint8_t[]> rom_data;
    bool isChecksumValid(void);

public:
    Cartridge(const std::string& file_path) : file_path_(file_path) {}
    Cartridge();
    ~Cartridge();
    bool loadCartridge();
    void setCartridgePath(const std::string& file_path);
};

#endif /*_CARTRIDGE_H_*/