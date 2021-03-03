#define A4988
#define TMC2209

#ifdef TMC2209 
    #undef A4988
#endif

#ifdef A4988
    #include "driver_a4988.h"
#endif    
#ifdef TMC2209
    #include "driver_tmc2209.h"
#endif    

#define SEC_TO_MICROS 1000000.0
#define MIN_DELAY_MICROS 300.0

#define TRANSMISSION_RATIO 1.0 / 2400.0
#define STEP_SIZE_DEG 1.8
#define GEARED_STEP_SIZE_DEG STEP_SIZE_DEG * TRANSMISSION_RATIO

const double GEARED_STEP_SIZED_DEG[] = {
    GEARED_STEP_SIZE_DEG, 
    GEARED_STEP_SIZE_DEG / 2.0,
    GEARED_STEP_SIZE_DEG / 4.0, 
    GEARED_STEP_SIZE_DEG / 8.0,
    GEARED_STEP_SIZE_DEG / 16.0,
    GEARED_STEP_SIZE_DEG / 32.0,
    GEARED_STEP_SIZE_DEG / 64.0,
    GEARED_STEP_SIZE_DEG / 128.0,
    GEARED_STEP_SIZE_DEG / 256.0,
};


class Stepper {
    private:
        // Stepper ID
        const char* id;

        // Stepper driver
        StepperDriver* hw;

        // Current position in fractional degrees
        double currentPosition = 0.0;

        // Step timing
        unsigned long lastStepTime = 0;

        // Step interval
        double stepInterval = 0;

        // Ramp counter
        long n = 0;

        // Ramp start
        double c0 = 0;

        // Acceleration status (-1=decelerating, 1=accelerating, 0=steady)
        int acceleration = 0;

        // Command flag
        bool cmd = false;

        // Commanded targed speed
        double cmdTargetSpeed_degsec = 0;

        // Commanded target delay
        double cmdTargetDelay_us = 0;

        // Actual target step delay
        double targetDelay_us = 0;

        // Current step delay
        double currentDelay_us = 0;

        // Current diff. between actual and target step delay
        double currentDiff_us = 0;

        void setAcceleration(double acceleration_degsec2);
        void refreshSpeed();

    public:
        Stepper(StepperDriver *driver, double acceleration_degsec2);

        /**
         * @returns Current stepper speed in °/s (geared)
         */
        double getSpeedDegSec();

        /**
         * @returns Current stepper position in fractional steps.
         */
        double getPosition();

        /**
         * @returns Current microstep setting. One of: MICRO_FULL, MICRO_2, MICRO_4, MICRO_8, MICRO_16
         */
        int getMicrostep();

        /**
         * Sets the desired stepper speed.
         * @param speed Desired rotation speed in °/s (geared)
         */ 
        void setSpeedDegSec(double speed);

        /**
         * Sets current position. If not set, defaults to 0.0
         * @param pos Position in fractional steps
         */
        virtual void setPosition(double pos);

        /**
         * Stops the stepper.
         */ 
        void emergencyStop();

        /**
         * Updates stepper speed.
         * Call from separate thread.
         */
        void update();

        /**
         * Moves the stepper.
         * Call from loop()
         */
        void move();
};
