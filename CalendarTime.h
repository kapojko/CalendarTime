#ifndef CALENDARTIME_H
#define CALENDARTIME_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t UnixTime32;
typedef int32_t UnixTimeDiff32;

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

UnixTime32 CalendarToUnixTime32(const struct CalendarTime *calendar);
void UnixTime32ToCalendarTime(UnixTime32 timecount, struct CalendarTime *calendar);

UnixTimeDiff32 GetUnixTimeDiff(UnixTime32 time1, UnixTime32 time2);
UnixTimeDiff32 GetUnixTimeDiffAbs(UnixTime32 time1, UnixTime32 time2);

const char *CalendarTime_UnitTest(void);

#endif // CALENDARTIME_H
