#include "engine.hpp"
#include <iostream>
#include <exception>
using namespace std;

Engine::Engine(short start, short end)
    : startJalaliYear(start), endJalaliYear(end),
    now(start, 1, 1, JALALI)
{
    try
    {
        final.setDate(Date::dateString(end, 12, 30), JALALI);
    }
    catch(const std::exception& e)
    {
        final.setDate(Date::dateString(end, 12, 29), JALALI);
    }
}

void Engine::turnOn()
{
    
}
