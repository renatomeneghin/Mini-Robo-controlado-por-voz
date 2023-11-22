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
  bool greater_than(ClockCalendar);
  bool smaller_than(ClockCalendar);
  friend void operator++ (ClockCalendar&);
  bool operator == (const ClockCalendar&) const;
  bool operator < (const ClockCalendar&) const;
  bool operator > (const ClockCalendar&) const;
  bool operator != (const ClockCalendar&) const;
};


#endif