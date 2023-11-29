#ifndef DRIVER_MOTORES_H_INCLUDED
#define DRIVER_MOTORES_H_INCLUDED
#pragma once

#include "Motor.h"
#include "driver/gpio.h"
#include "driver/mcpwm_prelude.h"
#include "esp_err.h"

class Driver_motor : public Motor{
  protected:
    mcpwm_timer_handle_t temporizador;
    mcpwm_oper_handle_t operador;
    mcpwm_cmpr_handle_t comparador;
    mcpwm_gen_handle_t geradores[2];
    int PWM_gpios[2];
  public:
    Driver_motor(uint16_t , uint8_t , uint32_t , uint32_t , int *);
    Driver_motor(uint16_t , uint8_t , uint32_t , uint32_t , int, int);
    Driver_motor(int pinos[2]);
    Driver_motor(int pinoA,int pinoB);
    ~Driver_motor();
    void init(void);
    void girar();
    void parar();
};

#endif