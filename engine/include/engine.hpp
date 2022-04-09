#ifndef ENGINE_IG
#define ENGINE_IG

#include <date.hpp>
#include <fstream>
#include <random>

struct SummaryMode
{   
    //if each data member is true prints in event summary property
    bool weekday;
    bool year;
    bool month;
    bool monthName;
    bool day;
};

class Engine
{
public:
    Engine(short, short);
    void turnOn();
    void printCalendarProperties();
    void setSummaryMode();
    void printSummary();
    void printUID();
    bool validateStartEndJalaliYear();
    std::string getPersianWeekDay(WeekDay);
    std::string getPersianMonth(JalaliMonth);
    static void splashScreen();
private:
    short startJalaliYear;
    short endJalaliYear;
    Date now;
    Date final;
    std::ofstream file;
    std::mt19937 eng;
    WeekDay weekday;
    SummaryMode mode;
};

#endif