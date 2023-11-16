#ifndef MICROFONE_I2S_H_INCLUDED
#define MICROFONE_I2S_H_INCLUDED
#pragma once

#include "Microfone.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "esp_err.h"

class Microfone_I2S : Microfone {
protected:
  int pino_BCLK;
  int pino_WS;
  i2s_chan_handle_t rx_handle;
public:
  Microfone_I2S(int,int,int);
  ~Microfone_I2S();
  void setPino_BCLK(int);
  void setPino_WS(int);
  int getPino_BCLK();
  int getPino_WS();
  float hearSound();
  operator float();
};

#endif