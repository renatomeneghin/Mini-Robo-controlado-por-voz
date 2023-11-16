#include "clock.h"
// int hr, min, sec, is_pm;

Clock::Clock (int h, int s, int m, int pm){
  this->hr = h;
  this->sec = s;
  this->min = m;
  this->is_pm = pm;
  this->mudanca = false;
}

void Clock::setClock (int h, int s, int m, int pm){
  this->hr = h;
  this->sec = s;
  this->min = m;
  this->is_pm = pm;
  this->mudanca = false;
}

void Clock::readClock (int *h, int *s, int *m, int *pm){
  *h = this->hr;
  *s = this->sec;
  *m = this->min;
  *pm = this->is_pm;
}

void Clock::advance (){
  sec++;
  if (sec >= 60){ min++;  sec %= 60; }
  if (min >= 60){ hr++;   min %= 60; }
  if (hr >= 12 && mudanca){ is_pm = !is_pm;}
  if (hr > 12) {hr %= 12;}
  mudanca = (hr != 12);  
}

void operator++ (Clock& C){C.advance();}