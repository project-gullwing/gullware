#include "bt.h"
#include "stepper.h"


#define ENABLE 5
#define UART_RX 16
#define UART_TX 17
#define UART Serial2

#define AZI_ADDR 0b00
#define AZI_STEP 18

#define ELE_ADDR 0b01
#define ELE_STEP 19




#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"



class Dobson {
    private:
        Stepper* pAzimuth = NULL;
        Stepper* pElevation = NULL;
        BtServer* pBTServer = NULL;
        BtService* pBTService = NULL;
        BLECharacteristic* pTxCharacteristic = NULL;

        bool _state = false;
        bool _kill = false;
        int _delayMicros = 50;
        unsigned long _lastUpdateTime; 
        char txBuffer[20];        

        TaskHandle_t* xHandle = NULL;        

        void executeMove(double angle, double speed);
        void executeIdle();
        void executeHardStop();

    public:
        Dobson();
        void update();
        void move();
};
