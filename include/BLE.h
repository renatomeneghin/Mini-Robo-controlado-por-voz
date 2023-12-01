#ifndef BLE_H_INCLUDED
#define BLE_H_INCLUDED
#pragma once

#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class BLE: public BLECharacteristicCallbacks {
  private:
    char servername[32];
    NimBLEServer *pServer;
    NimBLECharacteristic * pTxCharacteristic;
    NimBLECharacteristic * pRxCharacteristic;
    bool deviceConnected = false;
    bool oldDeviceConnected = false;
  public:
    BLE(char servername[32]);
    ~BLE();
    void init();
    void init(const std::string& deviceName);
    void initFull();
    void initFull(const std::string& deviceName);
    void createServer();
    void createCharacteristicRX(const std::string& characteristicUUID, uint8_t properties);
    void createCharacteristicTX(const std::string& characteristicUUID, uint8_t properties);
    void start();
    void getAdvertising();
    std::string onWrite(BLECharacteristic *pCharacteristic, BLEConnInfo& connInfo);
    void send(std::string data);
    void stop();
    char* getServerName();
    void setDeviceConnected(bool connected);
    bool getDeviceConnected();
    void setOldDeviceConnected(bool connected);
    bool getOldDeviceConnected();
};

#endif
