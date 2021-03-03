#include "stepper_common.h"

#define BM_M0 0b001
#define BM_M1 0b010
#define BM_M2 0b100

const int MICROSTEPPING_SETTINGS[] = {0b000, 0b001, 0b010, 0b011, 0b111};


class DriverA4988: public StepperDriver {

    private:
        // Stepper ID to be used in log messages
        const char* id;

        // STEP pin number
        int pinStep;

        // DIR pin number
        int pinDir;

        // Microstep configuration pin 0
        int pinM0;

        // Microstep configuration pin 1
        int pinM1;

        // Microstep configuration pin 2
        int pinM2;

        // Current microstepping value (ordinal, 0..4)
        int currentMicro = -1;

        // Current direction value (DIR_CW or DIR_CCW)
        int currentDir = 0;

        // Current axis position in fractional steps
        double currentPos = 0.0;

    public:
        /**
         * Creates a new A4988 Stepper instance.
         * @param id Stepper ID to be used in log messages
         * @param pinStep STEP pin number
         * @param pinDir DIR pin number
         * @param pinM0 Microstepping configuration pin 0
         * @param pinM1 Microstepping configuration pin 1
         * @param pinM2 Microstepping configuration pin 2
         */  
        DriverA4988(const char* id, int pinStep, int pinDir, int pinM0, int pinM1, int pinM2);

        const char* getId();

        int getMicrostep();

        int getDir();

        void setMicrostep(int div);

        void setDir(int dir);

        void step();

        virtual void debug();
};
