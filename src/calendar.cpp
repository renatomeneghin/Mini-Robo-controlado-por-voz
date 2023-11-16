#include "calendar.h"
// int mo, day, yr;

Calendar::Calendar (int m, int d, int y){
  this->mo = m;
  this->day = d;
  this->yr = y;
}

void Calendar::setCalendar (int m, int d, int y){
  this->mo = m;
  this->day = d;
  this->yr = y;
}

void Calendar::readCalendar (int *m, int *d,int *y){
  *m = this->mo;
  *d = this->day;
  *y = this->yr;
}

void Calendar::advance (){
  const int dias_no_mes[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  const int dias_no_mes_bissexto[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
  bool is_bissexto = !(yr%4);
  day++;
  if (is_bissexto){
    if (day > dias_no_mes_bissexto[mo]){ mo++;  day = 1; }
    if (mo > 12){ yr++;   mo = 1; }
  }
  else {
    if (day > dias_no_mes[mo]){ mo++;  day = 1; }
    if (mo > 12){ yr++;   mo = 1; }    
  }
}

 void operator++ (Calendar &t){t.advance();}