#ifndef DRIVER_MOTORES_H_INCLUDED
#define DRIVER_MOTORES_H_INCLUDED
#pragma once

#include "Motor.h"

#define N_MOTORES 2

class Driver_motor {
  protected:
    Motor Motores[N_MOTORES];
    int GPIO_pins[N_MOTORES];
  public:
    Driver_motor (Motor Motores_[N_MOTORES], int IO[N_MOTORES]);
    
};


#endif