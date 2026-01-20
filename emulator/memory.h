#ifndef EMULATOR_MEMORY_
#define EMULATOR_MEMORY_

#include <cstdint>
#include <vector>
#include <string>
#include "../utils.h"

enum class Reg_u8 {
    A, F,
    B, // high
    C, // low
    D, E,
    H, L,
    SP_H, SP_L,
    PC_H, PC_L,
    temp
};
enum class Reg_u16 {
    AF,
    BC,
    DE,
    HL,
    SP,
    PC,
    temp16
};
enum class Reg_flag {
    Z, N, H, C
};

class Register_Handler {
    public:
        Register_Handler();

        [[nodiscard]] uint8_t get(Reg_u8 reg) const;
        void set(Reg_u8 reg, uint8_t value);
        [[nodiscard]] uint16_t get(Reg_u16 reg) const;
        void set(Reg_u16 reg, uint16_t value);
        [[nodiscard]] bool get(Reg_flag reg) const;
        void set(Reg_flag reg, bool value);

    private:
        uint8_t F;
        uint8_t A;
        uint8_t C; // low
        uint8_t B; // high
        uint8_t E;
        uint8_t D;
        uint8_t L;
        uint8_t H;
        uint16_t SP; // Stack Pointer
        uint16_t PC; // Program Counter

        uint8_t temp; // For use by opcodes
        uint16_t temp16;
};

class Cart_Details {
    public:
        Cart_Details();

        bool hadLoadError();
        bool hadLoadWarning();
        std::string getFirstError();

        std::vector<std::string> getCartDetails();
        std::vector<EntryColor> getCartDetailColors();

        bool m_has_load_error;
        bool m_has_load_warning;
        std::string m_first_error;
        std::string m_first_warning;

        std::string m_romname; // For display only
        std::string m_cart_name;
        std::string m_rom_size_name;
        std::string m_ram_size_name;
        std::string m_is_cgb_cart;
        std::string m_licensee;
        bool m_passes_checksum;
        bool m_valid_logo;
        EntryColor m_romname_color; // For display only
        EntryColor m_cart_name_color;
        EntryColor m_rom_size_name_color;
        EntryColor m_ram_size_name_color;
        EntryColor m_is_cgb_cart_color;
        EntryColor m_licensee_color;
        EntryColor m_passes_checksum_color;
        EntryColor m_valid_logo_color;

        int m_mbc_type; // Only emulate MBC cartridges, for now
        bool m_cart_has_ram;
        bool m_cart_has_battery;
        bool m_cart_has_timer;
        bool m_cart_has_rumble;

        int m_num_rom_banks;
        int m_num_ram_banks;
};


class Memory_Handler {
    public:
        Memory_Handler();

        void PowerCycle(); // Use with caution, wipes ROM as well
        void Reset(); // Only resets RAM (same as reset button)

        Cart_Details Initialize(std::vector<uint8_t>* rom, bool use_boot_rom);

        uint8_t Read(uint16_t address);
        void Write(uint16_t address, uint8_t data);
        uint8_t _Get(uint16_t address); // For setup/internal, raw output without checks
        void _Set(uint16_t address, uint8_t data); // For setup/internal, raw input without checks
        uint8_t PC_Eat_Byte(Register_Handler &regs);

    private:
        // uint8_t X0000_ROM_STATIC[0x4000]; // (Switchable) // Reference m_RomBanks directly
        // uint8_t X4000_ROM_BANK[0x4000]; // Switchable // Reference m_RomBanks directly
        uint8_t X8000_VRAM[0x2000];
        // uint8_t XA000_EXT_RAM[0x2000]; // (Switchable) // Reference m_RamBanks directly
        uint8_t XC000_WORK_RAM[0x2000]; // 2nd half only a bank in CGB
        // uint8_t XE000_ECHO_RAM[0x1e00]; // Echo of 0xC000 - 0xDDFF
        uint8_t XFE00_OAM[0xa0];
        // uint8_t XFEA0_NOT_USABLE[0x60]; // Use prohibited and not usable
        uint8_t XFF00_IO_REGS[0x80]; // More callbacks, less array?
        uint8_t XFF80_HRAM[0x7f];
        uint8_t XFFFF_IE;

        uint8_t m_RomBanks[512][0x4000];
        uint8_t m_RamBanks[16][0x2000];

        bool m_in_boot_rom_internal;
        int m_rom_bank_1; // Only switchable on later cartridges
        int m_rom_bank_2;
        int m_ram_bank;

        int m_mbc_type; // Only emulate MBC cartridges, for now
        bool m_cart_has_ram;
        bool m_cart_has_battery;
        bool m_cart_has_timer;
        bool m_cart_has_rumble;

        int m_num_rom_banks;
        int m_num_ram_banks;

        void interpret_cartridge_type(uint8_t cart_type, Cart_Details &cart_details);
        void interpret_rom_size_type(uint8_t rom_size, Cart_Details &cart_details);
        void interpret_ram_size_type(uint8_t ram_size, Cart_Details &cart_details);
        void interpret_licensee_code(uint8_t old_code, uint8_t new_code_1, uint8_t new_code_2, Cart_Details &cart_details);
        std::string readROMName(std::vector<uint8_t>* rom);
};

#endif