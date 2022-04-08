#include "engine.hpp"
#include <iostream>
#include <sstream>
#include <exception>
#include <ctime>
#include <vector>
#include "picosha2.h"

using namespace std;
WeekDay operator++(WeekDay &, int);

Engine::Engine(short start, short end)
    : startJalaliYear(start), endJalaliYear(end),
    now(start, 1, 1, JALALI), final(end + 1, 1, 1, JALALI),
    file("hengam.calendar@gmail.com.ics", ios::out | ios::trunc),
    eng(static_cast<unsigned int>(time(0)))
{
    validateStartEndJalaliYear();
    weekday = Date::weekDay(now.getGregorianDate());
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
        weekday++;
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
    string src_str = now.getJalaliDate();

    vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(src_str.begin(), src_str.end(), hash.begin(), hash.end());

    string uid = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
    
    uid.append("@hengam.calendar");
    file << "UID:" << uid << endl;
}

bool Engine::validateStartEndJalaliYear()
{
    if(startJalaliYear > endJalaliYear)
    {
        throw invalid_argument("start year must be earlier than end year");
    }

    if(startJalaliYear < 1178 || endJalaliYear > 1633)    //Jalali algorithm supports only [1178, 1633] range
    {
        cout << "------- warning -----------------------------------------------------" << endl;
        cout << "hengam engine has used Jalali algorithim to convert calendar dates" << endl;
        cout << "The Jalali algorithm for the years outside 1178 to 1633 solar" << endl;
        cout << "may not have the same leaps as the official iran calendars" << endl;
        
        char command;

        do
        {
            cout << "do yo want to continue? (y/n) ";
            cin >> command;
            command = tolower(command);
        } while (command != 'y' && command != 'n'); 

        if(command == 'n')
        {
            throw invalid_argument("program finished");
        }
        
        cout << "---------------------------------------------------------------------" << endl;
    }
}

string Engine::getPersianWeekDay(WeekDay d)
{
    switch (d)
    {
        case SATURDAY  : return "شنبه";
        case SUNDAY    : return "یکشنبه";
        case MONDAY    : return "دوشنبه";
        case TUESDAY   : return "سه شنبه";
        case WEDNESDAY : return "چهارشنبه";
        case THURSDAY  : return "پنجشنبه";
        case FRIDAY    : return "جمعه";
    }
}

WeekDay operator++(WeekDay & d, int)
{
    d = static_cast<WeekDay>((static_cast<short>(d) + 1) % 7);
    return d;
}