
#include "memory.h"

#include <cstdio>
#include <cstring>
#include <cctype>
#include "../utils.h"
#include "../enable_logging.h"
#include "components/emulator_log.h"


std::string getRegName(Reg_u8 reg) {
    switch (reg) {
        case Reg_u8::A: return "A";
        case Reg_u8::F: return "F";
        case Reg_u8::B: return "B";
        case Reg_u8::C: return "C";
        case Reg_u8::D: return "D";
        case Reg_u8::E: return "E";
        case Reg_u8::H: return "H";
        case Reg_u8::L: return "L";
        case Reg_u8::SP_H: return "SP_H";
        case Reg_u8::SP_L: return "SP_L";
        case Reg_u8::PC_H: return "PC_H";
        case Reg_u8::PC_L: return "PC_L";
        case Reg_u8::WZ_H: return "WZ_H";
        case Reg_u8::WZ_L: return "WZ_L";
        case Reg_u8::temp_H: return "temp_H";
        case Reg_u8::temp_L: return "temp_L";
    }
    return "";
};
std::string getRegName(Reg_u16 reg) {
    switch (reg) {
        case Reg_u16::AF: return "AF";
        case Reg_u16::BC: return "BC";
        case Reg_u16::DE: return "DE";
        case Reg_u16::HL: return "HL";
        case Reg_u16::SP: return "SP";
        case Reg_u16::PC: return "PC";
        case Reg_u16::WZ: return "WZ";
        case Reg_u16::temp: return "temp";
    }
    return "";
};
std::string getRegName(Reg_flag reg) {
    switch (reg) {
        case Reg_flag::Z: return "fZ";
        case Reg_flag::N: return "fN";
        case Reg_flag::H: return "fH";
        case Reg_flag::C: return "fC";
        case Reg_flag::NZ: return "fNZ";
        case Reg_flag::NN: return "fNN";
        case Reg_flag::NH: return "fNH";
        case Reg_flag::NC: return "fNC";
    }
    return "";
};

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
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Read Reg_u8 "+getRegName(reg)+", ");
        }
    #endif
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
        case Reg_u8::WZ_H: return (WZ >> 8);
        case Reg_u8::WZ_L: return (WZ & 0xff);
        case Reg_u8::temp_H: return (temp >> 8);
        case Reg_u8::temp_L: return (temp & 0xff);
    }
    return 0;
};
void Register_Handler::set(Reg_u8 reg, uint8_t value) {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Set Reg_u8 "+getRegName(reg)+" to "+to_b8_out(value)+", ");
        }
    #endif
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
        case Reg_u8::WZ_H: WZ = ((WZ & 0x00ff) | (uint16_t(value) << 8)); break;
        case Reg_u8::WZ_L: WZ = ((WZ & 0xff00) | value); break;
        case Reg_u8::temp_H: temp = ((temp & 0x00ff) | (uint16_t(value) << 8)); break;
        case Reg_u8::temp_L: temp = ((temp & 0xff00) | value); break;
    }
};
[[nodiscard]] uint16_t Register_Handler::get(Reg_u16 reg) const {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Read Reg_u16 "+getRegName(reg)+", ");
        }
    #endif
    switch (reg) {
        case Reg_u16::AF: return ((uint16_t(A) << 8) | F);
        case Reg_u16::BC: return ((uint16_t(B) << 8) | C);
        case Reg_u16::DE: return ((uint16_t(D) << 8) | E);
        case Reg_u16::HL: return ((uint16_t(H) << 8) | L);
        case Reg_u16::SP: return (SP);
        case Reg_u16::PC: return (PC);
        case Reg_u16::WZ: return (WZ);
        case Reg_u16::temp: return (temp);
    }
    return 0;
};
void Register_Handler::set(Reg_u16 reg, uint16_t value) {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Set Reg_u16 "+getRegName(reg)+" to "+to_b16_out(value)+", ");
        }
    #endif
    switch (reg) {
        case Reg_u16::AF: A = (value >> 8); F = value & 0xf0; break;
        case Reg_u16::BC: B = (value >> 8); C = value & 0xff; break;
        case Reg_u16::DE: D = (value >> 8); E = value & 0xff; break;
        case Reg_u16::HL: H = (value >> 8); L = value & 0xff; break;
        case Reg_u16::SP: SP = value; break;
        case Reg_u16::PC: PC = value; break;
        case Reg_u16::WZ: WZ = value; break;
        case Reg_u16::temp: temp = value; break;
    }
};
[[nodiscard]] bool Register_Handler::get(Reg_flag reg) const {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Read Reg_flag "+getRegName(reg)+", ");
        }
    #endif
    switch (reg) {
        case Reg_flag::Z: return bool((F >> 7) & 1);
        case Reg_flag::N: return bool((F >> 6) & 1);
        case Reg_flag::H: return bool((F >> 5) & 1);
        case Reg_flag::C: return bool((F >> 4) & 1);
        case Reg_flag::NZ: return !bool((F >> 7) & 1);
        case Reg_flag::NN: return !bool((F >> 6) & 1);
        case Reg_flag::NH: return !bool((F >> 5) & 1);
        case Reg_flag::NC: return !bool((F >> 4) & 1);
    }
    return false;
};
void Register_Handler::set(Reg_flag reg, bool value) {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Read Reg_flag "+getRegName(reg)+" to "+std::to_string(value)+", ");
        }
    #endif
    switch (reg) {
        case Reg_flag::Z: F = ((F & 0b0111'0000) | (uint8_t(value) << 7)); break;
        case Reg_flag::N: F = ((F & 0b1011'0000) | (uint8_t(value) << 6)); break;
        case Reg_flag::H: F = ((F & 0b1101'0000) | (uint8_t(value) << 5)); break;
        case Reg_flag::C: F = ((F & 0b1110'0000) | (uint8_t(value) << 4)); break;
        case Reg_flag::NZ: // Reading is one thing (jumps/calls), But setting should never happen
#ifdef DEBUG_LOGGING
            printf("Cannot write to NZ flag!\n");
#endif
            break;
        case Reg_flag::NN:  
#ifdef DEBUG_LOGGING
            printf("Cannot write to NN flag!\n");
#endif
            break;
        case Reg_flag::NH: 
#ifdef DEBUG_LOGGING
            printf("Cannot write to NH flag!\n");
#endif
            break; 
        case Reg_flag::NC: 
#ifdef DEBUG_LOGGING
            printf("Cannot write to NC flag!\n");
#endif
            break; 
        default:
#ifdef DEBUG_LOGGING
            printf("Cannot write to N* flag!\n");
#endif
            break;
    }
};
void Register_Handler::latchFlags() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Latched Flags, ");
        }
    #endif
    F_latched = F;
};
[[nodiscard]] bool Register_Handler::getLatched(Reg_flag reg) const {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Read latched Reg_flag "+getRegName(reg)+", ");
        }
    #endif
    switch (reg) {
        case Reg_flag::Z: return bool((F_latched >> 7) & 1);
        case Reg_flag::N: return bool((F_latched >> 6) & 1);
        case Reg_flag::H: return bool((F_latched >> 5) & 1);
        case Reg_flag::C: return bool((F_latched >> 4) & 1);
        case Reg_flag::NZ: return !bool((F_latched >> 7) & 1);
        case Reg_flag::NN: return !bool((F_latched >> 6) & 1);
        case Reg_flag::NH: return !bool((F_latched >> 5) & 1);
        case Reg_flag::NC: return !bool((F_latched >> 4) & 1);
    }
    return false;
};

std::string Register_Handler::dumpState() {
    return std::string("A: ")+to_b8_out(A)
        + std::string(" F: ")+to_b8_out(F)
        + std::string(" B: ")+to_b8_out(B)
        + std::string(" C: ")+to_b8_out(C)
        + std::string(" D: ")+to_b8_out(D)
        + std::string(" E: ")+to_b8_out(E)
        + std::string(" H: ")+to_b8_out(H)
        + std::string(" L: ")+to_b8_out(L)
        + std::string(" PC: ")+to_b16_out(PC)
        + std::string(" SP: ")+to_b16_out(SP)
        + std::string(" WZ: ")+to_b16_out(WZ)
        + std::string(" temp: ")+to_b8_out(temp);
};


Cart_Details::Cart_Details():
    m_has_load_error{ false },
    m_has_load_warning{ false },
    m_first_error{ "" },
    m_first_warning{ "" },

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

};
bool Cart_Details::hadLoadError() {
    return m_has_load_error;
};
bool Cart_Details::hadLoadWarning() {
    return m_has_load_warning;
};
std::string Cart_Details::getFirstError() {
    if (m_first_error == "") {
        return m_first_warning;
    }else {
        return m_first_error;
    }
};

std::vector<std::string> Cart_Details::getCartDetails() {
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
std::vector<EntryColor> Cart_Details::getCartDetailColors() {
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
    IME{ false },
    IME_Delayed{ false },

    m_in_boot_rom_internal{ true },
    m_rom_bank_1{ 0 },
    m_rom_bank_2{ 1 },
    m_ram_bank{ 0 },

    m_mbc_type{ 0 },
    m_cart_has_ram{ false },
    m_cart_has_battery{ false },
    m_cart_has_timer{ false },
    m_cart_has_rumble{ false },

    m_num_rom_banks{ 0 },
    m_num_ram_banks{ 0 },

    m_latched_addr{ 0 },
    m_bus_latched{ false },
    m_open_bus{ 0xff }
{
    PowerCycle();
};

void Memory_Handler::PowerCycle() { // This is extreme...
    std::memset(m_RomBanks, 0, sizeof(m_RomBanks));
    std::memset(m_RamBanks, 0, sizeof(m_RamBanks));
    Reset();

    m_in_boot_rom_internal = true;

    m_rom_bank_1 = 0;
    m_rom_bank_2 = 1;
    m_ram_bank = 0;

    m_mbc_type = 0;
    m_cart_has_ram = false;
    m_cart_has_battery = false;
    m_cart_has_timer = false;
    m_cart_has_rumble = false;

    m_num_rom_banks = 0;
    m_num_ram_banks = 0;
};
void Memory_Handler::Reset() {
    std::memset(X8000_VRAM,     0, sizeof(X8000_VRAM));
    std::memset(XC000_WORK_RAM, 0, sizeof(XC000_WORK_RAM));
    std::memset(XFE00_OAM,      0, sizeof(XFE00_OAM));
    std::memset(XFF00_IO_REGS,  0, sizeof(XFF00_IO_REGS));
    std::memset(XFF80_HRAM,     0, sizeof(XFF80_HRAM));
    XFFFF_IE = 0;
    IME = false;
    IME_Delayed = false;
};

void Memory_Handler::interpret_cartridge_type(uint8_t cart_type, Cart_Details &cart_details) {
    switch (cart_type) {
        case 0x00: // ROM Only
            cart_details.m_cart_name = "ROM Only";
            m_mbc_type = 0;
            break;
        case 0x01: // MBC1
            cart_details.m_cart_name = "MBC1";
            m_mbc_type = 1;
            break;
        case 0x02: // MBC1 + RAM
            cart_details.m_cart_name = "MBC1 + RAM";
            m_mbc_type = 1;
            m_cart_has_ram = true;
            break;
        case 0x03: // MBC1 + RAM + BATTERY
            cart_details.m_cart_name = "MBC1 + RAM + BATTERY";
            m_mbc_type = 1;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x05: // MBC2
            cart_details.m_cart_name = "MBC2";
            m_mbc_type = 2;
            // m_cart_has_ram = true;
            break;
        case 0x06: // MBC2 + Battery
            cart_details.m_cart_name = "MBC2 + Battery";
            m_mbc_type = 2;
            // m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x08: // ROM + RAM (Unused Type)
#ifdef DEBUG_LOGGING
            printf("WARNING: Memory Setup: Cartridge Uses Type 'ROM + RAM' Never Used In Licensed Games\n");
#endif
            if (cart_details.m_first_warning == "") {
                cart_details.m_first_warning = "Cartridge Type 'ROM + RAM' Never Used In Licensed Games";
            }
            cart_details.m_cart_name_color = COLOR_ORANGE;
            // m_has_load_error = true;
            cart_details.m_has_load_warning = true;
            cart_details.m_cart_name = "ROM + RAM (Unused Type)";
            m_mbc_type = 0;
            m_cart_has_ram = true;
            break;
        case 0x09: // ROM + RAM + Battery (Unused Type)
#ifdef DEBUG_LOGGING
            printf("WARNING: Memory Setup: Cartridge Uses Type 'ROM + RAM + Battery' Never Used In Licensed Games\n");
#endif
            if (cart_details.m_first_warning == "") {
                cart_details.m_first_warning = "Cartridge Type 'ROM + RAM' Never Used In Licensed Games";
            }
            cart_details.m_cart_name_color = COLOR_ORANGE;
            // m_has_load_error = true;
            cart_details.m_has_load_warning = true;
            cart_details.m_cart_name = "ROM + RAM + Battery (Unused Type)";
            m_mbc_type = 0;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x0b: // MMM01
            cart_details.m_cart_name = "MMM01";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01' Not Supported\n");
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'MMM01'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
        case 0x0c: // MMM01 + RAM
            cart_details.m_cart_name = "MMM01 + RAM";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01 + RAM' Not Supported\n");
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'MMM01'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
        case 0x0d: // MMM01 + RAM + Battery
            cart_details.m_cart_name = "MMM01 + RAM + Battery";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Type 'MMM01 + RAM + Battery' Not Supported\n");
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'MMM01'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
        case 0x0f: // MBC3 + Timer + Battery
            cart_details.m_cart_name = "MBC3 + Timer + Battery";
            m_mbc_type = 3;
            m_cart_has_battery = true;
            m_cart_has_timer = true;
            break;
        case 0x10: // MBC3 + Timer + RAM + Battery
            cart_details.m_cart_name = "MBC3 + Timer + RAM + Battery";
            m_mbc_type = 3;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            m_cart_has_timer = true;
            break;
        case 0x11: // MBC3
            cart_details.m_cart_name = "MBC3";
            m_mbc_type = 3;
            break;
        case 0x12: // MBC3 + RAM
            cart_details.m_cart_name = "MBC3 + RAM";
            m_mbc_type = 3;
            m_cart_has_ram = true;
            break;
        case 0x13: // MBC3 + RAM + Battery
            cart_details.m_cart_name = "MBC3 + RAM + Battery";
            m_mbc_type = 3;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x19: // MBC5
            cart_details.m_cart_name = "MBC5";
            m_mbc_type = 5;
            break;
        case 0x1a: // MBC5 + RAM
            cart_details.m_cart_name = "MBC5 + RAM";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            break;
        case 0x1b: // MBC5 + RAM + Battery
            cart_details.m_cart_name = "MBC5 + RAM + Battery";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0x1c: // MBC5 + Rumble
            cart_details.m_cart_name = "MBC5 + Rumble";
            m_mbc_type = 5;
            m_cart_has_rumble = true;
            break;
        case 0x1d: // MBC5 + Rumble + RAM
            cart_details.m_cart_name = "MBC5 + Rumble + RAM";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            m_cart_has_rumble = true;
            break;
        case 0x1e: // MBC5 + Rumble + RAM + Battery
            cart_details.m_cart_name = "MBC5 + Rumble + RAM + Battery";
            m_mbc_type = 5;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            m_cart_has_rumble = true;
            break;
        case 0x20: // MBC6
            cart_details.m_cart_name = "MBC6";
            m_mbc_type = 6;
            break;
        case 0x22: // MBC7 + Sensor + Rumble + RAM + Battery
            cart_details.m_cart_name = "MBC7 + Sensor + Rumble + RAM + Battery";
            m_mbc_type = 7;
            m_cart_has_ram = true;
            m_cart_has_battery = true;
            break;
        case 0xfc: // Pocket Camera
            cart_details.m_cart_name = "Pocket Camera";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Type 'Pocket Camera' Not Supported\n");
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'Pocket Camera'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
        case 0xfd: // Bandai Tama5
            cart_details.m_cart_name = "Bandai Tama5";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Type 'Bandai Tama5' Not Supported\n");
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'Bandai Tama5'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
        case 0xfe: // HuC3
            cart_details.m_cart_name = "HuC3";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Type 'HuC3' Not Supported\n");
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'HuC3'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
        case 0xff: // HuC1 + RAM + Battery
            cart_details.m_cart_name = "HuC1 + RAM + Battery";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Type 'HuC1 + RAM + Battery' Not Supported\n");
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'HuC1'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
        default:
            cart_details.m_cart_name = "Unknown";
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized Type '%i'\n", cart_type);
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Unsupported Cartridge Type 'Unknown'";
            }
            cart_details.m_cart_name_color = COLOR_RED;
            cart_details.m_has_load_error = true;
            break;
    }
};
void Memory_Handler::interpret_rom_size_type(uint8_t rom_size, Cart_Details &cart_details) {
    switch (rom_size) {
        case 0x00:
            m_num_rom_banks = 2;
            cart_details.m_rom_size_name = "No ROM Banking, 32 KiB";
            break;
        case 0x01:
            m_num_rom_banks = 4;
            cart_details.m_rom_size_name = "4 ROM Banks, 64 KiB";
            break;
        case 0x02:
            m_num_rom_banks = 8;
            cart_details.m_rom_size_name = "8 ROM Banks, 128 KiB";
            break;
        case 0x03:
            m_num_rom_banks = 16;
            cart_details.m_rom_size_name = "16 ROM Banks, 256 KiB";
            break;
        case 0x04:
            m_num_rom_banks = 32;
            cart_details.m_rom_size_name = "32 ROM Banks, 512 KiB";
            break;
        case 0x05:
            m_num_rom_banks = 64;
            cart_details.m_rom_size_name = "64 ROM Banks, 1 MiB";
            break;
        case 0x06:
            m_num_rom_banks = 128;
            cart_details.m_rom_size_name = "128 ROM Banks, 2 MiB";
            break;
        case 0x07:
            m_num_rom_banks = 256;
            cart_details.m_rom_size_name = "256 ROM Banks, 4 MiB";
            break;
        case 0x08:
            m_num_rom_banks = 512;
            cart_details.m_rom_size_name = "512 ROM Banks, 8 MiB";
            break;
        case 0x52: // Only listed in unofficial docs
#ifdef DEBUG_LOGGING
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '72 banks'\n");
#endif
            if (cart_details.m_first_warning == "") {
                cart_details.m_first_warning = "ROM Size '72 banks' Not Used In Licensed Games";
            }
            cart_details.m_rom_size_name_color = COLOR_ORANGE;
            // cart_details.m_has_load_error = true;
            cart_details.m_has_load_warning = true;
            m_num_rom_banks = 72;
            cart_details.m_rom_size_name = "72 ROM Banks, 1.1 MiB (Unused Type)";
            break;
        case 0x53: // Only listed in unofficial docs
#ifdef DEBUG_LOGGING
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '80 banks'\n");
#endif
            if (cart_details.m_first_warning == "") {
                cart_details.m_first_warning = "ROM Size '80 banks' Not Used In Licensed Games";
            }
            cart_details.m_rom_size_name_color = COLOR_ORANGE;
            // cart_details.m_has_load_error = true;
            cart_details.m_has_load_warning = true;
            m_num_rom_banks = 80;
            cart_details.m_rom_size_name = "80 ROM Banks, 1.2 MiB (Unused Type)";
            break;
        case 0x54: // Only listed in unofficial docs
#ifdef DEBUG_LOGGING
            printf("WARNING: Memory Setup: Uses Unused Cartridge ROM Size '96 banks'\n");
#endif
            if (cart_details.m_first_warning == "") {
                cart_details.m_first_warning = "ROM Size '96 banks' Not Used In Licensed Games";
            }
            cart_details.m_rom_size_name_color = COLOR_ORANGE;
            // cart_details.m_has_load_error = true;
            cart_details.m_has_load_warning = true;
            cart_details.m_rom_size_name = "96 ROM Banks, 1.5 MiB (Unused Type)";
            m_num_rom_banks = 96;
            break;
        default:
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized ROM Size '%i'\n", rom_size);
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Cartridge ROM Size Unrecognized";
            }
            cart_details.m_rom_size_name_color = COLOR_RED;
            cart_details.m_rom_size_name = "Unknown";
            cart_details.m_has_load_error = true;
            break;
    }
};
void Memory_Handler::interpret_ram_size_type(uint8_t ram_size, Cart_Details &cart_details) {
    switch (ram_size) {
        case 0x00:
            m_num_rom_banks = 0;
            cart_details.m_ram_size_name = "No RAM";
            break;
        case 0x01: // Only listed in unofficial docs
#ifdef DEBUG_LOGGING
            printf("WARNING: Memory Setup: Uses Unused Cartridge RAM Size '2 KiB'\n");
#endif
            if (cart_details.m_first_warning == "") {
                cart_details.m_first_warning = "RAM Size '2 KiB' Not Used In Licensed Games";
            }
            cart_details.m_ram_size_name_color = COLOR_ORANGE;
            // cart_details.m_has_load_error = true;
            cart_details.m_has_load_warning = true;
            m_num_rom_banks = 1;
            cart_details.m_ram_size_name = "(1) RAM Bank, 2 KiB (Unused Type)";
            break;
        case 0x02:
            m_num_rom_banks = 1;
            cart_details.m_ram_size_name = "1 RAM Bank, 8 KiB";
            break;
        case 0x03:
            m_num_rom_banks = 4;
            cart_details.m_ram_size_name = "4 RAM Bank, 32 KiB";
            break;
        case 0x04:
            m_num_rom_banks = 16;
            cart_details.m_ram_size_name = "16 RAM Bank, 128 KiB";
            break;
        case 0x05:
            m_num_rom_banks = 8;
            cart_details.m_ram_size_name = "8 RAM Bank, 64 KiB";
            break;
        default:
#ifdef DEBUG_LOGGING
            printf("ERROR: Memory Setup: Cartridge Uses Unrecognized RAM Size '%i'\n", ram_size);
#endif
            if (cart_details.m_first_error == "") {
                cart_details.m_first_error = "Cartridge RAM Size Unrecognized";
            }
            cart_details.m_ram_size_name_color = COLOR_RED;
            cart_details.m_ram_size_name = "Unknown";
            cart_details.m_has_load_error = true;
            break;
    }
};
void Memory_Handler::interpret_licensee_code(uint8_t old_code, uint8_t new_code_1, uint8_t new_code_2, Cart_Details &cart_details) {
    if (old_code != 0x33) {
        switch (old_code) {
            case 0x00: cart_details.m_licensee = "None"; break;
            case 0x01: cart_details.m_licensee = "Nintendo"; break;
            case 0x08: cart_details.m_licensee = "Capcom"; break;
            case 0x09: cart_details.m_licensee = "HOT-B"; break;
            case 0x0A: cart_details.m_licensee = "Jaleco"; break;
            case 0x0B: cart_details.m_licensee = "Coconuts Japan"; break;
            case 0x0C: cart_details.m_licensee = "Elite Systems"; break;
            case 0x13: cart_details.m_licensee = "EA (Electronic Arts)"; break;
            case 0x18: cart_details.m_licensee = "Hudson Soft"; break;
            case 0x19: cart_details.m_licensee = "ITC Entertainment"; break;
            case 0x1A: cart_details.m_licensee = "Yanoman"; break;
            case 0x1D: cart_details.m_licensee = "Japan Clary"; break;
            case 0x1F: cart_details.m_licensee = "Virgin Games Ltd."; break;
            case 0x24: cart_details.m_licensee = "PCM Complete"; break;
            case 0x25: cart_details.m_licensee = "San-X"; break;
            case 0x28: cart_details.m_licensee = "Kemco"; break;
            case 0x29: cart_details.m_licensee = "SETA Corporation"; break;
            case 0x30: cart_details.m_licensee = "Infogrames"; break;
            case 0x31: cart_details.m_licensee = "Nintendo"; break;
            case 0x32: cart_details.m_licensee = "Bandai"; break;
            // case 0x33: cart_details.m_licensee = "Use New Licensee Code"; break;
            case 0x34: cart_details.m_licensee = "Konami"; break;
            case 0x35: cart_details.m_licensee = "HectorSoft"; break;
            case 0x38: cart_details.m_licensee = "Capcom"; break;
            case 0x39: cart_details.m_licensee = "Banpresto"; break;
            case 0x3C: cart_details.m_licensee = "Entertainment Interactive"; break;
            case 0x3E: cart_details.m_licensee = "Gremlin"; break;
            case 0x41: cart_details.m_licensee = "Ubi Soft"; break;
            case 0x42: cart_details.m_licensee = "Atlus"; break;
            case 0x44: cart_details.m_licensee = "Malibu Interactive"; break;
            case 0x46: cart_details.m_licensee = "Angel"; break;
            case 0x47: cart_details.m_licensee = "Spectrum HoloByte"; break;
            case 0x49: cart_details.m_licensee = "Irem"; break;
            case 0x4A: cart_details.m_licensee = "Virgin Games Ltd."; break;
            case 0x4D: cart_details.m_licensee = "Malibu Interactive"; break;
            case 0x4F: cart_details.m_licensee = "U.S. Gold"; break;
            case 0x50: cart_details.m_licensee = "Absolute"; break;
            case 0x51: cart_details.m_licensee = "Acclaim Entertainment"; break;
            case 0x52: cart_details.m_licensee = "Activision"; break;
            case 0x53: cart_details.m_licensee = "Sammy USA Corporation"; break;
            case 0x54: cart_details.m_licensee = "GameTek"; break;
            case 0x55: cart_details.m_licensee = "Park Place"; break;
            case 0x56: cart_details.m_licensee = "LJN"; break;
            case 0x57: cart_details.m_licensee = "Matchbox"; break;
            case 0x59: cart_details.m_licensee = "Milton Bradley Company"; break;
            case 0x5A: cart_details.m_licensee = "Mindscape"; break;
            case 0x5B: cart_details.m_licensee = "Romstar"; break;
            case 0x5C: cart_details.m_licensee = "Naxat Soft"; break;
            case 0x5D: cart_details.m_licensee = "Tradewest"; break;
            case 0x60: cart_details.m_licensee = "Titus Interactive"; break;
            case 0x61: cart_details.m_licensee = "Virgin Games Ltd."; break;
            case 0x67: cart_details.m_licensee = "Ocean Software"; break;
            case 0x69: cart_details.m_licensee = "EA (Electronic Arts)"; break;
            case 0x6E: cart_details.m_licensee = "Elite Systems"; break;
            case 0x6F: cart_details.m_licensee = "Electro Brain"; break;
            case 0x70: cart_details.m_licensee = "Infogrames"; break;
            case 0x71: cart_details.m_licensee = "Interplay Entertainment"; break;
            case 0x72: cart_details.m_licensee = "Broderbund"; break;
            case 0x73: cart_details.m_licensee = "Sculptured Software"; break;
            case 0x75: cart_details.m_licensee = "The Sales Curve Limited"; break;
            case 0x78: cart_details.m_licensee = "THQ"; break;
            case 0x79: cart_details.m_licensee = "Accolade"; break;
            case 0x7A: cart_details.m_licensee = "Triffix Entertainment"; break;
            case 0x7C: cart_details.m_licensee = "MicroProse"; break;
            case 0x7F: cart_details.m_licensee = "Kemco"; break;
            case 0x80: cart_details.m_licensee = "Misawa Entertainment"; break;
            case 0x83: cart_details.m_licensee = "LOZC G."; break;
            case 0x86: cart_details.m_licensee = "Tokuma Shoten"; break;
            case 0x8B: cart_details.m_licensee = "Bullet-Proof Software"; break;
            case 0x8C: cart_details.m_licensee = "Vic Tokai Corp."; break;
            case 0x8E: cart_details.m_licensee = "Ape Inc."; break;
            case 0x8F: cart_details.m_licensee = "I’Max"; break;
            case 0x91: cart_details.m_licensee = "Chunsoft Co."; break;
            case 0x92: cart_details.m_licensee = "Video System"; break;
            case 0x93: cart_details.m_licensee = "Tsubaraya Productions"; break;
            case 0x95: cart_details.m_licensee = "Varie"; break;
            case 0x96: cart_details.m_licensee = "Yonezawa / S’Pal"; break;
            case 0x97: cart_details.m_licensee = "Kemco"; break;
            case 0x99: cart_details.m_licensee = "Arc"; break;
            case 0x9A: cart_details.m_licensee = "Nihon Bussan"; break;
            case 0x9B: cart_details.m_licensee = "Tecmo"; break;
            case 0x9C: cart_details.m_licensee = "Imagineer"; break;
            case 0x9D: cart_details.m_licensee = "Banpresto"; break;
            case 0x9F: cart_details.m_licensee = "Nova"; break;
            case 0xA1: cart_details.m_licensee = "Hori Electric"; break;
            case 0xA2: cart_details.m_licensee = "Bandai"; break;
            case 0xA4: cart_details.m_licensee = "Konami"; break;
            case 0xA6: cart_details.m_licensee = "Kawada"; break;
            case 0xA7: cart_details.m_licensee = "Takara"; break;
            case 0xA9: cart_details.m_licensee = "Technos Japan"; break;
            case 0xAA: cart_details.m_licensee = "Broderbund"; break;
            case 0xAC: cart_details.m_licensee = "Toei Animation"; break;
            case 0xAD: cart_details.m_licensee = "Toho"; break;
            case 0xAF: cart_details.m_licensee = "Namco"; break;
            case 0xB0: cart_details.m_licensee = "Acclaim Entertainment"; break;
            case 0xB1: cart_details.m_licensee = "ASCII Corporation / Nexsoft"; break;
            case 0xB2: cart_details.m_licensee = "Bandai"; break;
            case 0xB4: cart_details.m_licensee = "Square Enix"; break;
            case 0xB6: cart_details.m_licensee = "HAL Laboratory"; break;
            case 0xB7: cart_details.m_licensee = "SNK"; break;
            case 0xB9: cart_details.m_licensee = "Pony Canyon"; break;
            case 0xBA: cart_details.m_licensee = "Culture Brain"; break;
            case 0xBB: cart_details.m_licensee = "Sunsoft"; break;
            case 0xBD: cart_details.m_licensee = "Sony Imagesoft"; break;
            case 0xBF: cart_details.m_licensee = "Sammy Corporation"; break;
            case 0xC0: cart_details.m_licensee = "Taito"; break;
            case 0xC2: cart_details.m_licensee = "Kemco"; break;
            case 0xC3: cart_details.m_licensee = "Square"; break;
            case 0xC4: cart_details.m_licensee = "Tokuma Shoten"; break;
            case 0xC5: cart_details.m_licensee = "Data East"; break;
            case 0xC6: cart_details.m_licensee = "Tonkin House"; break;
            case 0xC8: cart_details.m_licensee = "Koei"; break;
            case 0xC9: cart_details.m_licensee = "UFL"; break;
            case 0xCA: cart_details.m_licensee = "Ultra Games"; break;
            case 0xCB: cart_details.m_licensee = "VAP, Inc."; break;
            case 0xCC: cart_details.m_licensee = "Use Corporation"; break;
            case 0xCD: cart_details.m_licensee = "Meldac"; break;
            case 0xCE: cart_details.m_licensee = "Pony Canyon"; break;
            case 0xCF: cart_details.m_licensee = "Angel"; break;
            case 0xD0: cart_details.m_licensee = "Taito"; break;
            case 0xD1: cart_details.m_licensee = "SOFEL"; break;
            case 0xD2: cart_details.m_licensee = "Quest"; break;
            case 0xD3: cart_details.m_licensee = "Sigma Enterprises"; break;
            case 0xD4: cart_details.m_licensee = "ASK Kodansha Co."; break;
            case 0xD6: cart_details.m_licensee = "Naxat Soft"; break;
            case 0xD7: cart_details.m_licensee = "Copya System"; break;
            case 0xD9: cart_details.m_licensee = "Banpresto"; break;
            case 0xDA: cart_details.m_licensee = "Tomy"; break;
            case 0xDB: cart_details.m_licensee = "LJN"; break;
            case 0xDD: cart_details.m_licensee = "Nippon Computer Systems"; break;
            case 0xDE: cart_details.m_licensee = "Human Entertainment"; break;
            case 0xDF: cart_details.m_licensee = "Altron"; break;
            case 0xE0: cart_details.m_licensee = "Jaleco"; break;
            case 0xE1: cart_details.m_licensee = "Towa Chiki"; break;
            case 0xE2: cart_details.m_licensee = "Yutaka"; break;
            case 0xE3: cart_details.m_licensee = "Varie"; break;
            case 0xE5: cart_details.m_licensee = "Epoch"; break;
            case 0xE7: cart_details.m_licensee = "Athena"; break;
            case 0xE8: cart_details.m_licensee = "Asmik Ace Entertainment"; break;
            case 0xE9: cart_details.m_licensee = "Natsume"; break;
            case 0xEA: cart_details.m_licensee = "King Records"; break;
            case 0xEB: cart_details.m_licensee = "Atlus"; break;
            case 0xEC: cart_details.m_licensee = "Epic / Sony Records"; break;
            case 0xEE: cart_details.m_licensee = "IGS"; break;
            case 0xF0: cart_details.m_licensee = "A Wave"; break;
            case 0xF3: cart_details.m_licensee = "Extreme Entertainment"; break;
            case 0xFF: cart_details.m_licensee = "LJN"; break;
            default: cart_details.m_licensee = "Unknown"; break;
        }
    }else {
        if (!std::isprint(new_code_1) || !std::isprint(new_code_2)) {
            cart_details.m_licensee = "Invalid New Licensee Code";
            return;
        }
        switch (combine_chars(static_cast<char>(new_code_1), static_cast<char>(new_code_2))) {
            case combine_chars('0','0'): cart_details.m_licensee = "None"; break;
            case combine_chars('0','1'): cart_details.m_licensee = "Nintendo Research & Development 1"; break;
            case combine_chars('0','8'): cart_details.m_licensee = "Capcom"; break;
            case combine_chars('1','3'): cart_details.m_licensee = "EA (Electronic Arts)"; break;
            case combine_chars('1','8'): cart_details.m_licensee = "Hudson Soft"; break;
            case combine_chars('1','9'): cart_details.m_licensee = "B-AI"; break;
            case combine_chars('2','0'): cart_details.m_licensee = "KSS"; break;
            case combine_chars('2','2'): cart_details.m_licensee = "Planning Office WADA"; break;
            case combine_chars('2','4'): cart_details.m_licensee = "PCM Complete"; break;
            case combine_chars('2','5'): cart_details.m_licensee = "San-X"; break;
            case combine_chars('2','8'): cart_details.m_licensee = "Kemco"; break;
            case combine_chars('2','9'): cart_details.m_licensee = "SETA Corporation"; break;
            case combine_chars('3','0'): cart_details.m_licensee = "Viacom"; break;
            case combine_chars('3','1'): cart_details.m_licensee = "Nintendo"; break;
            case combine_chars('3','2'): cart_details.m_licensee = "Bandai"; break;
            case combine_chars('3','3'): cart_details.m_licensee = "Ocean Software / Acclaim Entertainment"; break;
            case combine_chars('3','4'): cart_details.m_licensee = "Konami"; break;
            case combine_chars('3','5'): cart_details.m_licensee = "HectorSoft"; break;
            case combine_chars('3','7'): cart_details.m_licensee = "Taito"; break;
            case combine_chars('3','8'): cart_details.m_licensee = "Hudson Soft"; break;
            case combine_chars('3','9'): cart_details.m_licensee = "Banpresto"; break;
            case combine_chars('4','1'): cart_details.m_licensee = "Ubi Soft"; break;
            case combine_chars('4','2'): cart_details.m_licensee = "Atlus"; break;
            case combine_chars('4','4'): cart_details.m_licensee = "Malibu Interactive"; break;
            case combine_chars('4','6'): cart_details.m_licensee = "Angel"; break;
            case combine_chars('4','7'): cart_details.m_licensee = "Bullet-Proof Software"; break;
            case combine_chars('4','9'): cart_details.m_licensee = "Irem"; break;
            case combine_chars('5','0'): cart_details.m_licensee = "Absolute"; break;
            case combine_chars('5','1'): cart_details.m_licensee = "Acclaim Entertainment"; break;
            case combine_chars('5','2'): cart_details.m_licensee = "Activision"; break;
            case combine_chars('5','3'): cart_details.m_licensee = "Sammy USA Corporation"; break;
            case combine_chars('5','4'): cart_details.m_licensee = "Konami"; break;
            case combine_chars('5','5'): cart_details.m_licensee = "Hi Tech Expressions"; break;
            case combine_chars('5','6'): cart_details.m_licensee = "LJN"; break;
            case combine_chars('5','7'): cart_details.m_licensee = "Matchbox"; break;
            case combine_chars('5','8'): cart_details.m_licensee = "Mattel"; break;
            case combine_chars('5','9'): cart_details.m_licensee = "Milton Bradley Company"; break;
            case combine_chars('6','0'): cart_details.m_licensee = "Titus Interactive"; break;
            case combine_chars('6','1'): cart_details.m_licensee = "Virgin Games Ltd."; break;
            case combine_chars('6','4'): cart_details.m_licensee = "Lucasfilm Games"; break;
            case combine_chars('6','7'): cart_details.m_licensee = "Ocean Software"; break;
            case combine_chars('6','9'): cart_details.m_licensee = "EA (Electronic Arts)"; break;
            case combine_chars('7','0'): cart_details.m_licensee = "Infogrames"; break;
            case combine_chars('7','1'): cart_details.m_licensee = "Interplay Entertainment"; break;
            case combine_chars('7','2'): cart_details.m_licensee = "Broderbund"; break;
            case combine_chars('7','3'): cart_details.m_licensee = "Sculptured Software"; break;
            case combine_chars('7','5'): cart_details.m_licensee = "The Sales Curve Limited"; break;
            case combine_chars('7','8'): cart_details.m_licensee = "THQ"; break;
            case combine_chars('7','9'): cart_details.m_licensee = "Accolade"; break;
            case combine_chars('8','0'): cart_details.m_licensee = "Misawa Entertainment"; break;
            case combine_chars('8','3'): cart_details.m_licensee = "LOZC G."; break;
            case combine_chars('8','6'): cart_details.m_licensee = "Tokuma Shoten"; break;
            case combine_chars('8','7'): cart_details.m_licensee = "Tsukuda Original"; break;
            case combine_chars('9','1'): cart_details.m_licensee = "Chunsoft Co."; break;
            case combine_chars('9','2'): cart_details.m_licensee = "Video System"; break;
            case combine_chars('9','3'): cart_details.m_licensee = "Ocean Software / Acclaim Entertainment"; break;
            case combine_chars('9','5'): cart_details.m_licensee = "Varie"; break;
            case combine_chars('9','6'): cart_details.m_licensee = "Yonezawa / S’Pal"; break;
            case combine_chars('9','7'): cart_details.m_licensee = "Kaneko"; break;
            case combine_chars('9','9'): cart_details.m_licensee = "Pack-In-Video"; break;
            case combine_chars('9','H'): cart_details.m_licensee = "Bottom Up"; break;
            case combine_chars('A','4'): cart_details.m_licensee = "Konami (Yu-Gi-Oh!)"; break;
            case combine_chars('B','L'): cart_details.m_licensee = "MTO"; break;
            case combine_chars('D','K'): cart_details.m_licensee = "Kodansha"; break;
            default: cart_details.m_licensee = "Unknown"; break;
        }
    }
};
Cart_Details Memory_Handler::Initialize(std::vector<uint8_t>* rom, bool use_boot_rom) {
    if (!use_boot_rom) {
        _Set(0xff00, 0xcf);
        _Set(0xff01, 0x00);
        _Set(0xff02, 0x7e);
        _Set(0xff04, 0xab);
        _Set(0xff05, 0x00);
        _Set(0xff06, 0x00);
        _Set(0xff07, 0xf8);
        _Set(0xff0f, 0xe1);
        _Set(0xff10, 0x80);
        _Set(0xff11, 0xbf);
        _Set(0xff12, 0xf3);
        _Set(0xff13, 0xff);
        _Set(0xff14, 0xbf);
        _Set(0xff16, 0x3f);
        _Set(0xff17, 0x00);
        _Set(0xff18, 0xff);
        _Set(0xff19, 0xbf);
        _Set(0xff1a, 0x7f);
        _Set(0xff1b, 0xff);
        _Set(0xff1c, 0x9f);
        _Set(0xff1d, 0xff);
        _Set(0xff1e, 0xbf);
        _Set(0xff20, 0xff);
        _Set(0xff21, 0x00);
        _Set(0xff22, 0x00);
        _Set(0xff23, 0xfb);
        _Set(0xff24, 0x77);
        _Set(0xff25, 0xf3);
        _Set(0xff26, 0xf1);
        _Set(0xff40, 0x91);
        _Set(0xff41, 0x81);
        _Set(0xff42, 0x00);
        _Set(0xff43, 0x00);
        _Set(0xff44, 0x00);
        _Set(0xff45, 0x00);
        _Set(0xff46, 0xff);
        _Set(0xff47, 0xfc);
        _Set(0xff48, 0x00);
        _Set(0xff49, 0x00);
        _Set(0xff4a, 0x00);
        _Set(0xff4b, 0x00);
        _Set(0xffff, 0x00);

        IME = true;
    }

    Cart_Details cart_details = Cart_Details();

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
#ifdef DEBUG_LOGGING
        printf("WARNING: Memory Setup: CGB Cartridge In Compatability Mode\n");
#endif
        if (cart_details.m_first_warning == "") {
            cart_details.m_first_warning = "CGB Cartridge In Compatability Mode";
        }
        cart_details.m_is_cgb_cart_color = COLOR_ORANGE;
        // cart_details.m_has_load_error = true;
        cart_details.m_has_load_warning = true;
        cart_details.m_is_cgb_cart = "CGB Cartridge with Compatability";
    }else if (cgb_flag == 0xc0) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Setup: CGB Cartridge, Cannot Be Run\n");
#endif
        if (cart_details.m_first_error == "") {
            cart_details.m_first_error = "CGB Cartridge, Cannot Be Run";
        }
        cart_details.m_is_cgb_cart_color = COLOR_RED;
        cart_details.m_has_load_error = true;
        cart_details.m_is_cgb_cart = "Uncompatible CGB Cartridge";
    }else {
        cart_details.m_is_cgb_cart = "DMG Cartridge";
    }

    // Checksum
    uint8_t checksum = 0;
    for (uint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - rom->at(address) - 1;
    }
    if (checksum == header_checksum) {
#ifdef DEBUG_LOGGING
        printf("COMPLETE: Memory Setup: Header Checksum Passed\n");
#endif
        cart_details.m_passes_checksum = true;
    }else {
#ifdef DEBUG_LOGGING
        printf("WARNING: Memory Setup: Header Checksum Failed\n");
#endif
        if (cart_details.m_first_warning == "") {
            cart_details.m_first_warning = "Header Checksum Failed";
        }
        cart_details.m_passes_checksum_color = COLOR_ORANGE;
        // cart_details.m_has_load_error = true;
        cart_details.m_has_load_warning = true;
        cart_details.m_passes_checksum = false;
    }

    // Nintendo Logo
    bool logo_matches = true;
    for (uint16_t address = 0x0104; address <= 0x0133; address++) {
        if (rom->at(address) != Nintendo_Logo[address - 0x0104]) {
            logo_matches = false;
        }
    }
    if (logo_matches) {
#ifdef DEBUG_LOGGING
        printf("COMPLETE: Memory Setup: Nintendo Logo Matches\n");
#endif
        cart_details.m_valid_logo = true;
    }else {
#ifdef DEBUG_LOGGING
        printf("WARNING: Memory Setup: Nintendo Logo Does Not Match\n");
#endif
        if (cart_details.m_first_warning == "") {
            cart_details.m_first_warning = "Nintendo Logo Does Not Match";
        }
        cart_details.m_valid_logo_color = COLOR_ORANGE;
        // cart_details.m_has_load_error = true;
        cart_details.m_has_load_warning = true;
        cart_details.m_valid_logo = false;
    }

    interpret_cartridge_type(cart_type, cart_details);
    interpret_rom_size_type(rom_size, cart_details);
    interpret_ram_size_type(ram_size, cart_details);
    interpret_licensee_code(old_licensee_code, new_licensee_code_1, new_licensee_code_2, cart_details);
    cart_details.m_romname = readROMName(rom);

    cart_details.m_mbc_type = m_mbc_type;
    cart_details.m_cart_has_ram = m_cart_has_ram;
    cart_details.m_cart_has_battery = m_cart_has_battery;
    cart_details.m_cart_has_timer = m_cart_has_timer;
    cart_details.m_cart_has_rumble = m_cart_has_rumble;

    cart_details.m_num_rom_banks = m_num_rom_banks;
    cart_details.m_num_ram_banks = m_num_ram_banks;
    
    constexpr size_t ROM_BANK_SIZE = 0x4000;
    for (int bank = 0; bank < m_num_rom_banks; ++bank) {
        std::memcpy(
            m_RomBanks[bank],
            rom->data() + bank * ROM_BANK_SIZE,
            ROM_BANK_SIZE
        );
    }

    // Read Saved RAM WORKING HERE

#ifdef DEBUG_LOGGING
    if (!cart_details.m_has_load_error) {
        if (!cart_details.m_has_load_warning) {
            printf("COMPLETE: Memory Setup: ROM Loaded\n");
        }else {
            printf("COMPLETE: Memory Setup: ROM Loaded With Warning\n");
        }
        printf("ROM Name: %s\n", cart_details.m_romname.c_str());
        printf("Cartridge Type: %i (%s)\n", cart_type, cart_details.m_cart_name.c_str());
        printf("ROM Size: %i (%s)\n", rom_size, cart_details.m_rom_size_name.c_str());
        printf("RAM Size: %i (%s)\n", ram_size, cart_details.m_ram_size_name.c_str());
    }else {
        printf("ERROR: Memory Setup: ROM Could Not Be Loaded\n");
    }
#endif

    return cart_details;
};

void Memory_Handler::latchBus(uint16_t addr) {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Bus Latched to "+to_b16_out(addr)+", ");
        }
    #endif
    if (!m_bus_latched) {
        m_latched_addr = addr;
        m_bus_latched = true;
    }else {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Handler: latchBus: Bus Cannot Be Latched Twice In One M Cycle\n");
#endif
    }
};
void Memory_Handler::freeBus() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Bus Freed, ");
        }
    #endif
    m_bus_latched = false;
};
uint8_t Memory_Handler::Read() {
    if (!m_bus_latched) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Handler: Read: Bus Not Latched\n");
        if (m_log_enable) {
            m_logfile.print("Tried to Read without Latching Bus, ");
        }
#endif
        return m_open_bus;
    }
    uint16_t address = m_latched_addr;
    uint8_t ret = m_open_bus;
    if (address < 0x4000) {
        if (m_in_boot_rom_internal && address < 0x0100) {
// #ifdef DEBUG_LOGGING
//             printf("BOOT ROM Read at addr %s\n", to_b16_out(address).c_str());
// #endif
            ret = Boot_ROM[address];
        }else {
            ret = m_RomBanks[m_rom_bank_1][address];
        }
    }else if (address < 0x8000) {
        ret = m_RomBanks[m_rom_bank_2][address - 0x4000];
    }else if (address < 0xa000) {
        ret = X8000_VRAM[address - 0x8000];
    }else if (address < 0xc000) {
        ret = m_RamBanks[m_ram_bank][address - 0xa000];
    }else if (address < 0xe000) {
        ret = XC000_WORK_RAM[address - 0xc000];
    }else if (address < 0xfe00) {
#ifdef DEBUG_LOGGING
        printf("WARNING: Memory Read: Echo RAM Read\n");
#endif
        ret = XC000_WORK_RAM[address - 0xe000];
    }else if (address < 0xfea0) {
        ret = XFE00_OAM[address - 0xfe00];
    }else if (address < 0xff00) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Read: Unusable Memory Read\n");
