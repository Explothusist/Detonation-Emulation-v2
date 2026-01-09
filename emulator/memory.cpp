
#include "memory.h"

#include <cstdio>
#include <cstring>

Register_Handler::Register_Handler():
    F{ 0 },
    A{ 0 },
    C{ 0 },
    B{ 0 },
    E{ 0 },
    D{ 0 },
    L{ 0 },
    H{ 0 },
    SP{ 0 },
    PC{ 0 }
{

};
[[nodiscard]] uint8_t Register_Handler::get(Reg_u8 reg) const {
    switch (reg) {
        case Reg_u8::A: return A;
        case Reg_u8::F: return F;
        case Reg_u8::B: return B;
        case Reg_u8::C: return C;
        case Reg_u8::D: return D;
        case Reg_u8::E: return E;
        case Reg_u8::H: return H;
        case Reg_u8::L: return L;
        case Reg_u8::SP_H: return (SP >> 8);
        case Reg_u8::SP_L: return (SP & 0xff);
        case Reg_u8::PC_H: return (PC >> 8);
        case Reg_u8::PC_L: return (PC & 0xff);
    }
    return 0;
};
void Register_Handler::set(Reg_u8 reg, uint8_t value) {
    switch (reg) {
        case Reg_u8::A: A = value; break;
        case Reg_u8::F: F = value & 0xf0; break;
        case Reg_u8::B: B = value; break;
        case Reg_u8::C: C = value; break;
        case Reg_u8::D: D = value; break;
        case Reg_u8::E: E = value; break;
        case Reg_u8::H: H = value; break;
        case Reg_u8::L: L = value; break;
        case Reg_u8::SP_H: SP = ((SP & 0x00ff) | (uint16_t(value) << 8)); break;
        case Reg_u8::SP_L: SP = ((SP & 0xff00) | value); break;
        case Reg_u8::PC_H: PC = ((PC & 0x00ff) | (uint16_t(value) << 8)); break;
        case Reg_u8::PC_L: PC = ((PC & 0xff00) | value); break;
    }
};
[[nodiscard]] uint16_t Register_Handler::get(Reg_u16 reg) const {
    switch (reg) {
        case Reg_u16::AF: return ((uint16_t(A) << 8) | F);
        case Reg_u16::BC: return ((uint16_t(B) << 8) | C);
        case Reg_u16::DE: return ((uint16_t(D) << 8) | E);
        case Reg_u16::HL: return ((uint16_t(H) << 8) | L);
        case Reg_u16::SP: return (SP);
        case Reg_u16::PC: return (PC);
    }
    return 0;
};
void Register_Handler::set(Reg_u16 reg, uint16_t value) {
    switch (reg) {
        case Reg_u16::AF: A = (value >> 8); F = value & 0xf0; break;
        case Reg_u16::BC: B = (value >> 8); C = value & 0xff; break;
        case Reg_u16::DE: D = (value >> 8); E = value & 0xff; break;
        case Reg_u16::HL: H = (value >> 8); L = value & 0xff; break;
        case Reg_u16::SP: SP = value; break;
        case Reg_u16::PC: PC = value; break;
    }
};
[[nodiscard]] bool Register_Handler::get(Reg_flag reg) const {
    switch (reg) {
        case Reg_flag::Z: return ((F >> 7) & 1);
        case Reg_flag::N: return ((F >> 6) & 1);
        case Reg_flag::H: return ((F >> 5) & 1);
        case Reg_flag::C: return ((F >> 4) & 1);
    }
    return 0;
};
void Register_Handler::set(Reg_flag reg, bool value) {
    switch (reg) {
        case Reg_flag::Z: F = ((F & 0b01111111) | (value << 7)); break;
        case Reg_flag::N: F = ((F & 0b10111111) | (value << 6)); break;
        case Reg_flag::H: F = ((F & 0b11011111) | (value << 5)); break;
        case Reg_flag::C: F = ((F & 0b11101111) | (value << 4)); break;
    }
};


