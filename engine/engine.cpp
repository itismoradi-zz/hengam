#include "engine.hpp"
#include "lib/src/converter.cpp"
#include <iostream>
#include <exception>
using namespace std;

Engine::Engine(short start, short end)
    : startJalaliYear(start), endJalaliYear(end)
{

}

void Engine::turnOn()
{
    if(startJalaliYear < 1178 || endJalaliYear > 1633)
    {
        //The 33-year period of the jalali algorithm is responsible only in this period = [1178, 1633]
        throw invalid_argument("please select the years between 1178 and 1633 AH \n"
        "See for more information : https://github.com/SCR-IR/date-algorithms-article");
    }
}
