
#include "apu.h"

#include "../enable_logging.h"

DMG_APU::DMG_APU() {

};

void DMG_APU::Initialize() {
    
};

void DMG_APU::PowerCycle() {
    Reset();
};
void DMG_APU::Reset() {
    
};

void DMG_APU::runMCycle() {
    for (int i = 0; i < 4; i++) {
        runTCycle();
    }
};
void DMG_APU::runTCycle() {
    
};