uint8_t Boot_ROM[0x100] = {
    0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, 
    0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, 
    0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B, 
    0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9, 
    0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, 
    0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04, 
    0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2, 
    0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 
    0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20, 
    0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17, 
    0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 
    0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20, 
    0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
};

uint8_t Nintendo_Logo[0x30] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};


Memory_Handler::Memory_Handler():
    XFFFF_IE{ 0 },

    m_in_boot_rom_internal{ true },
    m_has_load_error{ false },
    m_has_load_warning{ false },

    m_rom_bank_1{ 0 },
    m_rom_bank_2{ 1 },
    m_ram_bank{ 0 },

    m_romname{ },
    m_cart_name{ },
    m_rom_size_name{ },
    m_ram_size_name{ },

    m_mbc_type{ 0 },
    m_cart_has_ram{ false },
    m_cart_has_battery{ false },
    m_cart_has_timer{ false },
    m_cart_has_rumble{ false },

    m_num_rom_banks{ 0 },
    m_num_ram_banks{ 0 }
{
    Full_Reset();
};

void Memory_Handler::Full_Reset() { // This is extreme...
    // for (int j = 0; j < 512; j++) {
    //     // X0000_ROM_STATIC[i] = 0;
    //     // X4000_ROM_BANK[i] = 0;
    //     for (int i = 0; i < 0x4000; i++) {
    //         m_RomBanks[j][i] = 0;
    //     }
    // }
    // for (int j = 0; j < 16; j++) {
    //     // XA000_EXT_RAM[i] = 0;
    //     for (int i = 0; i < 0x2000; i++) {
    //         m_RamBanks[j][i] = 0;
    //     }
    // }
    std::memset(m_RomBanks, 0, sizeof(m_RomBanks));
    std::memset(m_RamBanks, 0, sizeof(m_RamBanks));
    Reset();
};
void Memory_Handler::Reset() {
    // for (int i = 0; i < 0x2000; i++) {
    //     X8000_VRAM[i] = 0;
    //     XC000_WORK_RAM[i] = 0;
    // }
    // for (int i = 0; i < 0xa0; i++) {
    //     XFE00_OAM[i] = 0;
    // }
    // for (int i = 0; i < 0x80; i++) {
    //     XFF00_IO_REGS[i] = 0; // This should use more callbacks
    // }
    // for (int i = 0; i < 0x7f; i++) {
    //     XFF80_HRAM[i] = 0;
    // }
    std::memset(X8000_VRAM,     0, sizeof(X8000_VRAM));
    std::memset(XC000_WORK_RAM, 0, sizeof(XC000_WORK_RAM));
    std::memset(XFE00_OAM,      0, sizeof(XFE00_OAM));
    std::memset(XFF00_IO_REGS,  0, sizeof(XFF00_IO_REGS));
    std::memset(XFF80_HRAM,     0, sizeof(XFF80_HRAM));
    XFFFF_IE = 0;
};

