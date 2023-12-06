#include "../include/clock.h"
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

void Clock::subtract(Clock relogio1){
  int hora, minuto, segundo, pm;
  relogio1.readClock(&hora,&segundo,&minuto,&pm);
  this->hr = (this->hr + 12 * bool(this->is_pm))-(hora + 12 * bool(pm));
  this->min -= minuto;
  this->sec -= segundo;

  while (this->sec < 0){
    this->min--;
    this->sec += 60;
  }
  while (this->min < 0){
    this->hr--;
    this->min += 60;
  }

}

void operator++ (Clock& C){C.advance();}

Clock operator-(const Clock &clock1,const Clock &clock2) {
  Clock temp = clock1;
  temp.subtract(clock2);
  return temp;
}

bool Clock::operator==(const Clock &clock) const
{
  int h1, m1, s1, p1;//, h2, m2, s2, p2;
  Clock relogio = clock;
  relogio.readClock(&h1,&s1,&m1,&p1);
  //clock2.readClock(&h2,&s2,&m2,&p2);
  return ((hr == h1) && (min == m1) && (sec == s1) && (is_pm == p1));
}

bool Clock::operator!=(const Clock &clock) const
{
  int h1, m1, s1, p1;//, h2, m2, s2, p2;
  Clock relogio = clock;
  relogio.readClock(&h1,&s1,&m1,&p1);
  //clock2.readClock(&h2,&s2,&m2,&p2);
  return ((hr != h1) || (min != m1) || (sec != s1) || (is_pm != p1));
}

bool Clock::operator<(const Clock &clock) const
{
  bool resultado = false;
  int h1, m1, s1, p1;//, h2, m2, s2, p2;
  Clock relogio = clock;
  relogio.readClock(&h1,&s1,&m1,&p1);
  //clock2.readClock(&h2,&s2,&m2,&p2);
  if(!bool(is_pm) && bool(p1)){
    resultado = true;
  }
  else if (bool(is_pm) == bool(p1)){
    if(hr < h1){
      resultado = true;
    }
    else if (hr == h1){
      if(min < m1){
        resultado = true;
      }
      else if (min == m1){
        if (sec < s1){
          resultado = true;
          }
      }
    }
  }
  return resultado;
}

bool Clock::operator>(const Clock &clock) const
{
  int h1, m1, s1, p1;//, h2, m2, s2, p2;
  bool resultado = false;
  Clock relogio = clock;
  relogio.readClock(&h1,&s1,&m1,&p1);
  //clock2.readClock(&h2,&s2,&m2,&p2);
  if(bool(is_pm) && !bool(p1)){
    resultado = true;
  }
  else if (bool(is_pm) == bool(p1)){
    if(hr > h1){
      resultado = true;
    }
    else if (hr == h1){
      if(min > m1){
        resultado = true;
      }
      else if (min == m1){
        if (sec > s1){
          resultado = true;
          }
      }
    }
  }
  return resultado;
}