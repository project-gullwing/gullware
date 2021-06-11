#include "ramp.h"
#include "stepper.h"
#include <Arduino.h>

Stepper::Stepper(StepperDriver* driver) {
    hw = driver;
	id = hw->getId();

    Serial.printf("%s    | ===================\n", id);
}


double Stepper::getSpeedDegSec() {
    if (currentDelay_us == INFINITY) {
        return 0.0;
    }
    return delayToAngularSpeed(currentDelay_us, ms) * dir;
}


void Stepper::setSpeedDegSec(double speed_degSec) {
    // Detect direction change
    if ((speed_degSec * dir) < 0) {
      flip = true;
    }
    targetDelay_us = angularSpeedToDelay(fabs(speed_degSec), ms);
    acc = true;
};


double Stepper::getPosition() {
    return pos;
}



void Stepper::setPosition(double positionDeg) {
    pos = positionDeg;
}



int Stepper::getMicrostep() {
    return ms;
}



void Stepper::move() {
    double tgt = targetDelay_us;

    // Handle speed change
    if (acc) {

      // Handle direction change  
      if (flip) {
        if (currentDelay_us != INFINITY) {
          tgt = INFINITY;
        } else {
          flip = false;
          dir *= -1;
        }
      }

      double delta = lt(tgt, currentDelay_us, 0.0001)
          ? 1
          : gt(tgt, currentDelay_us, 0.0001)
              ? -1
              : 0;

      n += delta;
      currentDelay_us = (n < 0)
          ? INFINITY
          : DELAYS_MICROS[n];

      acc = ((delta > 0) && (n < MAX_RAMP_IDX) && (DELAYS_MICROS[n+1] > tgt)) || ((delta < 0) && (n >= 0)) || flip;
    }

    if ((n >= 0) && (n <= MAX_RAMP_IDX)) {
        unsigned long now = micros();   
        if ((now - lastStepTime) > DELAYS_MICROS[n]) {
            hw->step();
            lastStepTime = now;
        }
    }
}



void Stepper::stop(bool emergency) {
    targetDelay_us = INFINITY;
    if (emergency) {
        n = -1;
        currentDelay_us = INFINITY;
        acc = SPEED_CONSTANT;
    }
}



