/*
    hengam engine
    automatic creation process of the hengam calendar
*/

#include <iostream>
#include <exception>
#include "engine.hpp"
using namespace std;

int main()
{
    short startJalaliYear;
    short endJalaliYear;

    cout << "hengam engine" << endl;
    cout << "Enter << start >> year in jalali calendar : ";
    cin >> startJalaliYear;
    cout << "Enter << end >> year in jalali calendar : ";
    cin >> endJalaliYear;

    Engine engine(startJalaliYear, endJalaliYear);
    
    try
    {
        engine.turnOn();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}