#endif
        ret = m_open_bus;
    }else if (address < 0xff80) {
        switch (address) {
            case 0xff00:
                ret = m_controllers.read_FF00();
                break;
            case 0xff01:
                ret = m_serial.read_FF01();
                break;
            case 0xff02:
                ret = m_serial.read_FF02();
                break;
            case 0xff04:
                ret = m_timer.read_FF04();
                break;
            case 0xff05:
                ret = m_timer.read_FF05();
                break;
            case 0xff06:
                ret = m_timer.read_FF06();
                break;
            case 0xff07:
                ret = m_timer.read_FF07();
                break;
            default:
                ret = XFF00_IO_REGS[address - 0xff00]; // Reading special registers...
                break;
        }
    }else if (address < 0xffff) {
        ret = XFF80_HRAM[address - 0xff80];
    }else {
        ret = XFFFF_IE;
    }
    m_open_bus = ret; // Update m_open_bus
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Read from "+to_b16_out(address)+" value "+to_b8_out(ret)+", ");
        }
    #endif
    return ret;
};
void Memory_Handler::Write(uint8_t value) {
    if (!m_bus_latched) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Handler: Write: Bus Not Latched\n");
        if (m_log_enable) {
            m_logfile.print("Tried to Write without Latching Bus, ");
        }
#endif
        return;
    }
    uint16_t address = m_latched_addr;
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Write to "+to_b16_out(address)+" value "+to_b8_out(value)+", ");
        }
    #endif
    m_open_bus = value;
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
#ifdef DEBUG_LOGGING
        printf("WARNING: Memory Read: Echo RAM Read\n");
