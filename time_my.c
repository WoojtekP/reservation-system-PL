#include "time_my.h"

bool verifyTimeInput(Time_my start, Time_my end)
{
    if (end.year > start.year + 1)
        return false;
    if (start.month > 12 || end.month > 12)
        return false;
    int maxdays, m = start.month, d = start.day;
    for (int i = 0; i < 2; ++i){
        if (m == 4 || m == 6 || m == 9 || m == 11)
            maxdays = 30;
        else if (m == 2)
                if (isLeapYear(start.year))
                    maxdays = 29;
                else
                    maxdays = 28;
            else maxdays = 31;
        if (d > maxdays)
            return false;
        m = end.month;
        d = end.day;
    }
    if (isGreaterThan(start, end))
        return false;
    int s = getYearDay(start.year, start.month, start.day), e = getYearDay(end.year, end.month, end.day);
    if (e > s && s - e > TERM_DAYS)
        return false;
    if (s > e && 365 - s + e > TERM_DAYS)
        return false;
    return true;
}

bool isLeapYear(int y)
{
    return ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
}

int getWeekDay(int y, int m, int d)
{
    int r = (y - 1) % 100;
    int c = (y - 1) - r;
    int g = r + r / 4;
    int firstWeekDay = ((((c / 100) % 4) * 5) + g) % 7;
    return (firstWeekDay + getYearDay(y, m, d) - 1) % 7;
}

int getYearDay(int y, int m, int d)
{
    int day;
    switch(m){
        case 1: day = 0; break;
        case 2: day = 31; break;
        case 3: day = 59; break;
        case 4: day = 90; break;
        case 5: day = 120; break;
        case 6: day = 151; break;
        case 7: day = 181; break;
        case 8: day = 212; break;
        case 9: day = 243; break;
        case 10: day = 273; break;
        case 11: day = 304; break;
        case 12: day = 334; break;
    }
    day += d;
    if (isLeapYear(y) && m > 2)
        day++;
    return day;
}

Time_my getTimeStruct(int y, int m, int d)
{
    Time_my date;
    date.year = y;
    date.month = m;
    date.day = d;
    date.hours = 0;
    date.minutes = 0;
    date.weekDay = getWeekDay(y, m, d);
    return date;
}

bool isGreaterThan(Time_my date, Time_my comparator)
{
    if (date.year > comparator.year) return true;
    if (date.year < comparator.year) return false;
    if (date.month > comparator.month) return true;
    if (date.month < comparator.month) return false;
    if (date.day > comparator.day) return true;
    if (date.day < comparator.day) return false;
    if (date.hours > comparator.hours) return true;
    if (date.hours < comparator.hours) return false;
    if (date.minutes > comparator.minutes) return true;
    if (date.minutes < comparator.minutes) return false;
    return true;
}

bool getTime(int *h, int *m, const char *time)
{
    int len = strlen(time);
    if (len > 5 || len < 4)
        return false;
    int colon = 0;
    for (int i = 0; i < len; i++)
        if (time[i] == ':')
            colon++;
        else if (time[i] < '0' || time[i] > '9')
            return false;
    if (colon != 1)
        return false;
    if (sscanf(time, "%d:%d", h, m) != 2)
        return false;
    if (*h > 23)
        return false;
    if (*m > 59)
        return false;
    return true;
}

bool isTimeCorrect(Time_my *start, Time_my *end, GtkWidget *startDate, GtkWidget *endDate, GtkWidget *startTime, GtkWidget *endTime)
{
    int hr1, hr2, min1, min2;
    if ( !getTime(&hr1, &min1, gtk_entry_get_text(GTK_ENTRY(startTime))) ||
         !getTime(&hr2, &min2, gtk_entry_get_text(GTK_ENTRY(endTime)))) {
        return false;
    }
    guint y1, y2, mon1, mon2, day1, day2;
    gtk_calendar_get_date(GTK_CALENDAR(startDate), &y1, &mon1, &day1);
    gtk_calendar_get_date(GTK_CALENDAR(endDate), &y2, &mon2, &day2);
    mon1++; mon2++; //gtk calendar enumerates months from 0
    *start = getTimeStruct(y1, mon1, day1);
    *end = getTimeStruct(y2, mon2, day2);
    (*start).hours = hr1;
    (*start).minutes = min1;
    (*end).hours = hr2;
    (*end).minutes = min2;
    if (!isGreaterThan(*end, *start))
        return false;
    if (!isGreaterThan(*start, firstSemesterDay) || !isGreaterThan(lastSemesterDay, *end))
        return false;
    return true;
}

void addDays(Time_my *time, int days)
{
    int maxdays;
    switch(time->month){
        case 4:
        case 6:
        case 9:
        case 11: maxdays = 30; break;
        case 2:
            if (isLeapYear(time->year))
                maxdays = 29;
            else
                maxdays = 28;
            break;
        default: maxdays = 31;
    }
    if (time->day + days > maxdays){
        days = days - maxdays + time->day;
        time->day = 0;
        if (++time->month > 12){
            time->month = 1;
            ++time->year;
        }
        addDays(time, days);
    }
    else
        time->day += days;
}
