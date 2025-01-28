#include "MinUnit.h"
#include "CalendarTime.h"

static const uint8_t calendarWeekTable[12] = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };
static const uint8_t calendarMonthtable[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

bool IsLeapYear(uint16_t year)
{
    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
                return true;
            else
                return false;
        } else
            return true;
    } else
        return false;
}

uint8_t GetWeekday(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp2;
    uint8_t  yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    if (yearH > 19)
        yearL += 100;

    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + calendarWeekTable[month - 1];
    if (yearL % 4 == 0 && month < 3)
        temp2--;

    return (temp2 % 7);
}

uint32_t CalendarToUnixTime32(const struct CalendarTime *calendar) {
    uint16_t t;
    uint32_t seccount = 0;

    if (calendar->year < 1970 || calendar->year > 2099)
        return 0;

    for (t = 1970; t < calendar->year; t++) {
        if (IsLeapYear(t))
            seccount += 31622400;
        else
            seccount += 31536000;
    }

    uint8_t smon = calendar->month - 1;
    for (t = 0; t < smon; t++) {
        seccount += (uint32_t)calendarMonthtable[t] * 86400;
        if (IsLeapYear(calendar->year) && t == 1)
            seccount += 86400;
    }

    seccount += (uint32_t)(calendar->day - 1) * 86400;
    seccount += (uint32_t)calendar->hour * 3600;
    seccount += (uint32_t)calendar->min * 60;
    seccount += calendar->sec;

    return seccount;
}

void UnixTime32ToCalendarTime(uint32_t timecount, struct CalendarTime *calendar) {
    static uint16_t daycnt = 0;
    uint32_t temp = 0;
    uint16_t temp1 = 0;

    temp = timecount / 86400;
    if (daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while (temp >= 365)
        {
            if (IsLeapYear(temp1))
            {
                if (temp >= 366)
                    temp -= 366;
                else
                {
                    temp1++;
                    break;
                }
            } else
                temp -= 365;
            temp1++;
        }
        calendar->year = temp1;

        temp1 = 0;
        while (temp >= 28)
        {
            if (IsLeapYear(calendar->year) && temp1 == 1)
            {
                if (temp >= 29)
                    temp -= 29;
                else
                    break;
            } else
            {
                if (temp >= calendarMonthtable[temp1])
                    temp -= calendarMonthtable[temp1];
                else
                    break;
            }
            temp1++;
        }
        calendar->month = temp1 + 1;
        calendar->day = temp + 1;
    } else {
        calendar->year = 1970;
        calendar->month = 1;
        calendar->day = 1;
    }

    temp = timecount % 86400;
    calendar->hour = temp / 3600;
    calendar->min = (temp % 3600) / 60;
    calendar->sec = (temp % 3600) % 60;
    calendar->weekday = GetWeekday(calendar->year, calendar->month, calendar->day);
}

const char *CalendarTime_UnitTest(void) {
    // Test IsLeapYear function
    mu_assert("Year 2000 should be a leap year", IsLeapYear(2000) == true);
    mu_assert("Year 1900 should not be a leap year", IsLeapYear(1900) == false);
    mu_assert("Year 2020 should be a leap year", IsLeapYear(2020) == true);
    mu_assert("Year 2021 should not be a leap year", IsLeapYear(2021) == false);

    // Test GetWeekday function
    mu_assert("January 1, 1970 should be a Thursday (4)", GetWeekday(1970, 1, 1) == 4);
    mu_assert("February 28, 2000 should be a Monday (1)", GetWeekday(2000, 2, 28) == 1);
    mu_assert("March 1, 2000 should be a Wednesday (3)", GetWeekday(2000, 3, 1) == 3);
    mu_assert("December 31, 1999 should be a Friday (5)", GetWeekday(1999, 12, 31) == 5);

    // Test CalendarToUnixTime32 function
    struct CalendarTime time1 = { 0, 0, 0, 1970, 1, 1, 4 }; // January 1, 1970
    mu_assert("January 1, 1970 should be 0 in Unix time", CalendarToUnixTime32(&time1) == 0);

    struct CalendarTime time2 = { 0, 0, 0, 2000, 1, 1, 6 }; // January 1, 2000
    mu_assert("January 1, 2000 should be 946684800 in Unix time", CalendarToUnixTime32(&time2) == 946684800);

    // Test UnixTime32ToCalendarTime function
    struct CalendarTime calendarTime;
    UnixTime32ToCalendarTime(0, &calendarTime);
    mu_assert("Unix time 0 should be January 1, 1970",
        calendarTime.year == 1970 && calendarTime.month == 1 && calendarTime.day == 1);

    UnixTime32ToCalendarTime(946684800, &calendarTime);
    mu_assert("Unix time 946684800 should be January 1, 2000",
        calendarTime.year == 2000 && calendarTime.month == 1 && calendarTime.day == 1);

    return 0;
}
