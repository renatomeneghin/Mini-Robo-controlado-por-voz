#ifndef BLE_H_INCLUDED
#define BLE_H_INCLUDED
#pragma once

class MyServerCallbacks: public BLEServerCallbacks {
    private:
        bool deviceConnected = false;

    void onConnect(BLEServer* pServer, BLEConnInfo& connInfo) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer, BLEConnInfo& connInfo, int reason) {
      deviceConnected = false;
    }
  /***************** New - Security handled here ********************
  ****** Note: these are the same return values as defaults ********/
    uint32_t onPassKeyRequest(){
      printf("Server PassKeyRequest\n");
      return 12345678;
    }

    bool onConfirmPIN(uint32_t pass_key){
      printf("The passkey YES/NO number: %" PRIu32"\n", pass_key);
      return true;
    }

    void onAuthenticationComplete(BLEConnInfo& connInfo){
      printf("Starting BLE work!\n");
    }
  /*******************************************************************/
};

#endif