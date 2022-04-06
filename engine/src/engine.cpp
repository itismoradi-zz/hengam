#include "engine.hpp"
#include <iostream>
#include <sstream>
#include <exception>
#include <ctime>

using namespace std;

Engine::Engine(short start, short end)
    : startJalaliYear(start), endJalaliYear(end),
    now(start, 1, 1, JALALI), final(end + 1, 1, 1, JALALI),
    file("hengam.calendar@gmail.com.ics", ios::out | ios::trunc),
    eng(static_cast<unsigned int>(time(0)))
{

}

void Engine::turnOn()
{
    //print calendar properties
    printCalendarProperties();
    cout << "hengam is calculating...\nplease wait" << endl;

    do
    {   
        //print loading characters to wait user
        if(((now.year() - startJalaliYear) % 5) == 0 && now.day() == 1 && now.month() == 1)
        {
            cout << "|";
        } 
        //print every day event properties
        file << "BEGIN:VEVENT" << endl;
        file << "DTSTART;VALUE=DATE:" << now.getGregorianDate() << endl;
        file << "DTEND;VALUE=DATE:" << now.getGregorianTomorrowDate() << endl;
        file << "DTSTAMP:" << Date::getSystemLocalDateTimeStr() << endl;
        printUID();
        file << "CREATED:" << Date::getSystemLocalDateTimeStr() << endl;
        file << "DESCRIPTION:" << endl;
        file << "LAST-MODIFIED:" << Date::getSystemLocalDateTimeStr() << endl;
        file << "LOCATION:" << endl;
        file << "SEQUENCE:" << 0 << endl;
        file << "STATUS:CONFIRMED" << endl;
        printSummary();
        file << "TRANSP:TRANSPARENT" << endl;
        file << "END:VEVENT" << endl;
        now.gotoTomorrow();
    }
    while (now != final);

    file << "END:VCALENDAR" << endl;

    cout << "\nhengam calendar icalendar file exported." << endl;
    cout << "file name is -------> hengam.calendar@gmail.com.ics" << endl;
    cout << "import this file to your phone or laptop calendar application,"
    " plan your time and enjoy!" << endl;
}

void Engine::printCalendarProperties()
{
    file << "BEGIN:VCALENDAR" << endl;
    file << "PRODID:-//hengam//hengam Calendar//EN" << endl;
    file << "VERSION:2.0" << endl;
    file << "CALSCALE:GREGORIAN" << endl;
    file << "METHOD:PUBLISH" << endl;
    file << "X-WR-CALNAME:هنگام" << endl;
    file << "X-WR-TIMEZONE:Asia/Tehran" << endl;
    file << "X-WR-CALDESC:تقویم خورشیدی با روز های هفته و مناسبت ها" << endl;
}

void Engine::printSummary()
{
    file << "SUMMARY:" << now.year() << "/" << now.month() << "/" << now.day() << endl;
}

void Engine::printUID()
{
    stringstream randomStream;
    string uid;
    uniform_int_distribution<unsigned int> r(0, 25);

    for (size_t i = 0; i < 32; i++)
    {
        short number = r(eng);
        bool isNumeric = number % 2;

        if(isNumeric)   //numeric character
        {
            randomStream << number % 10;
        }
        else            //alphabetic character
        {
            bool isCapital = r(eng) % 2;

            if(isCapital) 
            {
                randomStream << static_cast<char>(number + 'A');
            }
            else
            {
                randomStream << static_cast<char>(number + 'a');
            }
        }
    }

    randomStream >> uid;
    uid.append("@hengam.calendar");
    file << "UID:" << uid << endl;
}