#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED
#pragma once

#include <stdint.h>

class Motor {
protected:
  uint16_t Duty_Cicle;
  uint8_t Direcao;
  uint32_t Frequencia;
  uint32_t Periodo;
  
public:
  Motor(uint16_t, uint8_t, uint32_t, uint32_t);
  Motor();
  virtual void init() = 0;
  void setDuty (uint16_t);
  uint16_t getDuty ();
  void setDirecao (uint8_t Direcao);
  uint8_t getDirecao ();
  void setFrequencia (uint32_t freq);
  uint32_t getFrequencia ();
  void setPeriodo (uint32_t period);
  uint32_t getPeriodo ();
  virtual void girar() = 0;
  virtual void parar() = 0;
};


#endif