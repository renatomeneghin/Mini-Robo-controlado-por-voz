#include "ESP32_GPIO.h"

ESP32::ESP32() {

}

ESP32::~ESP32() {

}

void ESP32::init() {

}

void ESP32::UART_Config() {
  //pinos GPIO 1 e 3, configurados na função 0
  volatile uint32_t *GPIO1_CONF_FUNC = (volatile uint32_t *) 0x3FF44534;
  volatile uint32_t *IO_MUX_GPIO1 = (volatile uint32_t *) 0x3FF49088;
  volatile uint32_t *GPIO3_CONF_FUNC = (volatile uint32_t *) (0x3FF44130 + 0x4*14);
  volatile uint32_t *IO_MUX_GPIO3 = (volatile uint32_t *) 0x3FF49084;
  volatile uint32_t *GPIO_OUT_EN = (volatile uint32_t *) 0x3FF44020;
  *(0x3FF44020) |= 0b1 << 1; //Pino GPIO1
  
  *(GPIO1_CONF_FUNC) = (*(GPIO1_CONF_FUNC) & ~(0b111111111111)) | 0b000000001100;
  *(IO_MUX_GPIO1) = (*(IO_MUX_GPIO1) &~(0b11111<<10)) | 0b10<<10;
  *(GPIO3_CONF_FUNC) = (*(GPIO3_CONF_FUNC) & ~(0b11111111)) | 0b10000011;
  *(IO_MUX_GPIO3) = (*(IO_MUX_GPIO3) &~(0b111111<<9)) | 0b000101<<9;
}

void ESP32::UART_Send(char* message) {

}

void ESP32::Bluetooth_Config() {

}

void ESP32::Bluetooth_Send(char* message) {

}

void ESP32::GPIO_Config(int pin, int mode) {

}

void ESP32::GPIO_Write(int pin, int state) {

}

int ESP32::GPIO_Read(int pin) {

}