#include "../include/Motor.h"

Motor::Motor(uint16_t Duty, uint8_t Direcao, uint32_t freq, uint32_t period)
{
  this->Duty_Cicle = Duty;
  this->Direcao = Direcao;
  this->Frequencia = freq;
  this->Periodo = period;
}

Motor::Motor()
{
  this->Duty_Cicle = 50;
  this->Direcao = 1;
  this->Frequencia = 1000000;
  this->Periodo = 100;
}

void Motor::setDuty (uint16_t Duty) {this->Duty_Cicle = Duty;}
uint16_t Motor::getDuty (){return this->Duty_Cicle;}
void Motor::setDirecao (uint8_t Direcao) {this->Direcao = Direcao;}
uint8_t Motor::getDirecao (){return this->Direcao;}
void Motor::setFrequencia(uint32_t Frequencia){this->Frequencia = Frequencia;}
uint32_t Motor::getFrequencia(){return this->Frequencia;}
void Motor::setPeriodo(uint32_t Periodo){this->Periodo = Periodo;}
uint32_t Motor::getPeriodo(){return this->Periodo;}
