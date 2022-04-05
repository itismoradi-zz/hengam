#ifndef ENGINE_IG
#define ENGINE_IG

#include <date.hpp>
#include <fstream>

class Engine
{
public:
    Engine(short, short);
    void turnOn();
    void printCalendarProperties();
    void printSummary();
private:
    short startJalaliYear;
    short endJalaliYear;
    Date now;
    Date final;
    std::ofstream file;
};

#endif