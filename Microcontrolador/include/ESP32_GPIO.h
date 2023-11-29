#ifndef ESP32_GPIO_H_INCLUDED
#define ESP32_GPIO_H_INCLUDED

#pragma once
#include <cstdint>

#define setBit(valor,bit)     (valor |= (1<<bit))
#define clearBit(valor,bit)   (valor &= ~(1<<bit))
#define testBit(valor,bit)    (valor & (1<<bit))
#define toggleBit(valor, bit)  (valor ^= (1<<bit))

volatile uint32_t *GPIO_BASE_REG = (volatile uint32_t *)0x3ff44000;

//Ativando as saídas dos pinos 0 até 31 (OFFSETS)
volatile uint32_t *GPIO_OUTPUT = (volatile uint32_t *)0x0004;
volatile uint32_t *GPIO_OUT_ENABLE = (volatile uint32_t *)0x0020;
volatile uint32_t *GPIO_CONF_PIN0 = (volatile uint32_t *)0x0088;
volatile uint32_t *GPIO_CONF_FUNC_OUT_0 = (volatile uint32_t *)0x0530;
//registradores de controle de entrada  (OFFSETS)
volatile uint32_t *GPIO_INPUT = (volatile uint32_t *)0x003c;
volatile uint32_t *GPIO_CONF_FUNC_IN_0 = (volatile uint32_t *)0x0130;
//Configurando os periféricos (OFFSETS)
volatile uint32_t *IO_MUX_REG_BASE = (volatile uint32_t *)0x3FF49000;

#define DR_REG_BT_BASE                          0x3ff51000
//Interrupt hardware source table
//This table is decided by hardware, don't touch this.
#define ETS_WIFI_MAC_INTR_SOURCE                0/**< interrupt of WiFi MAC, level*/
#define ETS_WIFI_MAC_NMI_SOURCE                 1/**< interrupt of WiFi MAC, NMI, use if MAC have bug to fix in NMI*/
#define ETS_WIFI_BB_INTR_SOURCE                 2/**< interrupt of WiFi BB, level, we can do some calibartion*/
#define ETS_BT_MAC_INTR_SOURCE                  3/**< will be cancelled*/
#define ETS_BT_BB_INTR_SOURCE                   4/**< interrupt of BT BB, level*/
#define ETS_BT_BB_NMI_SOURCE                    5/**< interrupt of BT BB, NMI, use if BB have bug to fix in NMI*/
#define ETS_RWBT_INTR_SOURCE                    6/**< interrupt of RWBT, level*/
#define ETS_RWBLE_INTR_SOURCE                   7/**< interrupt of RWBLE, level*/
#define ETS_RWBT_NMI_SOURCE                     8/**< interrupt of RWBT, NMI, use if RWBT have bug to fix in NMI*/
#define ETS_RWBLE_NMI_SOURCE                    9/**< interrupt of RWBLE, NMI, use if RWBT have bug to fix in NMI*/

class ESP32 {
public:
  ESP32();
  ~ESP32();
  void init();
  void UART_Config();
  void UART_Send(char*);
  void Bluetooth_Config();
  void Bluetooth_Send(char *);
  void GPIO_Config(int pin, int mode);
  void GPIO_Write(int pin, int state);
  int GPIO_Read(int pin);
};



#endif