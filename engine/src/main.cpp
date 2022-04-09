/*
    hengam engine
    automatic creation processor of the hengam calendar
*/

#include <iostream>
#include <exception>
#include "engine.hpp"
using namespace std;

int main()
{
    short startJalaliYear;
    short endJalaliYear;

    Engine::splashScreen();

    cout << "Enter years in Jalali calendar" << endl;
    cout << "    >) Enter << start >> year : ";
    cin >> startJalaliYear;
    cout << "    >) Enter <<  end  >> year : ";
    cin >> endJalaliYear;
    
    try
    {
        Engine engine(startJalaliYear, endJalaliYear);
        engine.turnOn();
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }

    cout << "program finished" << endl;
    system("pause");
    return 0;
}