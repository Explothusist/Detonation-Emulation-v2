#ifndef EMULATOR_APU_H_
#define EMULATOR_APU_H_

class DMG_APU {
    public:
        DMG_APU();

        void Initialize();

        void PowerCycle();
        void Reset();

        void runMCycle();
        void runTCycle();

    private:

};

#endif