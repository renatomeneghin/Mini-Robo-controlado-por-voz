#include "../include/BLEServer.h"

void MyServerCallbacks::onConnect(BLEServer* pServer, BLEConnInfo& connInfo) {
    this->deviceConnected = true;
};

void MyServerCallbacks::onDisconnect(BLEServer* pServer, BLEConnInfo& connInfo, int reason) {
    this->deviceConnected = false;
}

uint32_t MyServerCallbacks::onPassKeyRequest(){
    printf("Server PassKeyRequest\n");
    return 12345678;
}

bool MyServerCallbacks::onConfirmPIN(uint32_t pass_key){
    printf("The passkey YES/NO number: %" PRIu32"\n", pass_key);
    return true;
}

void MyServerCallbacks::onAuthenticationComplete(BLEConnInfo& connInfo){
    printf("Starting BLE work!\n");
}