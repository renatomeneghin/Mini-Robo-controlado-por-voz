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
  friend void operator++ (Clock&);
  
	bool operator==( const Clock& ) const;
	bool operator!=( const Clock& ) const;
	bool operator<( const Clock& ) const;
	bool operator>( const Clock& ) const;
};


#endif