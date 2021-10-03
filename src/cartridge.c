#include "cartridge.h"

#include <stdlib.h>
#include <stdio.h>

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

static const char *kCartridgeTypes[] = {
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

static const char *kLicenseeCodeNames[kMaxNewLicenseeCode + 1] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"
};

static size_t rom_size;
static uint8_t* rom_data;
static CartridgeHeader* header;

// Private prototypes
static uint8_t getCartridgeLicenseeCodeFromValue(void);
static const char* getCartridgeLicenseeName(void);
static const char* getCartridgeType(void);

static uint8_t getCartridgeLicenseeCodeFromValue(void) {
    if (header->old_licensee_code != kNewLicenseeCodeFlag) {
        return header->old_licensee_code;
    }

    uint16_t licensee_code = header->new_licensee_code;

    uint8_t lower_byte = (uint8_t)(licensee_code & 0x00FF);
    uint8_t higher_byte = (uint8_t)((licensee_code >> 8) & 0x00FF);

    const char kLicenseeCodeString[3] = {(char)lower_byte, (char)higher_byte, '\0'};
    uint8_t converted_code = (uint8_t)strtoul(kLicenseeCodeString, NULL, 10);

    return converted_code;
}

static const char* getCartridgeLicenseeName(void) {
    uint8_t converted_code = getCartridgeLicenseeCodeFromValue();

    if (converted_code <= kMaxNewLicenseeCode) {
        return kLicenseeCodeNames[converted_code];
    } else {
        return "Invalid Licensee Code"; 
    }
}

static const char* getCartridgeType(void) {
    if (header->cartridge_type <= kMaxCartridgeType) {
        return kCartridgeTypes[header->cartridge_type];
    }

    return "Invalid Type";
}

static bool isChecksumValid(void) {
    uint16_t x = 0;

    for (uint16_t i = 0x0134; i <= 0x014C; ++i) {
        x -= rom_data[i] - 1;
    }

    return (x & 0xFF);
}

bool loadCartridge(const char* rom_file_name) {
    FILE *fp = fopen(rom_file_name, "r");

    if (!fp) {
        printf("Failed to open: %s\n", rom_file_name);
        return false;
    }

    printf("Open: %s\n", rom_file_name);

    fseek(fp, 0, SEEK_END);
    rom_size = ftell(fp);

    printf("ROM size: %lu bytes\n", rom_size);

    rewind(fp);

    rom_data = malloc(rom_size);

    if (NULL == rom_data) {
        perror("Failed to allocate memory for ROM data!");
        return false;
    }

    fread(rom_data, rom_size, 1, fp);
    fclose(fp);

    header = (CartridgeHeader*)(rom_data + kRomSpace);
    
    header->title[15] = '\0';

    bool is_checksum_valid = isChecksumValid();

    printf("Cartridge Loaded:\n");
    printf("\t Title    : %s\n", header->title);
    printf("\t Type     : %2.2Xh (%s)\n", header->cartridge_type, getCartridgeType());
    printf("\t ROM Size : %d KB\n", 32 << header->rom_size);
    printf("\t RAM Size : %2.2Xh\n", header->ram_size);
    printf("\t LIC Code : %2.2Xh (%s)\n", getCartridgeLicenseeCodeFromValue(), getCartridgeLicenseeName());
    printf("\t ROM Vers : %2.2Xh\n", header->rom_version_number);
    printf("\t Checksum : %2.2Xh (%s)\n", header->header_checksum, is_checksum_valid ? "PASSED" : "FAILED");

    free(rom_data);
    
    return is_checksum_valid;
}