void Memory_Handler::interpret_cartridge_type(uint8_t cart_type) {
    switch (cart_type) {
        case 0x00: // ROM Only
            m_cart_name = "ROM Only";
            m_mbc_type = 0;
            break;
        case 0x01: // MBC1
            m_cart_name = "MBC1";
            m_mbc_type = 1;
            break;
        case 0x02: // MBC1 + RAM
            m_cart_name = "MBC1 + RAM";
            m_mbc_type = 1;
            m_cart_has_ram = true;
            break;
        case 0x03: // MBC1 + RAM + BATTERY
            m_cart_name = "MBC1 + RAM + BATTERY";
            m_mbc_type = 1;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x05: // MBC2
            m_cart_name = "MBC2";
            m_mbc_type = 2;
            // m_cart_has_ram = true;
            break;
        case 0x06: // MBC2 + Battery
            m_cart_name = "MBC2 + Battery";
            m_mbc_type = 2;
            // m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x08: // ROM + RAM (Unused Type)
            printf("WARNING: Memory Setup: Cartridge Uses Type 'ROM + RAM' Never Used In Liscensed Games\n");
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_cart_name = "ROM + RAM (Unused Type)";
            m_mbc_type = 0;
            m_cart_has_ram = true;
            break;
        case 0x09: // ROM + RAM + Battery (Unused Type)
            printf("WARNING: Memory Setup: Cartridge Uses Type 'ROM + RAM + Battery' Never Used In Liscensed Games\n");
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_cart_name = "ROM + RAM + Battery (Unused Type)";
            m_mbc_type = 0;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x0b: // MMM01
            m_cart_name = "MMM01";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01' Not Supported\n");
            m_has_load_error = true;
            break;
        case 0x0c: // MMM01 + RAM
            m_cart_name = "MMM01 + RAM";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01 + RAM' Not Supported\n");
            m_has_load_error = true;
            break;
        case 0x0d: // MMM01 + RAM + Battery
            m_cart_name = "MMM01 + RAM + Battery";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01 + RAM + Battery' Not Supported\n");
            m_has_load_error = true;
            break;
        case 0x0f: // MBC3 + Timer + Battery
            m_cart_name = "MBC3 + Timer + Battery";
            m_mbc_type = 3;
            m_cart_has_battery = true;
            m_cart_has_timer = true;
            break;
        case 0x10: // MBC3 + Timer + RAM + Battery
            m_cart_name = "MBC3 + Timer + RAM + Battery";
            m_mbc_type = 3;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            m_cart_has_timer = true;
            break;
        case 0x11: // MBC3
            m_cart_name = "MBC3";
            m_mbc_type = 3;
            break;
        case 0x12: // MBC3 + RAM
            m_cart_name = "MBC3 + RAM";
            m_mbc_type = 3;
            m_cart_has_ram = true;
            break;
        case 0x13: // MBC3 + RAM + Battery
            m_cart_name = "MBC3 + RAM + Battery";
            m_mbc_type = 3;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x19: // MBC5
            m_cart_name = "MBC5";
            m_mbc_type = 5;
            break;
        case 0x1a: // MBC5 + RAM
            m_cart_name = "MBC5 + RAM";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            break;
        case 0x1b: // MBC5 + RAM + Battery
            m_cart_name = "MBC5 + RAM + Battery";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x1c: // MBC5 + Rumble
            m_cart_name = "MBC5 + Rumble";
            m_mbc_type = 5;
            m_cart_has_rumble = true;
            break;
        case 0x1d: // MBC5 + Rumble + RAM
            m_cart_name = "MBC5 + Rumble + RAM";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            m_cart_has_rumble = true;
            break;
        case 0x1e: // MBC5 + Rumble + RAM + Battery
            m_cart_name = "MBC5 + Rumble + RAM + Battery";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            m_cart_has_rumble = true;
            break;
        case 0x20: // MBC6
            m_cart_name = "MBC6";
            m_mbc_type = 6;
            break;
        case 0x22: // MBC7 + Sensor + Rumble + RAM + Battery
            m_cart_name = "MBC7 + Sensor + Rumble + RAM + Battery";
            m_mbc_type = 7;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0xfc: // Pocket Camera
            m_cart_name = "Pocket Camera";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'Pocket Camera' Not Supported\n");
            m_has_load_error = true;
            break;
        case 0xfd: // Bandai Tama5
            m_cart_name = "Bandai Tama5";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'Bandai Tama5' Not Supported\n");
            m_has_load_error = true;
            break;
        case 0xfe: // HuC3
            m_cart_name = "HuC3";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'HuC3' Not Supported\n");
            m_has_load_error = true;
            break;
        case 0xff: // HuC1 + RAM + Battery
            m_cart_name = "HuC1 + RAM + Battery";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'HuC1 + RAM + Battery' Not Supported\n");
            m_has_load_error = true;
            break;
        default:
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized Type '%i'\n", cart_type);
            m_has_load_error = true;
            break;
    }
};
void Memory_Handler::interpret_rom_size_type(uint8_t rom_size) {
    switch (rom_size) {
        case 0x00:
            m_num_rom_banks = 2;
            m_rom_size_name = "No ROM Banking, 32 KiB";
            break;
        case 0x01:
            m_num_rom_banks = 4;
            m_rom_size_name = "4 ROM Banks, 64 KiB";
            break;
        case 0x02:
            m_num_rom_banks = 8;
            m_rom_size_name = "8 ROM Banks, 128 KiB";
            break;
        case 0x03:
            m_num_rom_banks = 16;
            m_rom_size_name = "16 ROM Banks, 256 KiB";
            break;
        case 0x04:
            m_num_rom_banks = 32;
            m_rom_size_name = "32 ROM Banks, 512 KiB";
            break;
        case 0x05:
            m_num_rom_banks = 64;
            m_rom_size_name = "64 ROM Banks, 1 MiB";
            break;
        case 0x06:
            m_num_rom_banks = 128;
            m_rom_size_name = "128 ROM Banks, 2 MiB";
            break;
        case 0x07:
            m_num_rom_banks = 256;
            m_rom_size_name = "256 ROM Banks, 4 MiB";
            break;
        case 0x08:
            m_num_rom_banks = 512;
            m_rom_size_name = "512 ROM Banks, 8 MiB";
            break;
        case 0x52: // Only listed in unofficial docs
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '72 banks'\n");
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_num_rom_banks = 72;
            m_rom_size_name = "72 ROM Banks, 1.1 MiB";
            break;
        case 0x53: // Only listed in unofficial docs
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '80 banks'\n");
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_num_rom_banks = 80;
            m_rom_size_name = "80 ROM Banks, 1.2 MiB";
            break;
        case 0x54: // Only listed in unofficial docs
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '96 banks'\n");
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_rom_size_name = "96 ROM Banks, 1.5 MiB";
            m_num_rom_banks = 96;
            break;
        default:
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized ROM Size '%i'\n", rom_size);
            m_has_load_error = true;
            break;
    }
};
void Memory_Handler::interpret_ram_size_type(uint8_t ram_size) {
    switch (ram_size) {
        case 0x00:
            m_num_rom_banks = 0;
            m_ram_size_name = "No RAM";
            break;
        case 0x01: // Only listed in unofficial docs
            printf("WARNING: Memory Setup: Uses Unused Cartridge RAM Size '2 KiB'\n");
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_num_rom_banks = 1;
            m_ram_size_name = "1 Partial RAM Bank, 2 KiB";
            break;
        case 0x02:
            m_num_rom_banks = 1;
            m_ram_size_name = "1 RAM Bank, 8 KiB";
            break;
        case 0x03:
            m_num_rom_banks = 4;
            m_ram_size_name = "4 RAM Bank, 32 KiB";
            break;
        case 0x04:
            m_num_rom_banks = 16;
            m_ram_size_name = "16 RAM Bank, 128 KiB";
            break;
        case 0x05:
            m_num_rom_banks = 8;
            m_ram_size_name = "8 RAM Bank, 64 KiB";
            break;
        default:
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized RAM Size '%i'\n", ram_size);
            m_has_load_error = true;
            break;
    }
};
void Memory_Handler::Setup(std::vector<uint8_t>* rom, bool use_boot_rom) {
    Full_Reset();

    m_in_boot_rom_internal = use_boot_rom;

    uint8_t cgb_flag = rom->at(0x0143); // Part of title in earlier models
    uint8_t cart_type = rom->at(0x0147); // Reading ROM Header
    uint8_t rom_size = rom->at(0x0148);
    uint8_t ram_size = rom->at(0x0149);
    uint8_t header_checksum = rom->at(0x014d);

    // CGB Flag
    if (cgb_flag == 0x80) {
        printf("WARNING: Memory Setup: CGB Cartridge In Compatability Mode\n");
        // m_has_load_error = true;
        m_has_load_warning = true;
    }else if (cgb_flag == 0xc0) {
        printf("ERROR: Memory Setup: CGB Cartridge, Cannot Be Run\n");
        m_has_load_error = true;
    }

    // Checksum
    uint8_t checksum = 0;
    for (uint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - rom->at(address) - 1;
    }
    if (checksum == header_checksum) {
        printf("COMPLETE: Memory Setup: Header Checksum Passed\n");
    }else {
        printf("WARNING: Memory Setup: Header Checksum Failed\n");
        // m_has_load_error = true;
        m_has_load_warning = true;
    }

    // Nintendo Logo
    bool logo_matches = true;
    for (uint16_t address = 0x0104; address <= 0x0133; address++) {
        if (rom->at(address) != Nintendo_Logo[address - 0x0104]) {
            logo_matches = false;
        }
    }
    if (logo_matches) {
        printf("COMPLETE: Memory Setup: Nintendo Logo Matches\n");
    }else {
        printf("WARNING: Memory Setup: Nintendo Logo Does Not Match\n");
        // m_has_load_error = true;
        m_has_load_warning = true;
    }

    interpret_cartridge_type(cart_type);
    interpret_rom_size_type(rom_size);
    interpret_ram_size_type(ram_size);
    
    constexpr size_t ROM_BANK_SIZE = 0x4000;
    for (int bank = 0; bank < m_num_rom_banks; ++bank) {
        std::memcpy(
            m_RomBanks[bank],
            rom->data() + bank * ROM_BANK_SIZE,
            ROM_BANK_SIZE
        );
    }

    // Read Saved RAM WORKING HERE

    m_romname = readROMName();

    if (!m_has_load_error) {
        if (!m_has_load_warning) {
            printf("COMPLETE: Memory Setup: ROM Loaded\n");
        }else {
            printf("COMPLETE: Memory Setup: ROM Loaded With Warning\n");
        }
        printf("ROM Name: %s\n", m_romname.c_str());
        printf("Cartridge Type: %i (%s)\n", cart_type, m_cart_name.c_str());
        printf("ROM Size: %i (%s)\n", rom_size, m_rom_size_name.c_str());
        printf("RAM Size: %i (%s)\n", ram_size, m_ram_size_name.c_str());
    }else {
        printf("ERROR: Memory Setup: ROM Could Not Be Loaded\n");
    }
};

