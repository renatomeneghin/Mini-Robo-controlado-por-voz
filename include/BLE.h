#ifndef BLE_H_INCLUDED
#define BLE_H_INCLUDED
#pragma once

#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class BLE: public BLECharacteristicCallbacks, public BLEServerCallbacks {
  private:
    std::string servername;
    NimBLEServer *pServer;
    BLEService *pService;
    NimBLECharacteristic * pTxCharacteristic;
    NimBLECharacteristic * pRxCharacteristic;
    bool deviceConnected = false;
    bool enviar = false;
  public:
    BLE();
    BLE(const std::string& servername);
    ~BLE();
    void init();
    void init(const std::string& deviceName);
    void initFull();
    void createCharacteristicRX(const std::string& characteristicUUID, uint8_t properties);
    void createCharacteristicTX(const std::string& characteristicUUID, uint8_t properties);
    void startAdvertising();
    void stopAdvertising();
    void send(std::string data);
    std::string getServerName();
    void setDeviceConnected(bool connected);
    bool getDeviceConnected();
    void onConnect(BLEServer* pServer, BLEConnInfo& connInfo);
    void onDisconnect(BLEServer* pServer, BLEConnInfo& connInfo, int reason);
    uint32_t onPassKeyRequest();
    bool onConfirmPIN(uint32_t pass_key);
    void onAuthenticationComplete(BLEConnInfo& connInfo);
    void onWrite(BLECharacteristic *pCharacteristic, BLEConnInfo& connInfo);
    void setBLEEnvio(bool envio);
    bool getBLEEnvio();
};

#endif