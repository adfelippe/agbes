#ifndef _CARTRIDGE_INFO_
#define _CARTRIDGE_INFO_

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

std::string getCartridgeTypeString(uint8_t cartridge_type);
std::string getCartridgeLicenseeCodeNameString(uint8_t old_licensee_code, uint16_t new_licensee_code);
uint8_t getCartridgeLicenseeCodeFromValue(uint8_t old_licensee_code, uint16_t new_licensee_code);

#endif