#ifndef BLE_H_INCLUDED
#define BLE_H_INCLUDED
#pragma once

#include <NimBLEDevice.h>

class MyServerCallbacks: public BLEServerCallbacks {
  private:
    bool deviceConnected = false;
  public:
    void onConnect(BLEServer* pServer, BLEConnInfo& connInfo);
    void onDisconnect(BLEServer* pServer, BLEConnInfo& connInfo, int reason);
    uint32_t onPassKeyRequest();
    bool onConfirmPIN(uint32_t pass_key);
    void onAuthenticationComplete(BLEConnInfo& connInfo);
};

#endif