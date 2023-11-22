#include "../include/clockcalendar.h"

ClockCalendar::ClockCalendar() : Clock(-1, -1, -1, -1), Calendar(-1, -1, -1) {}
ClockCalendar::ClockCalendar(int mt, int d, int y, int h, int m, int s, int pm)
    : Clock(h, m, s, pm), Calendar(mt, d, y) {}
ClockCalendar::~ClockCalendar() {}

void ClockCalendar::advance() {
  int wasPm = is_pm; // mude de AM para PM.
  Clock::advance();
  if (wasPm && !is_pm)
    Calendar::advance();
}

void operator++(ClockCalendar &cc) { cc.advance(); }

bool operator==(ClockCalendar &cc1,ClockCalendar &cc2)
{
  int d1, m1, a1, h1, mn1, s1, p1, d2, m2, a2, h2, mn2, s2, p2;
  cc1.readCalendar(&m1,&d1,&a1);  
  cc1.readClock(&h1,&s1,&mn1,&p1);
  cc2.readCalendar(&m2,&d2,&a2);  
  cc2.readClock(&h2,&s2,&mn2,&p2);
  return ((h1 == h2) && (mn1 == mn2) && (s1 == s2) && (p1 == p2) && (a1 == a2) && (m1 == m2) && (d1 == d2));
}

bool operator!=(ClockCalendar &cc1,ClockCalendar &cc2)
{
  int d1, m1, a1, h1, mn1, s1, p1, d2, m2, a2, h2, mn2, s2, p2;
  cc1.readCalendar(&m1,&d1,&a1);  
  cc1.readClock(&h1,&s1,&mn1,&p1);
  cc2.readCalendar(&m2,&d2,&a2);  
  cc2.readClock(&h2,&s2,&mn2,&p2);
  return ((h1 != h2) || (mn1 != mn2) || (s1 != s2) || (p1 != p2) || (a1 != a2) || (m1 != m2) || (d1 != d2));
}

bool operator<(ClockCalendar &cc1,ClockCalendar &cc2)
{
  bool resultado = false;
  int d1, m1, a1, h1, mn1, s1, p1, d2, m2, a2, h2, mn2, s2, p2;
  cc1.readCalendar(&m1,&d1,&a1);  
  cc1.readClock(&h1,&s1,&mn1,&p1);
  cc2.readCalendar(&m2,&d2,&a2);  
  cc2.readClock(&h2,&s2,&mn2,&p2);
  Calendar c1(m1,d1,a1),c2(m2,d2,a2); 
  Clock cl1(h1,s1,mn1,p1), cl2(h2,s2,mn2,p2);
  if(c1 < c2){
    resultado = true;
  } else if(c1 == c2){
    resultado = (cl1 < cl2);
  }
  return resultado;
}

bool operator>(ClockCalendar &cc1,ClockCalendar &cc2)
{
  bool resultado = false;
  int d1, m1, a1, h1, mn1, s1, p1, d2, m2, a2, h2, mn2, s2, p2;
  cc1.readCalendar(&m1,&d1,&a1);  
  cc1.readClock(&h1,&s1,&mn1,&p1);
  cc2.readCalendar(&m2,&d2,&a2);  
  cc2.readClock(&h2,&s2,&mn2,&p2);
  Calendar c1(m1,d1,a1),c2(m2,d2,a2); 
  Clock cl1(h1,s1,mn1,p1), cl2(h2,s2,mn2,p2);
  if(c1 > c2){
    resultado = true;
  } else if(c1 == c2){
    resultado = (cl1 > cl2);
  }
  return resultado;
}