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

void operator++ (Clock& C){C.advance();}

bool operator==(Clock &clock1,Clock &clock2)
{
  int h1, m1, s1, p1, h2, m2, s2, p2;
  clock1.readClock(&h1,&s1,&m1,&p1);
  clock2.readClock(&h2,&s2,&m2,&p2);
  return ((h1 == h2) && (m1 == m2) && (s1 == s2) && (p1 == p2));
}

bool operator!=(Clock &clock1,Clock &clock2)
{
  int h1, m1, s1, p1, h2, m2, s2, p2;
  clock1.readClock(&h1,&s1,&m1,&p1);
  clock2.readClock(&h2,&s2,&m2,&p2);
  return ((h1 != h2) || (m1 != m2) || (s1 != s2) || (p1 != p2));
}

bool operator<(Clock &clock1,Clock &clock2)
{
  bool resultado = false;
  int h1, m1, s1, p1, h2, m2, s2, p2;
  clock1.readClock(&h1,&s1,&m1,&p1);
  clock2.readClock(&h2,&s2,&m2,&p2);
  if(!p1 && p2){
    resultado == true;
  }
  else if (p1 == p2){
    if(h1 < h2){
      resultado = true;
    }
    else if (h1 == h2){
      if(m1 < m2){
        resultado = true;
      }
      else if (m1 == m2){
        if (s1 < s2){
          resultado = true;
          }
      }
    }
  }
  return resultado;
}

bool operator>(Clock &clock1,Clock &clock2)
{
  bool resultado = false;
  int h1, m1, s1, p1, h2, m2, s2, p2;
  clock1.readClock(&h1,&s1,&m1,&p1);
  clock2.readClock(&h2,&s2,&m2,&p2);
  if(p1 && !p2){
    resultado == true;
  }
  else if (p1 == p2){
    if(h1 > h2){
      resultado = true;
    }
    else if (h1 == h2){
      if(m1 > m2){
        resultado = true;
      }
      else if (m1 == m2){
        if (s1 > s2){
          resultado = true;
          }
      }
    }
  }
  return resultado;
}