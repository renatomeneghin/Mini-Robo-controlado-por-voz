#ifndef MICROFONE_I2S_H_INCLUDED
#define MICROFONE_I2S_H_INCLUDED
#pragma once

#include "Microfone.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "esp_err.h"

class Microfone_I2S : public Microfone {
protected:
  gpio_num_t pino_BCLK;
  gpio_num_t pino_WS;
  gpio_num_t pino_LR;
  gpio_num_t pino_DA;
  i2s_chan_handle_t rx_handle;
  i2s_port_t periferico;
  i2s_role_t funcao;
  i2s_data_bit_width_t comprimeto_dados;
  i2s_slot_bit_width_t comprimeto_pacote;
  i2s_slot_mode_t tipo_pacote;
  i2s_std_slot_mask_t mascara_pacote;
  soc_periph_i2s_clk_src_t clock_base;
  i2s_mclk_multiple_t multiplicador_clock;
public:
  Microfone_I2S(gpio_num_t,gpio_num_t,gpio_num_t,gpio_num_t);
  Microfone_I2S(uint32_t,gpio_num_t,gpio_num_t,gpio_num_t,gpio_num_t,i2s_port_t,
                i2s_role_t, i2s_data_bit_width_t, i2s_slot_bit_width_t,
                i2s_slot_mode_t, i2s_std_slot_mask_t,
                soc_periph_i2s_clk_src_t, i2s_mclk_multiple_t);
  ~Microfone_I2S();
  void init();
  void reconfig();
  void setPeriferico(i2s_port_t);
  void setFuncao(i2s_role_t);
  void setComprimentoDados(i2s_data_bit_width_t);
  void setComprimentoPacote(i2s_slot_bit_width_t);
  void setModoPacote(i2s_slot_mode_t);
  void setMascara(i2s_std_slot_mask_t);
  void setClockBase(soc_periph_i2s_clk_src_t);
  void setMultiplicadorClock(i2s_mclk_multiple_t);
  void hearSound();
};

#endif