#include "../include/BLE.h"
#include "BLE.h"

BLE::BLE(char servername[32]){
    strcpy(this->servername,servername);  
}
BLE::~BLE(){
    this->pServer->getAdvertising()->stop();
    this->pServer->stop();
    delete this->pServer;
}
void BLE::init(){
    BLEDevice::init(this->servername);
}
void BLE::init(const std::string& deviceName){
    strcpy(this->servername,deviceName.c_str());
    BLEDevice::init(this->servername);
}
void BLE::initFull()
{
    this->init();
    this->createServer();
    this->createCharacteristicTX(CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    this->createCharacteristicRX(CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    this->start();
    this->getAdvertising();
}


void BLE::createServer()
{
    this->pServer = BLEDevice::createServer();
    this->pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
}
void BLE::createCharacteristicTX(const std::string& characteristicUUID, uint8_t properties){
    // Create a BLE Characteristic
    this->pTxCharacteristic = pService->createCharacteristic(
                                        characteristicUUID,
                                        properties
                                       );
}
void BLE::createCharacteristicRX(const std::string& characteristicUUID, uint8_t properties){
    // Create a BLE Characteristic
    this->pRxCharacteristic = pService->createCharacteristic(
                                            characteristicUUID,
                                            properties
                                        );
    this->pRxCharacteristic->setCallbacks(new MyCallbacks());
}
void BLE::start(){
    // Start the service
    pService->start();
}
void BLE::getAdvertising(){
    this->pServer->getAdvertising()->start();
}

void BLE::stopAdvertising(){
    this->pServer->getAdvertising()->stop();
}

void BLE::stop(){
    this->pServer->stop();
}

std::string BLE::onWrite(BLECharacteristic *pCharacteristic, BLEConnInfo &connInfo)
{
    return pCharacteristic->getValue();
}

void BLE::send(std::string data)
{
    pTxCharacteristic->setValue(data);
    pTxCharacteristic->notify();
}

char *BLE::getServerName(){return this->servername;}
void BLE::setDeviceConnected(bool connected){this->deviceConnected = connected;}
bool BLE::getDeviceConnected(){return this->deviceConnected;}
void BLE::setOldDeviceConnected(bool connected){this->oldDeviceConnected = connected;}
bool BLE::getOldDeviceConnected(){return this->oldDeviceConnected;}
