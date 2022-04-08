#ifndef ENGINE_IG
#define ENGINE_IG

#include <date.hpp>
#include <fstream>
#include <random>

class Engine
{
public:
    Engine(short, short);
    void turnOn();
    void printCalendarProperties();
    void printSummary();
    void printUID();
    bool validateStartEndJalaliYear();
    std::string getPersianWeekDay(WeekDay);
private:
    short startJalaliYear;
    short endJalaliYear;
    Date now;
    Date final;
    std::ofstream file;
    std::mt19937 eng;
    WeekDay weekday;
};

#endif