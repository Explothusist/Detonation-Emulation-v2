
#include "memory.h"

#include <cstdio>
#include <cstring>
#include <cctype>
#include "../utils.h"

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

constexpr uint16_t combine_chars(char a, char b) {
    return (static_cast<uint16_t>(a) << 8) | static_cast<uint8_t>(b);
};


Memory_Handler::Memory_Handler():
    XFFFF_IE{ 0 },

    m_in_boot_rom_internal{ true },
    m_has_load_error{ false },
    m_has_load_warning{ false },
    m_first_error{ "" },
    m_first_warning{ "" },

    m_rom_bank_1{ 0 },
    m_rom_bank_2{ 1 },
    m_ram_bank{ 0 },

    m_romname{ "" },
    m_cart_name{ "" },
    m_rom_size_name{ "" },
    m_ram_size_name{ "" },
    m_is_cgb_cart{ "" },
    m_licensee{ "" },
    m_passes_checksum{ false },
    m_valid_logo{ false },
    
    m_romname_color{ COLOR_BLACK },
    m_cart_name_color{ COLOR_BLACK },
    m_rom_size_name_color{ COLOR_BLACK },
    m_ram_size_name_color{ COLOR_BLACK },
    m_is_cgb_cart_color{ COLOR_BLACK },
    m_licensee_color{ COLOR_BLACK},
    m_passes_checksum_color{ COLOR_BLACK },
    m_valid_logo_color{ COLOR_BLACK },

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

    m_in_boot_rom_internal = true;
    m_has_load_error = false;
    m_has_load_warning = false;
    m_first_error = "";
    m_first_warning = "";

    m_rom_bank_1 = 0;
    m_rom_bank_2 = 1;
    m_ram_bank = 0;

    m_romname = "";
    m_cart_name = "";
    m_rom_size_name = "";
    m_ram_size_name = "";
    m_is_cgb_cart = "";
    m_licensee = "";
    m_passes_checksum = false;
    m_valid_logo = false;

    m_romname_color = COLOR_BLACK;
    m_cart_name_color = COLOR_BLACK;
    m_rom_size_name_color = COLOR_BLACK;
    m_ram_size_name_color = COLOR_BLACK;
    m_is_cgb_cart_color = COLOR_BLACK;
    m_licensee_color = COLOR_BLACK;
    m_passes_checksum_color = COLOR_BLACK;
    m_valid_logo_color = COLOR_BLACK;

    m_mbc_type = 0;
    m_cart_has_ram = false;
    m_cart_has_battery = false;
    m_cart_has_timer = false;
    m_cart_has_rumble = false;

    m_num_rom_banks = 0;
    m_num_ram_banks = 0;
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
            printf("WARNING: Memory Setup: Cartridge Uses Type 'ROM + RAM' Never Used In Licensed Games\n");
            if (m_first_warning == "") {
                m_first_warning = "Cartridge Type 'ROM + RAM' Never Used In Licensed Games";
            }
            m_cart_name_color = COLOR_ORANGE;
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_cart_name = "ROM + RAM (Unused Type)";
            m_mbc_type = 0;
            m_cart_has_ram = true;
            break;
        case 0x09: // ROM + RAM + Battery (Unused Type)
            printf("WARNING: Memory Setup: Cartridge Uses Type 'ROM + RAM + Battery' Never Used In Licensed Games\n");
            if (m_first_warning == "") {
                m_first_warning = "Cartridge Type 'ROM + RAM' Never Used In Licensed Games";
            }
            m_cart_name_color = COLOR_ORANGE;
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
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'MMM01'";
            }
            m_cart_name_color = COLOR_RED;
            m_has_load_error = true;
            break;
        case 0x0c: // MMM01 + RAM
            m_cart_name = "MMM01 + RAM";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01 + RAM' Not Supported\n");
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'MMM01'";
            }
            m_cart_name_color = COLOR_RED;
            m_has_load_error = true;
            break;
        case 0x0d: // MMM01 + RAM + Battery
            m_cart_name = "MMM01 + RAM + Battery";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01 + RAM + Battery' Not Supported\n");
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'MMM01'";
            }
            m_cart_name_color = COLOR_RED;
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
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'Pocket Camera'";
            }
            m_cart_name_color = COLOR_RED;
            m_has_load_error = true;
            break;
        case 0xfd: // Bandai Tama5
            m_cart_name = "Bandai Tama5";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'Bandai Tama5' Not Supported\n");
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'Bandai Tama5'";
            }
            m_cart_name_color = COLOR_RED;
            m_has_load_error = true;
            break;
        case 0xfe: // HuC3
            m_cart_name = "HuC3";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'HuC3' Not Supported\n");
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'HuC3'";
            }
            m_cart_name_color = COLOR_RED;
            m_has_load_error = true;
            break;
        case 0xff: // HuC1 + RAM + Battery
            m_cart_name = "HuC1 + RAM + Battery";
            printf("ERROR: Memory Setup: Cartridge Uses Type 'HuC1 + RAM + Battery' Not Supported\n");
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'HuC1'";
            }
            m_cart_name_color = COLOR_RED;
            m_has_load_error = true;
            break;
        default:
            m_cart_name = "Unknown";
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized Type '%i'\n", cart_type);
            if (m_first_error == "") {
                m_first_error = "Unsupported Cartridge Type 'Unknown'";
            }
            m_cart_name_color = COLOR_RED;
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
            if (m_first_warning == "") {
                m_first_warning = "ROM Size '72 banks' Not Used In Licensed Games";
            }
            m_rom_size_name_color = COLOR_ORANGE;
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_num_rom_banks = 72;
            m_rom_size_name = "72 ROM Banks, 1.1 MiB (Unused Type)";
            break;
        case 0x53: // Only listed in unofficial docs
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '80 banks'\n");
            if (m_first_warning == "") {
                m_first_warning = "ROM Size '80 banks' Not Used In Licensed Games";
            }
            m_rom_size_name_color = COLOR_ORANGE;
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_num_rom_banks = 80;
            m_rom_size_name = "80 ROM Banks, 1.2 MiB (Unused Type)";
            break;
        case 0x54: // Only listed in unofficial docs
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '96 banks'\n");
            if (m_first_warning == "") {
                m_first_warning = "ROM Size '96 banks' Not Used In Licensed Games";
            }
            m_rom_size_name_color = COLOR_ORANGE;
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_rom_size_name = "96 ROM Banks, 1.5 MiB (Unused Type)";
            m_num_rom_banks = 96;
            break;
        default:
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized ROM Size '%i'\n", rom_size);
            if (m_first_error == "") {
                m_first_error = "Cartridge ROM Size Unrecognized";
            }
            m_rom_size_name_color = COLOR_RED;
            m_rom_size_name = "Unknown";
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
            if (m_first_warning == "") {
                m_first_warning = "RAM Size '2 KiB' Not Used In Licensed Games";
            }
            m_ram_size_name_color = COLOR_ORANGE;
            // m_has_load_error = true;
            m_has_load_warning = true;
            m_num_rom_banks = 1;
            m_ram_size_name = "(1) RAM Bank, 2 KiB (Unused Type)";
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
            if (m_first_error == "") {
                m_first_error = "Cartridge RAM Size Unrecognized";
            }
            m_ram_size_name_color = COLOR_RED;
            m_ram_size_name = "Unknown";
            m_has_load_error = true;
            break;
    }
};
void Memory_Handler::interpret_licensee_code(uint8_t old_code, uint8_t new_code_1, uint8_t new_code_2) {
    if (old_code != 0x33) {
        switch (old_code) {
            case 0x00: m_licensee = "None"; break;
            case 0x01: m_licensee = "Nintendo"; break;
            case 0x08: m_licensee = "Capcom"; break;
            case 0x09: m_licensee = "HOT-B"; break;
            case 0x0A: m_licensee = "Jaleco"; break;
            case 0x0B: m_licensee = "Coconuts Japan"; break;
            case 0x0C: m_licensee = "Elite Systems"; break;
            case 0x13: m_licensee = "EA (Electronic Arts)"; break;
            case 0x18: m_licensee = "Hudson Soft"; break;
            case 0x19: m_licensee = "ITC Entertainment"; break;
            case 0x1A: m_licensee = "Yanoman"; break;
            case 0x1D: m_licensee = "Japan Clary"; break;
            case 0x1F: m_licensee = "Virgin Games Ltd."; break;
            case 0x24: m_licensee = "PCM Complete"; break;
            case 0x25: m_licensee = "San-X"; break;
            case 0x28: m_licensee = "Kemco"; break;
            case 0x29: m_licensee = "SETA Corporation"; break;
            case 0x30: m_licensee = "Infogrames"; break;
            case 0x31: m_licensee = "Nintendo"; break;
            case 0x32: m_licensee = "Bandai"; break;
            // case 0x33: m_licensee = "Use New Licensee Code"; break;
            case 0x34: m_licensee = "Konami"; break;
            case 0x35: m_licensee = "HectorSoft"; break;
            case 0x38: m_licensee = "Capcom"; break;
            case 0x39: m_licensee = "Banpresto"; break;
            case 0x3C: m_licensee = "Entertainment Interactive"; break;
            case 0x3E: m_licensee = "Gremlin"; break;
            case 0x41: m_licensee = "Ubi Soft"; break;
            case 0x42: m_licensee = "Atlus"; break;
            case 0x44: m_licensee = "Malibu Interactive"; break;
            case 0x46: m_licensee = "Angel"; break;
            case 0x47: m_licensee = "Spectrum HoloByte"; break;
            case 0x49: m_licensee = "Irem"; break;
            case 0x4A: m_licensee = "Virgin Games Ltd."; break;
            case 0x4D: m_licensee = "Malibu Interactive"; break;
            case 0x4F: m_licensee = "U.S. Gold"; break;
            case 0x50: m_licensee = "Absolute"; break;
            case 0x51: m_licensee = "Acclaim Entertainment"; break;
            case 0x52: m_licensee = "Activision"; break;
            case 0x53: m_licensee = "Sammy USA Corporation"; break;
            case 0x54: m_licensee = "GameTek"; break;
            case 0x55: m_licensee = "Park Place"; break;
            case 0x56: m_licensee = "LJN"; break;
            case 0x57: m_licensee = "Matchbox"; break;
            case 0x59: m_licensee = "Milton Bradley Company"; break;
            case 0x5A: m_licensee = "Mindscape"; break;
            case 0x5B: m_licensee = "Romstar"; break;
            case 0x5C: m_licensee = "Naxat Soft"; break;
            case 0x5D: m_licensee = "Tradewest"; break;
            case 0x60: m_licensee = "Titus Interactive"; break;
            case 0x61: m_licensee = "Virgin Games Ltd."; break;
            case 0x67: m_licensee = "Ocean Software"; break;
            case 0x69: m_licensee = "EA (Electronic Arts)"; break;
            case 0x6E: m_licensee = "Elite Systems"; break;
            case 0x6F: m_licensee = "Electro Brain"; break;
            case 0x70: m_licensee = "Infogrames"; break;
            case 0x71: m_licensee = "Interplay Entertainment"; break;
            case 0x72: m_licensee = "Broderbund"; break;
            case 0x73: m_licensee = "Sculptured Software"; break;
            case 0x75: m_licensee = "The Sales Curve Limited"; break;
            case 0x78: m_licensee = "THQ"; break;
            case 0x79: m_licensee = "Accolade"; break;
            case 0x7A: m_licensee = "Triffix Entertainment"; break;
            case 0x7C: m_licensee = "MicroProse"; break;
            case 0x7F: m_licensee = "Kemco"; break;
            case 0x80: m_licensee = "Misawa Entertainment"; break;
            case 0x83: m_licensee = "LOZC G."; break;
            case 0x86: m_licensee = "Tokuma Shoten"; break;
            case 0x8B: m_licensee = "Bullet-Proof Software"; break;
            case 0x8C: m_licensee = "Vic Tokai Corp."; break;
            case 0x8E: m_licensee = "Ape Inc."; break;
            case 0x8F: m_licensee = "I’Max"; break;
            case 0x91: m_licensee = "Chunsoft Co."; break;
            case 0x92: m_licensee = "Video System"; break;
            case 0x93: m_licensee = "Tsubaraya Productions"; break;
            case 0x95: m_licensee = "Varie"; break;
            case 0x96: m_licensee = "Yonezawa / S’Pal"; break;
            case 0x97: m_licensee = "Kemco"; break;
            case 0x99: m_licensee = "Arc"; break;
            case 0x9A: m_licensee = "Nihon Bussan"; break;
            case 0x9B: m_licensee = "Tecmo"; break;
            case 0x9C: m_licensee = "Imagineer"; break;
            case 0x9D: m_licensee = "Banpresto"; break;
            case 0x9F: m_licensee = "Nova"; break;
            case 0xA1: m_licensee = "Hori Electric"; break;
            case 0xA2: m_licensee = "Bandai"; break;
            case 0xA4: m_licensee = "Konami"; break;
            case 0xA6: m_licensee = "Kawada"; break;
            case 0xA7: m_licensee = "Takara"; break;
            case 0xA9: m_licensee = "Technos Japan"; break;
            case 0xAA: m_licensee = "Broderbund"; break;
            case 0xAC: m_licensee = "Toei Animation"; break;
            case 0xAD: m_licensee = "Toho"; break;
            case 0xAF: m_licensee = "Namco"; break;
            case 0xB0: m_licensee = "Acclaim Entertainment"; break;
            case 0xB1: m_licensee = "ASCII Corporation / Nexsoft"; break;
            case 0xB2: m_licensee = "Bandai"; break;
            case 0xB4: m_licensee = "Square Enix"; break;
            case 0xB6: m_licensee = "HAL Laboratory"; break;
            case 0xB7: m_licensee = "SNK"; break;
            case 0xB9: m_licensee = "Pony Canyon"; break;
            case 0xBA: m_licensee = "Culture Brain"; break;
            case 0xBB: m_licensee = "Sunsoft"; break;
            case 0xBD: m_licensee = "Sony Imagesoft"; break;
            case 0xBF: m_licensee = "Sammy Corporation"; break;
            case 0xC0: m_licensee = "Taito"; break;
            case 0xC2: m_licensee = "Kemco"; break;
            case 0xC3: m_licensee = "Square"; break;
            case 0xC4: m_licensee = "Tokuma Shoten"; break;
            case 0xC5: m_licensee = "Data East"; break;
            case 0xC6: m_licensee = "Tonkin House"; break;
            case 0xC8: m_licensee = "Koei"; break;
            case 0xC9: m_licensee = "UFL"; break;
            case 0xCA: m_licensee = "Ultra Games"; break;
            case 0xCB: m_licensee = "VAP, Inc."; break;
            case 0xCC: m_licensee = "Use Corporation"; break;
            case 0xCD: m_licensee = "Meldac"; break;
            case 0xCE: m_licensee = "Pony Canyon"; break;
            case 0xCF: m_licensee = "Angel"; break;
            case 0xD0: m_licensee = "Taito"; break;
            case 0xD1: m_licensee = "SOFEL"; break;
            case 0xD2: m_licensee = "Quest"; break;
            case 0xD3: m_licensee = "Sigma Enterprises"; break;
            case 0xD4: m_licensee = "ASK Kodansha Co."; break;
            case 0xD6: m_licensee = "Naxat Soft"; break;
            case 0xD7: m_licensee = "Copya System"; break;
            case 0xD9: m_licensee = "Banpresto"; break;
            case 0xDA: m_licensee = "Tomy"; break;
            case 0xDB: m_licensee = "LJN"; break;
            case 0xDD: m_licensee = "Nippon Computer Systems"; break;
            case 0xDE: m_licensee = "Human Entertainment"; break;
            case 0xDF: m_licensee = "Altron"; break;
            case 0xE0: m_licensee = "Jaleco"; break;
            case 0xE1: m_licensee = "Towa Chiki"; break;
            case 0xE2: m_licensee = "Yutaka"; break;
            case 0xE3: m_licensee = "Varie"; break;
            case 0xE5: m_licensee = "Epoch"; break;
            case 0xE7: m_licensee = "Athena"; break;
            case 0xE8: m_licensee = "Asmik Ace Entertainment"; break;
            case 0xE9: m_licensee = "Natsume"; break;
            case 0xEA: m_licensee = "King Records"; break;
            case 0xEB: m_licensee = "Atlus"; break;
            case 0xEC: m_licensee = "Epic / Sony Records"; break;
            case 0xEE: m_licensee = "IGS"; break;
            case 0xF0: m_licensee = "A Wave"; break;
            case 0xF3: m_licensee = "Extreme Entertainment"; break;
            case 0xFF: m_licensee = "LJN"; break;
            default: m_licensee = "Unknown"; break;
        }
    }else {
        if (!std::isprint(new_code_1) || !std::isprint(new_code_2)) {
            m_licensee = "Invalid New Licensee Code";
            return;
        }
        switch (combine_chars(static_cast<char>(new_code_1), static_cast<char>(new_code_2))) {
            case combine_chars('0','0'): m_licensee = "None"; break;
            case combine_chars('0','1'): m_licensee = "Nintendo Research & Development 1"; break;
            case combine_chars('0','8'): m_licensee = "Capcom"; break;
            case combine_chars('1','3'): m_licensee = "EA (Electronic Arts)"; break;
            case combine_chars('1','8'): m_licensee = "Hudson Soft"; break;
            case combine_chars('1','9'): m_licensee = "B-AI"; break;
            case combine_chars('2','0'): m_licensee = "KSS"; break;
            case combine_chars('2','2'): m_licensee = "Planning Office WADA"; break;
            case combine_chars('2','4'): m_licensee = "PCM Complete"; break;
            case combine_chars('2','5'): m_licensee = "San-X"; break;
            case combine_chars('2','8'): m_licensee = "Kemco"; break;
            case combine_chars('2','9'): m_licensee = "SETA Corporation"; break;
            case combine_chars('3','0'): m_licensee = "Viacom"; break;
            case combine_chars('3','1'): m_licensee = "Nintendo"; break;
            case combine_chars('3','2'): m_licensee = "Bandai"; break;
            case combine_chars('3','3'): m_licensee = "Ocean Software / Acclaim Entertainment"; break;
            case combine_chars('3','4'): m_licensee = "Konami"; break;
            case combine_chars('3','5'): m_licensee = "HectorSoft"; break;
            case combine_chars('3','7'): m_licensee = "Taito"; break;
            case combine_chars('3','8'): m_licensee = "Hudson Soft"; break;
            case combine_chars('3','9'): m_licensee = "Banpresto"; break;
            case combine_chars('4','1'): m_licensee = "Ubi Soft"; break;
            case combine_chars('4','2'): m_licensee = "Atlus"; break;
            case combine_chars('4','4'): m_licensee = "Malibu Interactive"; break;
            case combine_chars('4','6'): m_licensee = "Angel"; break;
            case combine_chars('4','7'): m_licensee = "Bullet-Proof Software"; break;
            case combine_chars('4','9'): m_licensee = "Irem"; break;
            case combine_chars('5','0'): m_licensee = "Absolute"; break;
            case combine_chars('5','1'): m_licensee = "Acclaim Entertainment"; break;
            case combine_chars('5','2'): m_licensee = "Activision"; break;
            case combine_chars('5','3'): m_licensee = "Sammy USA Corporation"; break;
            case combine_chars('5','4'): m_licensee = "Konami"; break;
            case combine_chars('5','5'): m_licensee = "Hi Tech Expressions"; break;
            case combine_chars('5','6'): m_licensee = "LJN"; break;
            case combine_chars('5','7'): m_licensee = "Matchbox"; break;
            case combine_chars('5','8'): m_licensee = "Mattel"; break;
            case combine_chars('5','9'): m_licensee = "Milton Bradley Company"; break;
            case combine_chars('6','0'): m_licensee = "Titus Interactive"; break;
            case combine_chars('6','1'): m_licensee = "Virgin Games Ltd."; break;
            case combine_chars('6','4'): m_licensee = "Lucasfilm Games"; break;
            case combine_chars('6','7'): m_licensee = "Ocean Software"; break;
            case combine_chars('6','9'): m_licensee = "EA (Electronic Arts)"; break;
            case combine_chars('7','0'): m_licensee = "Infogrames"; break;
            case combine_chars('7','1'): m_licensee = "Interplay Entertainment"; break;
            case combine_chars('7','2'): m_licensee = "Broderbund"; break;
            case combine_chars('7','3'): m_licensee = "Sculptured Software"; break;
            case combine_chars('7','5'): m_licensee = "The Sales Curve Limited"; break;
            case combine_chars('7','8'): m_licensee = "THQ"; break;
            case combine_chars('7','9'): m_licensee = "Accolade"; break;
            case combine_chars('8','0'): m_licensee = "Misawa Entertainment"; break;
            case combine_chars('8','3'): m_licensee = "LOZC G."; break;
            case combine_chars('8','6'): m_licensee = "Tokuma Shoten"; break;
            case combine_chars('8','7'): m_licensee = "Tsukuda Original"; break;
            case combine_chars('9','1'): m_licensee = "Chunsoft Co."; break;
            case combine_chars('9','2'): m_licensee = "Video System"; break;
            case combine_chars('9','3'): m_licensee = "Ocean Software / Acclaim Entertainment"; break;
            case combine_chars('9','5'): m_licensee = "Varie"; break;
            case combine_chars('9','6'): m_licensee = "Yonezawa / S’Pal"; break;
            case combine_chars('9','7'): m_licensee = "Kaneko"; break;
            case combine_chars('9','9'): m_licensee = "Pack-In-Video"; break;
            case combine_chars('9','H'): m_licensee = "Bottom Up"; break;
            case combine_chars('A','4'): m_licensee = "Konami (Yu-Gi-Oh!)"; break;
            case combine_chars('B','L'): m_licensee = "MTO"; break;
            case combine_chars('D','K'): m_licensee = "Kodansha"; break;
            default: m_licensee = "Unknown"; break;
        }
    }
};
void Memory_Handler::Setup(std::vector<uint8_t>* rom, bool use_boot_rom) {
    // Full_Reset();

    m_in_boot_rom_internal = use_boot_rom;

    // Reading ROM Header
    uint8_t cgb_flag = rom->at(0x0143); // Part of title in earlier models
    uint8_t new_licensee_code_1 = rom->at(0x0144); // Part of title in earlier models
    uint8_t new_licensee_code_2 = rom->at(0x0145);
    uint8_t cart_type = rom->at(0x0147);
    uint8_t rom_size = rom->at(0x0148);
    uint8_t ram_size = rom->at(0x0149);
    uint8_t old_licensee_code = rom->at(0x014b);
    uint8_t header_checksum = rom->at(0x014d);
    

    // CGB Flag
    if (cgb_flag == 0x80) {
        printf("WARNING: Memory Setup: CGB Cartridge In Compatability Mode\n");
        if (m_first_warning == "") {
            m_first_warning = "CGB Cartridge In Compatability Mode";
        }
        m_is_cgb_cart_color = COLOR_ORANGE;
        // m_has_load_error = true;
        m_has_load_warning = true;
        m_is_cgb_cart = "CGB Cartridge with Compatability";
    }else if (cgb_flag == 0xc0) {
        printf("ERROR: Memory Setup: CGB Cartridge, Cannot Be Run\n");
        if (m_first_error == "") {
            m_first_error = "CGB Cartridge, Cannot Be Run";
        }
        m_is_cgb_cart_color = COLOR_RED;
        m_has_load_error = true;
        m_is_cgb_cart = "Uncompatible CGB Cartridge";
    }else {
        m_is_cgb_cart = "DMG Cartridge";
    }

    // Checksum
    uint8_t checksum = 0;
    for (uint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - rom->at(address) - 1;
    }
    if (checksum == header_checksum) {
        printf("COMPLETE: Memory Setup: Header Checksum Passed\n");
        m_passes_checksum = true;
    }else {
        printf("WARNING: Memory Setup: Header Checksum Failed\n");
        // if (m_first_warning == "") {
        //     m_first_warning = "Header Checksum Failed";
        // }
        m_passes_checksum_color = COLOR_ORANGE;
        // // m_has_load_error = true;
        // m_has_load_warning = true;
        m_passes_checksum = false;
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
        m_valid_logo = true;
    }else {
        printf("WARNING: Memory Setup: Nintendo Logo Does Not Match\n");
        if (m_first_warning == "") {
            m_first_warning = "Nintendo Logo Does Not Match";
        }
        m_valid_logo_color = COLOR_ORANGE;
        // m_has_load_error = true;
        m_has_load_warning = true;
        m_valid_logo = false;
    }

    interpret_cartridge_type(cart_type);
    interpret_rom_size_type(rom_size);
    interpret_ram_size_type(ram_size);
    interpret_licensee_code(old_licensee_code, new_licensee_code_1, new_licensee_code_2);
    m_romname = readROMName(rom);
    
    constexpr size_t ROM_BANK_SIZE = 0x4000;
    for (int bank = 0; bank < m_num_rom_banks; ++bank) {
        std::memcpy(
            m_RomBanks[bank],
            rom->data() + bank * ROM_BANK_SIZE,
            ROM_BANK_SIZE
        );
    }

    // Read Saved RAM WORKING HERE

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


std::string Memory_Handler::readROMName(std::vector<uint8_t>* rom) {
    std::string romname{ "" };
    uint16_t addr = 0x0134;
    uint8_t letter = rom->at(addr);
    while (letter != 0 && addr < 0x0143) {
        romname += char(letter);
        addr += 1;
        letter = rom->at(addr);
    }
    return romname;
};
bool Memory_Handler::hadLoadError() {
    return m_has_load_error;
};
bool Memory_Handler::hadLoadWarning() {
    return m_has_load_warning;
};
std::string Memory_Handler::getFirstError() {
    if (m_first_error == "") {
        return m_first_warning;
    }else {
        return m_first_error;
    }
};

std::vector<std::string> Memory_Handler::getCartDetails() {
    return { 
        m_romname, 
        m_cart_name, 
        m_rom_size_name, 
        m_ram_size_name, 
        m_is_cgb_cart, 
        m_licensee, 
        m_passes_checksum ? "Passed" : "Failed", 
        m_valid_logo ? "Passed" : "Failed" 
    };
};
std::vector<EntryColor> Memory_Handler::getCartDetailColors() {
    return { 
        m_romname_color, 
        m_cart_name_color, 
        m_rom_size_name_color, 
        m_ram_size_name_color, 
        m_is_cgb_cart_color, 
        m_licensee_color, 
        m_passes_checksum_color, 
        m_valid_logo_color
    };
};
