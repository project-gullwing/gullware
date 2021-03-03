#include "stepper.h"
#include <Arduino.h>

Stepper::Stepper(StepperDriver* driver, double acceleration_degsec2) {
    hw = driver;
	id = hw->getId();

    stepInterval = 0;
    setAcceleration(acceleration_degsec2);
    Serial.printf("%s    | Max speed: %f °/s\n", id, (SEC_TO_MICROS / MIN_DELAY_MICROS) * GEARED_STEP_SIZED_DEG[hw->getMicrostep()]);
    Serial.printf("%s    | ===================\n", id);
}


double Stepper::getSpeedDegSec() {
    return (stepInterval != INFINITY) ? (1000000.0 / stepInterval) * GEARED_STEP_SIZED_DEG[hw->getMicrostep()] * (hw->getDir() == DIR_CW ? 1 : -1) : 0.0;
}



double Stepper::getPosition() {
    return currentPosition;
}



int Stepper::getMicrostep() {
    return hw->getMicrostep();
}




void Stepper::setSpeedDegSec(double speed_degsec) {
    cmdTargetSpeed_degsec = speed_degsec;

    cmdTargetDelay_us = (speed_degsec != 0) ? abs(SEC_TO_MICROS / (speed_degsec / GEARED_STEP_SIZED_DEG[MICRO_FULL])) : INFINITY;
    //Serial.printf("%s    | TGT_DELAY: %.4fus\n", id, cmdTargetDelay_us);
    cmd = true;
};



void Stepper::setPosition(double pos) {
    currentPosition = pos;
}


void Stepper::setAcceleration(double acceleration_degsec2) {
    const double acceleration_steps_sec2 = acceleration_degsec2 / GEARED_STEP_SIZED_DEG[MICRO_FULL];
	c0 = 0.676 * sqrt(2.0 / acceleration_steps_sec2) * SEC_TO_MICROS; // Equation 15
    currentDelay_us = INFINITY;
    stepInterval = INFINITY;
    Serial.printf("%s    | C0: %.0fus\n", id, c0);
};






void Stepper::emergencyStop() {
    n = 0;
    stepInterval = 0;
    acceleration = 0;
}



void Stepper::update() {
    if (cmd) {
        cmd = false;
        if (stepInterval == INFINITY) {
            if (cmdTargetSpeed_degsec != 0) {
                hw->setDir(cmdTargetSpeed_degsec > 0 ? DIR_CW : DIR_CCW);
            }
        } 
        double direction = cmdTargetSpeed_degsec * hw->getDir();
        if (direction < 0) {
            targetDelay_us = INFINITY;
        } else {
            targetDelay_us = cmdTargetDelay_us;
            cmdTargetSpeed_degsec = 0;
        }
        currentDiff_us = abs(targetDelay_us - currentDelay_us);
        if (currentDelay_us > targetDelay_us) {
            if (acceleration != 1) {
                acceleration = 1;
                if (n < 0) {
                    n *= -1;
                }
                Serial.printf("%s    | (+)\n", id);
            }
        } else {
            if (acceleration != -1) {
                acceleration = -1;
                if (n > 0) {
                    n *= -1;
                }
                Serial.printf("%s    | (-)\n", id);
            }
        }
    }
    if (acceleration) {
        refreshSpeed();
    }
}



void Stepper::refreshSpeed() {
    hw->debug();

    if ((targetDelay_us == INFINITY) && (n == -1)) {
        n = 0;
        currentDelay_us = INFINITY;
        stepInterval = INFINITY;
        acceleration = 0;
        if (cmdTargetSpeed_degsec != 0) {
            cmd = true;
            Serial.printf("%s    | (<>)\n", id);
        } else {
            Serial.printf("%s    | (X)\n", id);
        }
        return;
    }

    double cms = hw->getMicrostep();
    double cn = (n == 0) ? c0 : currentDelay_us - ((2.0 * currentDelay_us) / (4.0 * n + 1)); // Equation 13
    double newDiff_us = abs(targetDelay_us - cn);
    n++;    
    if (newDiff_us > currentDiff_us) {
        Serial.printf("%s    | (=)\n", id);
        acceleration = 0;
    } else {
        currentDiff_us = newDiff_us;
    }
    currentDelay_us = cn;
    // int ms = 4;
    // while ((cn < MIN_DELAY_MICROS) && (ms > 0)) {
    //     cn *= 2;
    //     ms -= 1;
    // }
    // if (ms != cms) {
    //     hw->setMicrostep(ms);
    // }
    // if (stepInterval != INFINITY) {
    //     printf("%f [%d]", cn, n);
    // }
	cn = max(cn, (double) MIN_DELAY_MICROS); 
    stepInterval = cn;

}



void Stepper::move() {
    if (stepInterval != INFINITY) {
        unsigned long now = micros();   
        unsigned long timeDifference = (now - lastStepTime);   
        if ((timeDifference >= stepInterval) && ((n != 0))) {
            lastStepTime = now;
            hw->step();
            currentPosition += GEARED_STEP_SIZED_DEG[hw->getMicrostep()] * hw->getDir();
        }
    }
}