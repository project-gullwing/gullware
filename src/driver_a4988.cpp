#include "driver_a4988.h"


DriverA4988::DriverA4988(const char* id, int pinStep, int pinDir, int pinM0, int pinM1, int pinM2) {
    this->id = id;
    printf("%s:HW | Pololu A4988\n", id);
    this->pinStep = pinStep;
    this->pinDir = pinDir;
    this->pinM0 = pinM0;
    this->pinM1 = pinM1;
    this->pinM2 = pinM2;

    pinMode(pinStep, OUTPUT);
    pinMode(pinDir, OUTPUT);
    pinMode(pinM0, OUTPUT);
    pinMode(pinM1, OUTPUT);
    pinMode(pinM2, OUTPUT);

    digitalWrite(pinStep, LOW);
    setDir(DIR_CW);
    setMicrostep(MICRO_16);
}


const char* DriverA4988::getId() {
    return id;
}



int DriverA4988::getMicrostep() {
    return currentMicro;
}



int DriverA4988::getDir() {
    return currentDir;
}



void DriverA4988::setMicrostep(int div) {
    if ((div < MICRO_FULL) || (div > MICRO_16)) {
        printf("%s:HW | MS: Warning: unsupported value: %d, ignoring...\n", id, div);
        return;
    }
    if (div != currentMicro) {
        const int pinState = MICROSTEPPING_SETTINGS[div];
        digitalWrite(pinM0, pinState & BM_M0);
        digitalWrite(pinM1, pinState & BM_M1);
        digitalWrite(pinM2, pinState & BM_M2);
        currentMicro = div;
        printf("%s:HW | MS: %d [%c%c%c]\n", id, currentMicro, (pinState & BM_M0) ? 'o' : '-', (pinState & BM_M1) ? 'o' : '-', (pinState & BM_M2) ? 'o' : '-');
    }
}




void DriverA4988::setDir(int dir) {
    const int d = (dir == DIR_CCW) ? DIR_CCW : DIR_CW;
    if (d != currentDir) {
        digitalWrite(pinDir, (d == DIR_CW) ? LOW : HIGH);
        currentDir = d;
        printf("%s:HW | DIR: %s\n", id, (currentDir == DIR_CCW) ? "CCW" : "CW");
    }
}


void DriverA4988::step() {
    digitalWrite(pinStep, HIGH);
    digitalWrite(pinStep, LOW);
}


void DriverA4988::debug() {
}
 