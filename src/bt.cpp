#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "bt.h"


BtService::BtService(BLEServer *pServer, std::string uuid) {

    pService = pServer->createService(uuid);
}


BLECharacteristic* BtService::addTxCharacteristic(std::string uuid) {

    BLECharacteristic* pCharacteristic = pService->createCharacteristic(uuid, BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->addDescriptor(new BLE2902());
    return pCharacteristic;    
}


void BtService::addRxCharacteristic(std::string uuid, std::function<void(BLECharacteristic*)> onWrite) {

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(uuid, BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setCallbacks(new CharacteristicCallback(onWrite));    
}


void BtService::start() {
    pService->start();
}



BtServer::BtServer(std::string name) {

    BLEDevice::init(name);
    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new ServerCallbacks(
        [this](BLEServer* pServer) {
            Serial.println("DOB:BT | Connected");
            connected = true;
        },
        [this](BLEServer* pServer) {
            Serial.println("DOB:BT | Disconnected");
            connected = false;
        }
    ));
}



BtService* BtServer::addService(std::string uuid) {
    return new BtService(pServer, uuid);
}


void BtServer::start() {
    pServer->getAdvertising()->start();
}