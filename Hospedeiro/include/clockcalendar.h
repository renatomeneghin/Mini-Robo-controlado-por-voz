#ifndef CLOCKCALENDAR_H_INCLUDED
#define CLOCKCALENDAR_H_INCLUDED
#pragma once

#include "clock.h"
#include "calendar.h"

class ClockCalendar : public Clock, public Calendar {
public:
  ClockCalendar ();
  ClockCalendar (int mt, int d, int y, int h, int m, int s, int pm);
  ~ClockCalendar();
  void advance ();
  friend void operator++ (ClockCalendar&);
  friend bool operator == (ClockCalendar&,ClockCalendar&);
  friend bool operator < (ClockCalendar&,ClockCalendar&);
  friend bool operator > (ClockCalendar&,ClockCalendar&);
  friend bool operator != (ClockCalendar&,ClockCalendar&);
};


#endif