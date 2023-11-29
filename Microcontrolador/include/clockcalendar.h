#ifndef CLOCKCALENDAR_H_INCLUDED
#define CLOCKCALENDAR_H_INCLUDED
//#pragma once

#include "clock.h"
#include "calendar.h"

class ClockCalendar : public Clock, public Calendar {
public:
  ClockCalendar ();
  ClockCalendar (int mt, int d, int y, int h, int m, int s, int pm);
  ~ClockCalendar();
  void advance ();
  void subtract(ClockCalendar);
  bool greater_than(ClockCalendar);
  bool smaller_than(ClockCalendar);
  Clock full_time();
  friend void operator++ (ClockCalendar&);
  friend ClockCalendar operator- (const ClockCalendar&,const ClockCalendar&);
  bool operator == (const ClockCalendar&) const;
  bool operator < (const ClockCalendar&) const;
  bool operator > (const ClockCalendar&) const;
  bool operator != (const ClockCalendar&) const;
};


#endif