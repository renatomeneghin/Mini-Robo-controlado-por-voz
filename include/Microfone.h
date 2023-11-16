#ifndef MICROFONE_H_INCLUDED
#define MICROFONE_H_INCLUDED
#pragma once

class Microfone {
protected:
  int pino_Data;
public:
  Microfone(int);
  ~Microfone();
  void setPino(int);
  int getPino();
  virtual float hearSound() = 0;
  operator float();
};

#endif