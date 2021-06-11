#include "stepper_common.h"

#define SPEED_CONSTANT false
#define SPEED_CHANGE true


class Stepper {
    private:
        /**
         * Stepper ID
         */ 
        const char* id;

        /**
         * Stepper driver
         */ 
        StepperDriver* hw;

        /**
         * Microstep selector
         */ 
        int ms = 0;

        /**
         * Direction selector (1 = clockwise, -1 = counterclockwise)
         */ 
        int dir = DIR_CW;

        /**
         * Current position in fractional degrees
         */ 
        double pos = 0.0;

        /** 
         * Target delay
         */ 
        double targetDelay_us = 0;

        /**
         * Current step delay
         */ 
        double currentDelay_us = 0;

        /**
         * Ramp counter
         */ 
        long n = -1;

        /**
         * Acceleration status (-1 = decelerating, 0 = constant, 1 = accelerating)
         */ 
        bool acc = SPEED_CONSTANT;
        
        /**
         * Change the rotation direction. 
         * When set, the motor slows down to stop, flips the direction, and spins up to the final target speed.
         */
        bool flip = false;

        /**
         * Step timing
         */ 
        unsigned long lastStepTime = 0;


    public:
        Stepper(StepperDriver *driver);

        /**
         * @returns Current stepper speed in °/s (geared)
         */
        double getSpeedDegSec();

        /**
         * Sets the desired stepper speed.
         * @param speed Desired rotation speed in °/s (geared)
         */ 
        void setSpeedDegSec(double speed);

        /**
         * @returns Current stepper position in fractional steps.
         */
        double getPosition();

        /**
         * Sets current position. If not set, defaults to 0.0
         * @param positionDeg Position in fractional steps
         */
        virtual void setPosition(double positionDeg);

        /**
         * @returns Current microstep setting. One of: MICRO_FULL, MICRO_2, MICRO_4, MICRO_8, MICRO_16
         */
        int getMicrostep();

        /**
         * Moves the stepper.
         * Call from separate thread.
         */
        void move();

        /**
         * Stops the stepper.
         * @param emergency Instant cut-off?
         */ 
        void stop(bool emergency);
};
