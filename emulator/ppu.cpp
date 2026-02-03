
#include "ppu.h"

#include "../enable_logging.h"

DMG_PPU::DMG_PPU() {

};

void DMG_PPU::Initialize() {
    
};

void DMG_PPU::PowerCycle() {
    Reset();
};
void DMG_PPU::Reset() {

};

void DMG_PPU::runMCycle() {
    for (int i = 0; i < 4; i++) {
        runTCycle();
    }
};
void DMG_PPU::runTCycle() {

};

// void DMG_PPU::PresentScreen() {

// };
