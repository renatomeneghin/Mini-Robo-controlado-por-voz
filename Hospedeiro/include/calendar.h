#ifndef CALENDAR_H_INCLUDED
#define CALENDAR_H_INCLUDED
//#pragma once

class Calendar {
protected:
    int mo, day, yr;
public:
    Calendar (int m, int d, int y);
    void setCalendar (int m, int d, int y);
    void readCalendar (int *m, int *d, int *y);
    void advance ();
    friend void operator ++ (Calendar&);
    bool operator == (const Calendar&) const;
    bool operator < (const Calendar&) const;
    bool operator > (const Calendar&) const;
    bool operator != (const Calendar&) const;
};


#endif