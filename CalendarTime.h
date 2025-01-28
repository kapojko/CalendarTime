#ifndef CALENDARTIME_H
#define CALENDARTIME_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t UnixTime32;

struct CalendarTime {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t weekday;
};

bool IsLeapYear(uint16_t year);
uint8_t GetWeekday(uint16_t year, uint8_t month, uint8_t day);

uint32_t CalendarToUnixTime32(const struct CalendarTime *calendar);
void UnixTime32ToCalendarTime(uint32_t timecount, struct CalendarTime *calendar);


#endif // CALENDARTIME_H
