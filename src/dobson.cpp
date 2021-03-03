#include "dobson.h"
#include <Arduino.h>
#include <functional>



void refreshSpeed(void* pDobson) {
  for(;;) {
    ((Dobson*)pDobson)->update();
    delay(10);
  }
}


Dobson::Dobson() {
    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, LOW);

    pBTServer = new BtServer("DOBastl");

    #ifdef A4988
        pAzimuth = new Stepper(new DriverA4988("AZI", AZI_STEP, AZI_DIR, AZI_M0, AZI_M1, AZI_M2), 150.0);
        pElevation = new Stepper(new DriverA4988("ELE", ALT_STEP, ALT_DIR, ALT_M0, ALT_M1, ALT_M2), 150.0);
    #endif

    #ifdef TMC2209
        UART.begin(115200, SERIAL_8N1, UART_RX, UART_TX);
        //pAzimuth = new Stepper(new DriverTMC2209("AZI", AZI_STEP, &UART, AZI_ADDR), 150.0);
        pElevation = new Stepper(new DriverTMC2209("ELE", ELE_STEP, &UART, ELE_ADDR), 5.0);
    #endif

    pBTService = pBTServer->addService(SERVICE_UUID);
    pTxCharacteristic = pBTService->addTxCharacteristic(CHARACTERISTIC_UUID_TX);
    pBTService->addRxCharacteristic(CHARACTERISTIC_UUID_RX, [this](BLECharacteristic *pCharacteristic) {
            std::string msg = pCharacteristic->getValue();
            if (msg.find("M ") == 0) {
                const char* cmsg = msg.c_str();
                double angle = atof(cmsg + 2);
                double speed = atof(cmsg + msg.find_last_of(" "));
                executeMove(angle, speed);
            }
            if (msg.find("I ") == 0) {
                executeIdle();
            }
            if (msg.find("X ") == 0) {
                executeHardStop();
            }
        });

    pBTService->start();
    pBTServer->start();

    xTaskCreatePinnedToCore(refreshSpeed, "CalcTask", 10000, this,  0, this->xHandle, 0); 

};


void Dobson::update() {
    //pAzimuth->update();
    pElevation->update();
    if (pBTServer->connected) {
        unsigned int now = micros();
        if ((now - _lastUpdateTime) > 50000) {
            _lastUpdateTime = now;
            double dx = 0.0;//pAzimuth->getSpeedDegSec();
            double dy = pElevation->getSpeedDegSec();

            double speedDegSec = sqrt(dx*dx + dy*dy);
            double r = (speedDegSec > 0) ? dx / speedDegSec : 0;
            double angle = acos(r) * ((dy > 0) ? 1 : -1);
            double angleDeg = angle / PI * 180.0;

            sprintf(txBuffer, "A%.2f S%.2f Z%d L%d", angleDeg, speedDegSec, 0/*pAzimuth->getMicrostep()*/, pElevation->getMicrostep());
            pTxCharacteristic->setValue(txBuffer);
            pTxCharacteristic->notify();

            sprintf(txBuffer, "PZ%.5f PL%.5f", 0.0/*pAzimuth->getPosition()*/, pElevation->getPosition());
            pTxCharacteristic->setValue(txBuffer);
            pTxCharacteristic->notify();

            // sprintf(txBuffer, "MZ%d ML%d", pAzimuth->getMicrostep(), pAltitude->getMicrostep());
            // pTxCharacteristic->setValue(txBuffer);
            // pTxCharacteristic->notify();


        }
    }
}


void Dobson::move() { 
    //pAzimuth->move();
    pElevation->move(); 
}


void Dobson::executeMove(double angle, double speed) {
    double angleRad = angle / 180.0 * PI;
    double dx = cos(angleRad) * speed;
    double dy = sin(angleRad) * speed;
    Serial.printf("\nDOB:CMD| MOVE Angle: %.2f rads  Dx: %.2f°/s  Dy: %.2f°/s\n", angleRad, dx, dy);
    //pAzimuth->setSpeedDegSec(dx);
    pElevation->setSpeedDegSec(dy);
}

void Dobson::executeIdle() {
    Serial.println("\nDOB:CMD| IDLE");
    //pAzimuth->setSpeedDegSec(0);
    pElevation->setSpeedDegSec(0);
}

void Dobson::executeHardStop() {
    Serial.println("\nDOB:CMD| HARD_STOP");
    digitalWrite(ENABLE, HIGH);
    //pAzimuth->emergencyStop();
    pElevation->emergencyStop();
}