#endif
        XC000_WORK_RAM[address - 0xe000] = value;
    }else if (address < 0xfea0) {
        XFE00_OAM[address - 0xfe00] = value;
    }else if (address < 0xff00) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Read: Unusable Memory Read\n");
#endif
    }else if (address < 0xff80) {
        switch (address) {
            case 0xff00:
                m_controllers.write_FF00(value);
                break;
            case 0xff01:
                m_serial.write_FF01(value);
                break;
            case 0xff02:
                m_serial.write_FF02(value);
                break;
            case 0xff04:
                m_timer.write_FF04(value);
                break;
            case 0xff05:
                m_timer.write_FF05(value);
                break;
            case 0xff06:
                m_timer.write_FF06(value);
                break;
            case 0xff07:
                m_timer.write_FF07(value);
                break;
            default:
                XFF00_IO_REGS[address - 0xff00] = value; // Reading special registers...
                break;
        }
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
#ifdef DEBUG_LOGGING
        printf("WARNING: Memory Read: Echo RAM Read\n");
#endif
        return XC000_WORK_RAM[address - 0xe000];
    }else if (address < 0xfea0) {
        return XFE00_OAM[address - 0xfe00];
    }else if (address < 0xff00) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Read: Unusable Memory Read\n");
#endif
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
#ifdef DEBUG_LOGGING
        printf("WARNING: Memory Read: Echo RAM Read\n");
