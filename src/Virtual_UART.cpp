#include "Virtual_UART.h"

Virtual_UART::Virtual_UART() {
  // Default constructor implementation
}

Virtual_UART::Virtual_UART(int txPin, int rxPin, int baudRate, int dataBits, bool parity) {
  // Parameterized constructor implementation
  this->Tx = txPin;
  this->Rx = rxPin;
  this->baudRate = baudRate;
  this->dataBits = dataBits;
  this->parity = parity;
}

Virtual_UART::Virtual_UART(int txPin, int rxPin) {
  // Constructor implementation with only tx and rx pins
  this->Tx = txPin;
  this->Rx = rxPin;
  this->baudRate = 9600;
  this->dataBits = 7;
  this->parity = false;
}

Virtual_UART::~Virtual_UART() {
  
}

void Virtual_UART::setTx(int txPin) {

  this->Tx = txPin;
}

void Virtual_UART::setRx(int rxPin) {
  this->Rx = rxPin;
}

void Virtual_UART::setBaudRate(int baudRate) {
  this->baudRate = baudRate;
}

void Virtual_UART::setDataBits(int dataBits) {
  this->dataBits = dataBits;
}

void Virtual_UART::setParity(bool parity) {
  this->parity = parity;
}

int Virtual_UART::getTx() {
  return this->Tx;
}

int Virtual_UART::getRx() {
  return this->Rx;
}

int Virtual_UART::getBaudRate() {
  return this->baudRate;
}

int Virtual_UART::getDataBits() {
  return this->dataBits;
}

bool Virtual_UART::getParity() {
  return this->parity;
}

void Virtual_UART::send(char data) {
  // Envia os dados via Virtual_UART
  WritePin(this->Tx,0); // Inicia a transmissão
  Wait(this->baudRate); 
  for (char i = 0; i < this->dataBits; i++){ // Envia os dados
    WritePin(this->Tx,(data & (1 << i))); 
    Wait(this->baudRate);
  }
  //Verificação de paridade
  WritePin(this->Tx,this->parity);
  Wait(this->baudRate);

  // Finaliza a transmissão
  WritePin(this->Tx,1);
  Wait(this->baudRate);
}

char Virtual_UART::receive() {
  //Recebe dados da Virtual_UART
  char c = 0;

  while (ReadPin(this->Rx));

  Wait(this->baudRate);

  for (char i = 0; i < this->dataBits; i++){
    c |= (0x01 & ReadPin(this->Rx)) << i; //Leitura de bit
    Wait(this->baudRate);
  }

  //Verificação de paridade
  c |= (0x01 & ReadPin(this->Rx)) << this->dataBits; //Leitura de bit
  Wait(this->baudRate);

  //Aguarda o bit de parada
  Wait(this->baudRate);

  return c;
  
}