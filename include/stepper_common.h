#include <Arduino.h>
#include <math.h>

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


const double SEC_TO_MICROS = 1000000.0;
const double TRANSMISSION_RATIO = 1.0 / 2400.0;
const double STEP_SIZE_DEG = 1.8;
const double GEARED_STEP_SIZE_DEG = STEP_SIZE_DEG * TRANSMISSION_RATIO;
const double MS_STEP_SIZE_DEG[] = {
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


double diff(double a, double b, double epsilon) {
  return (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon;
}


/**
 * Equality comparison for double numbers.
 * Returns true, if "a" is within epsilon offset from "b".
 */  
bool eq(double a, double b, double epsilon)
{
    return fabs(a - b) <= diff(a, b, epsilon);
}


/**
 * Greater-than comparison for double numbers.
 * Returns true, if "a" is greater than "b", differing by more than epsilon offset.
 */  
bool gt(double a, double b, double epsilon) {
  int inf = (a == INFINITY) ? 1 : (b == INFINITY) ? -1 : 0;
  if (inf == 0) {
    return (a - b) > diff(a, b, epsilon);
  }
  return (inf > 0);
}


/**
 * Less-than comparison for double numbers.
 * Returns true, if "a" is less than "b", differing by more than epsilon offset.
 */  
bool lt(double a, double b, double epsilon) {
  int inf = (b == INFINITY) ? 1 : (a == INFINITY) ? -1 : 0;
  if (inf == 0) {
    return (b - a) > diff(a, b, epsilon);
  }
  return (inf > 0);
}


/**
 * Converts angular speed to pulse delay, with respect to microstep setting
 * @param speed_degSec Angular speed in degrees / second
 * @param ms Microstep setting (index of MS_STEP_SIZE_DEG array, 0 = full step, 8 = 1/256 microstep)
 * @returns Pulse delay in microseconds
 */ 
double angularSpeedToDelay(double speed_degSec, int ms) {
  return (speed_degSec > 0)
      ? fabs(SEC_TO_MICROS / (speed_degSec / MS_STEP_SIZE_DEG[ms]))
      : INFINITY;
}


/**
 * Converts pulse delay to angular speed, with respect to microstep setting
 * @param delay_us Pulse delay in microseconds
 * @param ms Microstep setting (index of MS_STEP_SIZE_DEG array, 0 = full step, 8 = 1/256 microstep)
 * @returns Angular speed in degrees / second
 */ 
double delayToAngularSpeed(double delay_us, int ms) {
  return (delay_us != INFINITY)
      ? MS_STEP_SIZE_DEG[ms] * (SEC_TO_MICROS / delay_us)
      : 0.0;
}


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