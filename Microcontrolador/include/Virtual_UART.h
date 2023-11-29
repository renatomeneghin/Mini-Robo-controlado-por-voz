#ifndef VIRTUAL_UART_H_INCLUDED
#define VIRTUAL_UART_H_INCLUDED
#pragma once

class Virtual_UART {
private:
  int Tx;
  int Rx;
  int baudRate;
  int dataBits;
  bool parity;
public:
  Virtual_UART();
  Virtual_UART(int, int, int, int, bool);
  Virtual_UART(int, int);
  ~Virtual_UART();
  void setTx(int);
  void setRx(int);
  void setBaudRate(int);
  void setDataBits(int);
  void setParity(bool);
  int getTx();
  int getRx();
  int getBaudRate();
  int getDataBits();
  bool getParity();
  void send(char);
  char receive();
};

#endif