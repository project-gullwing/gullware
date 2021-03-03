#include <functional>
#include <Arduino.h>
#include <BLEServer.h>

/**
 *  Generic BLE server callbacks class
 */
class ServerCallbacks: public BLEServerCallbacks {
    private:
        std::function<void(BLEServer*)> _onConnected;
        std::function<void(BLEServer*)> _onDisconnected;

    public:
        ServerCallbacks(std::function<void(BLEServer*)> onConnected, std::function<void(BLEServer*)> onDisconnected) {
            _onConnected = onConnected;
            _onDisconnected = onDisconnected;
        }

        void onConnect(BLEServer* pServer) {
            _onConnected(pServer);
        };

        void onDisconnect(BLEServer* pServer) {
            _onDisconnected(pServer);
        }
};


/**
 * Generic BLE characteristic callback class
 */
class CharacteristicCallback: public BLECharacteristicCallbacks {
    private:
        std::function<void(BLECharacteristic*)> _onWrite;

    public:
        CharacteristicCallback(std::function<void(BLECharacteristic*)> onWrite) {
            _onWrite = onWrite;
        }

        void onWrite(BLECharacteristic* pCharacteristic) {
            _onWrite(pCharacteristic);
        }
};


class BtService {
    private:
        BLEService* pService;

    public:
        BtService(BLEServer* server, std::string uuid);

        BLECharacteristic* addTxCharacteristic(std::string uuid);
        void addRxCharacteristic(std::string uuid, std::function<void(BLECharacteristic*)> onWrite);

        void start();
};


class BtServer {

    private:
        BLEServer* pServer;


    public:
        bool connected;
        
        BtServer(std::string name);

        BtService* addService(std::string uuid);

        void start();

};