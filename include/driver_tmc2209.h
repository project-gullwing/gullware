#include "stepper_common.h"
#include <TMCStepper.h>

#define R_SENSE 0.11f

const int MICROSTEPPING_SETTINGS[] = {0, 7, 6, 5, 4, 3, 2, 1};


class DriverTMC2209: public StepperDriver {

    private:
        // Stepper ID to be used in log messages
        const char* id;

        // STEP pin number
        int pinStep;

        // STEP pin number
        int cnt = 0;

        //TMCStepper instance
        TMC2209Stepper* driver;

        // Current microstepping value (ordinal, 0..4)
        int currentMicro = -1;

        // Current direction value (false = DIR_CW, true = DIR_CCW)
        bool currentDir = false;

        // Current axis position in fractional steps
        double currentPos = 0.0;

    public:
        /**
         * Creates a new TMC2209 Stepper instance.
         * @param id Stepper ID to be used in log messages
         * @param pinStep STEP pin number
         * @param serialPort Serial port to use for communication with the driver
         * @param driverAddress Driver address (0b00..0b11) on the serial port
         */  
        DriverTMC2209(const char* id, int pinStep, Stream* serialPort, uint8_t driverAddress);

        const char* getId();

        int getMicrostep();

        int getDir();

        void setMicrostep(int div);

        void setDir(int dir);

        void step();

        void debug();

};
