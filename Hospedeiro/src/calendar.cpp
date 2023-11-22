#include "../include/calendar.h"
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


bool operator==(Calendar &t,Calendar&calendar1)
{
  int d1, m1, a1, d2, m2, a2;

  t.readCalendar(&m1,&d1,&a1);
  calendar1.readCalendar(&m2,&d2,&a2);
  
  return ((a1 == a2) && (m1 == m2) && (d1 == d2));
}

bool operator!=(Calendar &t,Calendar&calendar1)
{
  int d1, m1, a1, d2, m2, a2;

  t.readCalendar(&m1,&d1,&a1);
  calendar1.readCalendar(&m2,&d2,&a2);
  return ((a1 != a2) || (m1 != m2) || (d1 != d2));
}

bool operator<(Calendar &t,Calendar&calendar1)
{
  bool resultado = false;
  int d1, m1, a1, d2, m2, a2;

  t.readCalendar(&m1,&d1,&a1);
  calendar1.readCalendar(&m2,&d2,&a2);
  if(a1 < a2){
    resultado = true;
  }
  else if (a1 == a2){
    if(m1 < m2){
      resultado = true;
    }
    else if (m1 == m2){
      if (d1 < d2){
        resultado = true;
        }
    }
  }
  return resultado;
}

bool operator>(Calendar &t,Calendar&calendar1)
{
  bool resultado = false;
  int d1, m1, a1, d2, m2, a2;

  t.readCalendar(&m1,&d1,&a1);
  calendar1.readCalendar(&m2,&d2,&a2);
  if(a1 > a2){
    resultado = true;
  }
  else if (a1 == a2){
    if(m1 > m2){
      resultado = true;
    }
    else if (m1 == m2){
      if (d1 > d2){
        resultado = true;
        }
    }
  }
  return resultado;
}