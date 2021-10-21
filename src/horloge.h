typedef struct Horloge
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} Horloge;

void setNow(Horloge *horloge);
void setupClock(int year, int month, int day, int dayName, int hour, int minute, int second);
String getTime();
String getDate();