#endif
        XC000_WORK_RAM[address - 0xe000] = value;
    }else if (address < 0xfea0) {
        XFE00_OAM[address - 0xfe00] = value;
    }else if (address < 0xff00) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Memory Read: Unusable Memory Read\n");
#endif
    }else if (address < 0xff80) {
        XFF00_IO_REGS[address - 0xff00] = value; // Reading special registers...
    }else if (address < 0xffff) {
        XFF80_HRAM[address - 0xff80] = value;
    }else {
        XFFFF_IE = value;
    }
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

void Memory_Handler::_Set_IME(bool value) {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Write to IME value "+std::to_string(value)+", ");
        }
    #endif
    IME = value;
    IME_Delayed = value; // So that Delayed doesn't undo the change
};
void Memory_Handler::_Set_IME_Delayed(bool value) {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Write to IME delayed value "+std::to_string(value)+", ");
        }
    #endif
    IME_Delayed = value;
};
void Memory_Handler::handleIME() {
    if (IME_Delayed != IME) {
        IME = IME_Delayed;
    }
};

bool Memory_Handler::_InterruptsPending() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Checked Pending Interrupts, ");
        }
    #endif
    return (_Get(0xff0f) & _Get(0xffff) & 0x1f) == 0;
};
bool Memory_Handler::_InterruptsEnabled() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Checked Interrupts Enabled, ");
        }
    #endif
    return IME;
};

