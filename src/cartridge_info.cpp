#include "cartridge_info.h"

#include <cstring>
#include <map>

enum CartridgeInfoConstants {
    kMaxNewLicenseeCode = 0xA4,
    kNewLicenseeCodeFlag = 0x33,
};

static const std::vector<std::string> kCartridgeTypes {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

std::map<uint8_t, std::string> licensee_code_names = {
    {0x00, "None"},
    {0x01, "Nintendo R&D1"},
    {0x08, "Capcom"},
    {0x13, "Electronic Arts"},
    {0x18, "Hudson Soft"},
    {0x19, "b-ai"},
    {0x20, "kss"},
    {0x22, "pow"},
    {0x24, "PCM Complete"},
    {0x25, "san-x"},
    {0x28, "Kemco Japan"},
    {0x29, "seta"},
    {0x30, "Viacom"},
    {0x31, "Nintendo"},
    {0x32, "Bandai"},
    {0x33, "Ocean/Acclaim"},
    {0x34, "Konami"},
    {0x35, "Hector"},
    {0x37, "Taito"},
    {0x38, "Hudson"},
    {0x39, "Banpresto"},
    {0x41, "Ubi Soft"},
    {0x42, "Atlus"},
    {0x44, "Malibu"},
    {0x45, "Electronic Arts - New Code"},
    {0x46, "angel"},
    {0x47, "Bullet-Proof"},
    {0x49, "irem"},
    {0x50, "Absolute"},
    {0x51, "Acclaim"},
    {0x52, "Activision"},
    {0x53, "American sammy"},
    {0x54, "Konami"},
    {0x55, "Hi tech entertainment"},
    {0x56, "LJN"},
    {0x57, "Matchbox"},
    {0x58, "Mattel"},
    {0x59, "Milton Bradley"},
    {0x60, "Titus"},
    {0x61, "Virgin"},
    {0x64, "LucasArts"},
    {0x67, "Ocean"},
    {0x69, "Electronic Arts"},
    {0x70, "Infogrames"},
    {0x71, "Interplay"},
    {0x72, "Broderbund"},
    {0x73, "sculptured"},
    {0x75, "sci"},
    {0x78, "THQ"},
    {0x79, "Accolade"},
    {0x80, "misawa"},
    {0x83, "lozc"},
    {0x86, "Tokuma Shoten Intermedia"},
    {0x87, "Tsukuda Original"},
    {0x91, "Chunsoft"},
    {0x92, "Video system"},
    {0x93, "Ocean/Acclaim"},
    {0x95, "Varie"},
    {0x96, "Yonezawa/s-pal"},
    {0x97, "Kaneko"},
    {0x99, "Pack in soft"},
    {0xA4, "Konami (Yu-Gi-Oh!)"}
};

std::string getCartridgeTypeString(uint8_t cartridge_type) {
    if (cartridge_type > kCartridgeTypes.size()) {
        return "Invalid cartridge type";
    }

    return kCartridgeTypes[cartridge_type];
}

std::string getCartridgeLicenseeCodeNameString(uint8_t old_licensee_code, uint16_t new_licensee_code) {
    uint8_t converted_code = getCartridgeLicenseeCodeFromValue(old_licensee_code, new_licensee_code);
    return licensee_code_names.find(converted_code)->second;
}

uint8_t getCartridgeLicenseeCodeFromValue(uint8_t old_licensee_code, uint16_t new_licensee_code) {
    if (old_licensee_code != kNewLicenseeCodeFlag) {
        return old_licensee_code;
    }

    uint16_t licensee_code = new_licensee_code;

    uint8_t lower_byte = (uint8_t)(licensee_code & 0x00FF);
    uint8_t higher_byte = (uint8_t)((licensee_code >> 8) & 0x00FF);

    const char kLicenseeCodeString[3] = {(char)lower_byte, (char)higher_byte, '\0'};
    uint8_t converted_code = (uint8_t)strtoul(kLicenseeCodeString, NULL, 10);

    return converted_code;
}