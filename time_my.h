#ifndef TIME_MY_H_INCLUDED
#define TIME_MY_H_INCLUDED

#include "common.h"

bool verifyTimeInput(Time_my start, Time_my end); //verify if given dates of start and end of the semester are correct
bool isLeapYear(int y);
int getWeekDay(int y, int m, int d); //return number of weekday: 0 - monday, 1 - tuesday ...
int getYearDay(int y, int m, int d); //return number of day of the year
Time_my getTimeStruct(int y, int m, int d); //generate structure which represents date
bool isGreaterThan(Time_my date, Time_my comparator); //check if date >= comparator
bool getTime(int *h, int *m, const char *time); //convert char* form format hh:mm into two integers
bool isTimeCorrect(Time_my *start, Time_my *end, GtkWidget *startDate, GtkWidget *endDate, GtkWidget *startTime, GtkWidget *endTime);
void addDays(Time_my *time, int days);

Time_my firstSemesterDay, lastSemesterDay;
int startDay, startMonth, startYear, endDay, endMonth, endYear;
#endif // TIME_MY_H_INCLUDED
