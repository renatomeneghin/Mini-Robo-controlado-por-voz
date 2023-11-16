#include "clockcalendar.h"

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