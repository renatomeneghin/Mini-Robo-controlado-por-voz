#include "../include/BLE.h"

BLE::BLE()
{
    this->servername = "ESP32_BLE_R";
}

BLE::BLE(const std::string& servername)
{
    this->servername = servername;
}
BLE::~BLE(){
    this->stopAdvertising();
}
void BLE::init(){
    BLEDevice::init(this->servername);
}
void BLE::init(const std::string& deviceName){
    this->servername = deviceName;
    BLEDevice::init(this->servername);
}
void BLE::initFull()
{
    BLEDevice::init(this->servername);
    this->pServer = BLEDevice::createServer();
    this->pServer->setCallbacks(this);
    // Create the BLE Service
    this->pService = pServer->createService(SERVICE_UUID);
    // Create a BLE Characteristic
    this->pTxCharacteristic = pService->createCharacteristic(
                                                CHARACTERISTIC_UUID_TX,
                                                NIMBLE_PROPERTY::NOTIFY                                       
                                                );
    
    this->pRxCharacteristic = pService->createCharacteristic(
                                                CHARACTERISTIC_UUID_RX, 
                                                NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ
                                                );
    this->pRxCharacteristic->setCallbacks(this);

    // Start the service
    this->pService->start();
    // Start advertising
    this->pServer->getAdvertising()->start();
}

void BLE::createCharacteristicTX(const std::string& characteristicUUID, uint8_t properties){
    // Create a BLE Characteristic
    this->pTxCharacteristic = this->pService->createCharacteristic(
                                        characteristicUUID,
                                        properties
                                       );
}
void BLE::createCharacteristicRX(const std::string& characteristicUUID, uint8_t properties){
    // Create a BLE Characteristic
    this->pRxCharacteristic = this->pService->createCharacteristic(
                                            characteristicUUID,
                                            properties
                                        );
    this->pRxCharacteristic->setCallbacks(this);
}

void BLE::startAdvertising(){
    this->pServer->getAdvertising()->start();
}

void BLE::stopAdvertising(){
    this->pServer->getAdvertising()->stop();
}

void BLE::send(std::string data)
{
    pTxCharacteristic->setValue(data);
    pTxCharacteristic->notify();
}

std::string BLE::getServerName(){return this->servername;}
void BLE::setDeviceConnected(bool connected){this->deviceConnected = connected;}
bool BLE::getDeviceConnected(){return this->deviceConnected;}

void BLE::onConnect(BLEServer *pServer, BLEConnInfo &connInfo) {
    this->deviceConnected = true;
};

void BLE::onDisconnect(BLEServer *pServer, BLEConnInfo &connInfo, int reason) {
    this->deviceConnected = false;
    this->startAdvertising();
}

uint32_t BLE::onPassKeyRequest()
{
    printf("Server PassKeyRequest\n");
    return 123456;
}
bool BLE::onConfirmPIN(uint32_t pass_key)
{
    printf("The passkey YES/NO number: %" PRIu32"\n", pass_key);
    return true;
}

void BLE::onAuthenticationComplete(BLEConnInfo& connInfo){
    printf("Starting BLE work!\n");
}

void BLE::setBLEEnvio(bool envio){this->enviar = envio;}
bool BLE::getBLEEnvio(){return this->enviar;}

void BLE::onWrite(BLECharacteristic *pCharacteristic, BLEConnInfo &connInfo){
    std::string rxValue = pCharacteristic->getValue();

    if(rxValue.contains("Comando") && deviceConnected) {this->enviar = true;}
}