#include <Arduino.h>

// Clockwise direction
#define DIR_CW 1

// Counterclockwise direction
#define DIR_CCW -1

// Microstepping: Full (No microstepping)
#define MICRO_FULL 0

// Microstepping: 1/2
#define MICRO_2 1

// Microstepping: 1/4
#define MICRO_4 2

// Microstepping: 1/8
#define MICRO_8 3

// Microstepping: 1/16
#define MICRO_16 4

// Microstepping: 1/32
#define MICRO_32 5

// Microstepping: 1/64
#define MICRO_64 6

// Microstepping: 1/128
#define MICRO_128 7

// Microstepping: 1/256
#define MICRO_256 8

class StepperDriver {
    public:

        /**
         * Gets the driver ID.
         */
        virtual const char* getId();

        /**
         * @returns Current microstep setting. One of: MICRO_FULL, MICRO_2, MICRO_4, MICRO_8, MICRO_16
         */
        virtual int getMicrostep();

        /**
         * @returns Current direction (DIR_CW or DIR_CCW)
         */
        virtual int getDir();

        /**
         * Configures microstepping mode.
         * @param div Microstepping divisor. One of: MICRO_FULL, MICRO_2, MICRO_4, MICRO_8, MICRO_16
         */
        virtual void setMicrostep(int div);

        /**
         * Configures rotation direction.
         * @param dir Direction value. One of: DIR_CW, DIR_CCW
         */
        virtual void setDir(int dir);

        /**
         * Executes a single step.
         */
        virtual void step();


        virtual void debug();


};