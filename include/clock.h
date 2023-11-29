#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED
//#pragma once

class Clock {
protected:
  int hr, min, sec, is_pm;
  bool mudanca;
public:
  Clock (int h, int s, int m, int pm);
  void setClock (int h, int s, int m,
  int pm);
  void readClock (int *h, int *s,
  int *m, int *pm);
  void advance ();
  void subtract(Clock);
  friend void operator++ (Clock&);
  friend Clock operator-(const Clock&,const Clock&);
  bool operator==( const Clock& ) const;
  bool operator!=( const Clock& ) const;
  bool operator<( const Clock& ) const;
  bool operator>( const Clock& ) const;
};


#endif