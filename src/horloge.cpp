#include <DS1307.h>

DS1307 clock;

typedef struct Horloge
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} Horloge;

void setupClock(int year, int month, int day, int dayName, int hour, int minute, int second)
{
    clock.begin();
    clock.fillByYMD(year, month, day);
    clock.fillByHMS(hour, minute, second);
    clock.fillDayOfWeek(dayName);
    clock.setTime();
}

void setNow(Horloge *horloge)
{
    clock.getTime();
    horloge->year = clock.year;
    horloge->month = clock.month;
    horloge->day = clock.dayOfMonth;
    horloge->hour = clock.hour;
    horloge->minute = clock.minute;
    horloge->second = clock.second;
}

String getTime()
{
    String timeString;
    clock.getTime();
    switch (clock.dayOfWeek) // Friendly printout the weekday
    {
    case MON:
        timeString += String("LUN");
        break;
    case TUE:
        timeString += String("MAR");
        break;
    case WED:
        timeString += String("MER");
        break;
    case THU:
        timeString += String("JEU");
        break;
    case FRI:
        timeString += String("VEN");
        break;
    case SAT:
        timeString += String("SAM");
        break;
    case SUN:
        timeString += String("DIM");
        break;
    }
    timeString += "-";
    timeString += String(clock.year, DEC);
    timeString += "-";
    timeString += String(clock.month, DEC);
    timeString += "-";
    timeString += String(clock.dayOfMonth, DEC);
    timeString += ";";
    timeString += String(clock.hour, DEC);
    timeString += ":";
    timeString += String(clock.minute, DEC);
    timeString += ":";
    timeString += String(clock.second, DEC);
    return timeString;
}

String getDate()
{
    clock.getTime();
    return String(clock.year, DEC) + "-" + String(clock.month, DEC) + "-" + String(clock.dayOfMonth, DEC);
}