uint8_t Memory_Handler::Read(uint16_t address) {
    if (address < 0x4000) {
        // return X0000_ROM_STATIC[address];
        return m_RomBanks[m_rom_bank_1][address];
    }else if (address < 0x8000) {
        // return X4000_ROM_BANK[address - 0x4000];
        return m_RomBanks[m_rom_bank_2][address - 0x4000];
    }else if (address < 0xa000) {
        return X8000_VRAM[address - 0x8000];
    }else if (address < 0xc000) {
        // return XA000_EXT_RAM[address - 0xa000];
        return m_RamBanks[m_ram_bank][address - 0xa000];
    }else if (address < 0xe000) {
        return XC000_WORK_RAM[address - 0xc000];
    }else if (address < 0xfe00) {
        printf("WARNING: Memory Read: Echo RAM Read");
        return XC000_WORK_RAM[address - 0xe000];
    }else if (address < 0xfea0) {
        return XFE00_OAM[address - 0xfe00];
    }else if (address < 0xff00) {
        printf("ERROR: Memory Read: Unusable Memory Read");
        return 0;
    }else if (address < 0xff80) {
        return XFF00_IO_REGS[address - 0xff00]; // Reading special registers...
    }else if (address < 0xffff) {
        return XFF80_HRAM[address - 0xff80];
    }else {
        return XFFFF_IE;
    }
};
void Memory_Handler::Write(uint16_t address, uint8_t value) {
    if (address < 0x4000) {
        // X0000_ROM_STATIC[address] = value; // READ Only Memory
    }else if (address < 0x8000) {
        // X4000_ROM_BANK[address - 0x4000] = value; // READ Only Memory
    }else if (address < 0xa000) {
        X8000_VRAM[address - 0x8000] = value;
    }else if (address < 0xc000) {
        // XA000_EXT_RAM[address - 0xa000] = value;
        m_RamBanks[m_ram_bank][address - 0xa000] = value;
    }else if (address < 0xe000) {
        XC000_WORK_RAM[address - 0xc000] = value;
    }else if (address < 0xfe00) {
        printf("WARNING: Memory Read: Echo RAM Read");
        XC000_WORK_RAM[address - 0xe000] = value;
    }else if (address < 0xfea0) {
        XFE00_OAM[address - 0xfe00] = value;
    }else if (address < 0xff00) {
        printf("ERROR: Memory Read: Unusable Memory Read");
    }else if (address < 0xff80) {
        XFF00_IO_REGS[address - 0xff00] = value; // Reading special registers...
    }else if (address < 0xffff) {
        XFF80_HRAM[address - 0xff80] = value;
    }else {
        XFFFF_IE = value;
    }
};
uint8_t Memory_Handler::_Get(uint16_t address) {
    if (address < 0x4000) {
        // return X0000_ROM_STATIC[address];
        return m_RomBanks[m_rom_bank_1][address];
    }else if (address < 0x8000) {
        // return X4000_ROM_BANK[address - 0x4000];
        return m_RomBanks[m_rom_bank_2][address - 0x4000];
    }else if (address < 0xa000) {
        return X8000_VRAM[address - 0x8000];
    }else if (address < 0xc000) {
        // return XA000_EXT_RAM[address - 0xa000];
        return m_RamBanks[m_ram_bank][address - 0xa000];
    }else if (address < 0xe000) {
        return XC000_WORK_RAM[address - 0xc000];
    }else if (address < 0xfe00) {
        printf("WARNING: Memory Read: Echo RAM Read");
        return XC000_WORK_RAM[address - 0xe000];
    }else if (address < 0xfea0) {
        return XFE00_OAM[address - 0xfe00];
    }else if (address < 0xff00) {
        printf("ERROR: Memory Read: Unusable Memory Read");
        return 0;
    }else if (address < 0xff80) {
        return XFF00_IO_REGS[address - 0xff00];
    }else if (address < 0xffff) {
        return XFF80_HRAM[address - 0xff80];
    }else {
        return XFFFF_IE;
    }
};
void Memory_Handler::_Set(uint16_t address, uint8_t value) {
    if (address < 0x4000) {
        // X0000_ROM_STATIC[address] = value;
        m_RomBanks[m_rom_bank_1][address] = value; // _Set can do the impossible
    }else if (address < 0x8000) {
        // X4000_ROM_BANK[address - 0x4000] = value;
        m_RomBanks[m_rom_bank_2][address - 0x4000] = value; // _Set can do the impossible
    }else if (address < 0xa000) {
        X8000_VRAM[address - 0x8000] = value;
    }else if (address < 0xc000) {
        // XA000_EXT_RAM[address - 0xa000] = value;
        m_RamBanks[m_ram_bank][address - 0xa000] = value;
    }else if (address < 0xe000) {
        XC000_WORK_RAM[address - 0xc000] = value;
    }else if (address < 0xfe00) {
        printf("WARNING: Memory Read: Echo RAM Read");
        XC000_WORK_RAM[address - 0xe000] = value;
    }else if (address < 0xfea0) {
        XFE00_OAM[address - 0xfe00] = value;
    }else if (address < 0xff00) {
        printf("ERROR: Memory Read: Unusable Memory Read");
    }else if (address < 0xff80) {
        XFF00_IO_REGS[address - 0xff00] = value; // Reading special registers...
    }else if (address < 0xffff) {
        XFF80_HRAM[address - 0xff80] = value;
    }else {
        XFFFF_IE = value;
    }
};
uint8_t Memory_Handler::PC_Eat_Byte(Register_Handler &regs) {
    uint8_t ret = Read(regs.get(Reg_u16::PC)); // Read from <PC>
    regs.set(Reg_u16::PC, regs.get(Reg_u16::PC) + 1); // Increment PC
    return ret;
};


std::string Memory_Handler::readROMName() {
    std::string romname{ "" };
    uint16_t addr = 0x0134;
    uint8_t letter = _Get(addr);
    while (letter != 0 && addr < 0x0143) {
        romname += char(letter);
        addr += 1;
        letter = _Get(addr);
    }
    return romname;
};
bool Memory_Handler::hadLoadError() {
    return m_has_load_error;
};
bool Memory_Handler::hadLoadWarning() {
    return m_has_load_warning;
};
