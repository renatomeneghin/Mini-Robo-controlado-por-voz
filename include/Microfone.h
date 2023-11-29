#ifndef MICROFONE_H_INCLUDED
#define MICROFONE_H_INCLUDED
#pragma once

#include <inttypes.h>
#define BUFFER_LEN 16000

class Microfone {
protected:
  float Som_Buffer[BUFFER_LEN];
  uint32_t frequencia;
public:
  Microfone();
  Microfone(uint32_t);
  void set_freq(uint32_t);
  uint32_t get_freq();
  virtual void init() = 0;
  virtual void hearSound() = 0;
  float* getSom();
  operator float*();
};

 
#endif