#ifndef ENGINE_IG
#define ENGINE_IG

#include <date.hpp>

class Engine
{
public:
    Engine(short, short);
    void turnOn();
private:
    short startJalaliYear;
    short endJalaliYear;
    Date now;
    Date final;
};

#endif