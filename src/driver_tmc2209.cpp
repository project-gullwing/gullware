#include "driver_tmc2209.h"


DriverTMC2209::DriverTMC2209(const char* id, int pinStep, Stream* serialPort, uint8_t driverAddress) {
    this->id = id;
    this->pinStep = pinStep;
    this->driver = new TMC2209Stepper(serialPort, R_SENSE, driverAddress);
    this->driver->begin();
    this->driver->rms_current(1400);        // Set motor RMS current in milliamps
    //this->driver->en_spreadCycle(true);
    this->driver->TPWMTHRS(100);             // Set SpreadCycle threshold
    this->driver->pwm_autoscale(true);      // Set stealthChop PWM autoscale 
    
    pinMode(pinStep, OUTPUT);
    digitalWrite(pinStep, LOW);
    setDir(DIR_CW);
    setMicrostep(MICRO_FULL);
    printf("%s:HW | Trinamic 2209, Version: %d MS1: %d, MS2: %d, EN: %d\n", id, this->driver->version(), this->driver->ms1(), this->driver->ms2(), this->driver->isEnabled());
}


const char* DriverTMC2209::getId() {
    return id;
}



int DriverTMC2209::getMicrostep() {
    return currentMicro;
}



int DriverTMC2209::getDir() {
    return currentDir ? DIR_CCW : DIR_CW;
}



void DriverTMC2209::setMicrostep(int div) {
    if ((div < MICRO_FULL) || (div > MICRO_128)) {
        printf("%s:HW | MS: Warning: invalid value: %d, ignoring...\n", id, div);
        return;
    }
    if (div != currentMicro) {
        const int ms = 0; //MICROSTEPPING_SETTINGS[div];
        this->driver->microsteps(ms);
        currentMicro = div;
        printf("%s:HW | MS: %d [%d]\n", id, currentMicro, ms);
    }
}




void DriverTMC2209::setDir(int dir) {
    const bool d = (dir == DIR_CCW);
    if (d != currentDir) {
        this->driver->shaft(d);
        currentDir = d;
        printf("%s:HW | DIR: %s\n", id, currentDir ? "CCW" : "CW");
    }
}


void DriverTMC2209::step() {
    digitalWrite(pinStep, HIGH);
    digitalWrite(pinStep, LOW);
}


void DriverTMC2209::debug() {
    cnt++;
    if (cnt == 20) {
        //printf("%s:HW | TSTEP: %d PWM_GRAD_AUTO: %d PWM_OFS_AUTO: %d\n", id, this->driver->TSTEP(), this->driver->pwm_grad_auto(), this->driver->pwm_ofs_auto());
        cnt = 0;
    }
}