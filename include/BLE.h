#ifndef BLE_H_INCLUDED
#define BLE_H_INCLUDED
#pragma once

#include <NimBLEDevice.h>

class BLE {
  public:
    BLE();
    ~BLE();
    
    void init(const std::string& deviceName);
    void createServer();
    void createService(const std::string& serviceUUID);
    void createCharacteristic(const std::string& characteristicUUID, uint8_t properties);
    void addDescriptor(const std::string& descriptorUUID);
    void start();
    void getAdvertising();
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic, BLEConnInfo& connInfo) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        printf("*********\n");
        printf("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          printf("%d", rxValue[i]);

        printf("\n*********\n");
      }
    }
};