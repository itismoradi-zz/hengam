#include "engine.hpp"
#include <iostream>
#include <exception>

using namespace std;

Engine::Engine(short start, short end)
    : startJalaliYear(start), endJalaliYear(end),
    now(start, 1, 1, JALALI), final(end + 1, 1, 1, JALALI),
    file("hengam.calendar@gmail.com.ics", ios::out | ios::trunc)
{

}

void Engine::turnOn()
{
    //print calendar properties
    printCalendarProperties();

    do
    {   
        //print every day event properties
        file << "BEGIN:VEVENT" << endl;
        file << "DTSTART;VALUE=DATE:" << endl;
        file << "DTEND;VALUE=DATE:" << endl;
        file << "DTSTAMP:" << Date::getSystemLocalDateTimeStr() << endl;
        file << "UID:" << endl;
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