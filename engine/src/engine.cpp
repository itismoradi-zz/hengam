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
    setSummaryMode();
    weekday = Date::weekDay(now.getGregorianDate());
}

void Engine::turnOn()
{
    //print calendar properties
    printCalendarProperties();
    cout << "\nhengam is calculating...\nplease wait" << endl;

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

    cout << "\n\nhengam calendar icalendar file exported." << endl;
    cout << "file name is -------> hengam.calendar@gmail.com.ics" << endl;
    cout << "you can import this file to your calendar application"
    " and work with Jalali calendar" << endl;
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

void Engine::setSummaryMode()
{
    char ch;
    string command;
    cout << "\nPersonalize the title of your calendar days" << endl;
    //year
    do
    {
        cout << "    >) Should the title contain the << year >> ? (y/n) ";
        cin >> command;
        if(command.size() != 1)
        {
            continue;
        }
        ch = tolower(command.at(0));
    } 
    while (ch != 'y' && ch != 'n'); 
    mode.year = (ch == 'y') ? true : false;
    //month
    do
    {
        cout << "    >) Should the title contain the << months >> ? (y/n) ";
        cin >> command;
        if(command.size() != 1)
        {
            continue;
        }
        ch = tolower(command.at(0));
    } 
    while (ch != 'y' && ch != 'n'); 

    if(ch == 'y')
    {
        //month name
        mode.month = true;
        do
        {
            cout << "        >) does show month name instead month number? like Farvardin (y/n) ";
            cin >> command;
            if(command.size() != 1)
            {
                continue;
            }
            ch = tolower(command.at(0));
        } 
        while (ch != 'y' && ch != 'n'); 
        mode.monthName = (ch == 'y') ? true : false;
    }
    else
    {
        mode.month = false;
        mode.monthName = false;
    }
    
    //day
    do
    {
        cout << "    >) Should the title contain the << days >> ? (y/n) ";
        cin >> command;
        if(command.size() != 1)
        {
            continue;
        }
        ch = tolower(command.at(0));
    } 
    while (ch != 'y' && ch != 'n'); 
    mode.day = (ch == 'y') ? true : false;
    //weekday
    do
    {
        cout << "    >) Should the title contain the << week days name >> like yekshanbeh ? (y/n) ";
        cin >> command;
        if(command.size() != 1)
        {
            continue;
        }
        ch = tolower(command.at(0));
    } 
    while (ch != 'y' && ch != 'n'); 
    mode.weekday = (ch == 'y') ? true : false;
}

void Engine::printSummary()
{
    file << "SUMMARY:";
    short y = now.year();

    if(mode.weekday)
    {
        file << getPersianWeekDay(this->weekday);

        if(mode.day || mode.month || mode.year)
        {
            file << " ";    //space character to beautify event title in calendar views
        }
    }

    if(mode.day)
    {
        file << now.day();

        if(mode.monthName)
        {
            file << " ";
        }
        else
        {
            file << "/";
        } 
    }

    if(mode.month)
    {
        if(mode.monthName)      //month name like Bahman
        {
            file << getPersianMonth(static_cast<JalaliMonth>(now.month()));
            file << " ";
        }
        else    //month number like 11
        {
            if(y < 10)
            {
                file << 0;
            }
            file << now.month();

            if(mode.year)
            {
                file << "/";
            }
        }
    }

    if(mode.year)
    {
        if(y < 10)
        {
            file << 0 << 0 << 0;
        }
        else if(y < 100)
        {
            file << 0 << 0;
        }
        else if(y < 1000)
        {
            file << 0;
        }
        file << y;
    }

    file << endl;
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
        cout << "\n------- warning -----------------------------------------------------" << endl;
        cout << "hengam engine has used Jalali algorithim to convert calendar dates" << endl;
        cout << "The Jalali algorithm for the years outside 1178 to 1633 solar may" << endl;
        cout << "not have the same leaps as the official iran calendars." << endl;
        
        char command;

        do
        {
            cout << "do yo want to continue? (y/n) ";
            cin >> command;
            command = tolower(command);
        } while (command != 'y' && command != 'n'); 

        cout << "---------------------------------------------------------------------" << endl;

        if(command == 'n')
        {
            throw invalid_argument("program finished");
        }

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
        default : throw invalid_argument("invalid week day sent to getPersianWeekDay");
    }
}

string Engine::getPersianMonth(JalaliMonth m)
{
    switch (m)
    {
        case FARVARDIN   : return "فروردین";
        case ORDIBEHESHT : return "اردیبهشت";
        case KHORDAD     : return "خرداد";
        case TIR         : return "تیر";
        case MORDAD      : return "مرداد";
        case SHAHRIVAR   : return "شهریور";
        case MEHR        : return "مهر";
        case ABAN        : return "آبان";
        case AZAR        : return "آذر";
        case DEY         : return "دی";
        case BAHMAN      : return "بهمن";
        case ESFAND      : return "اسفند";
        default : throw invalid_argument("invalid month number sent to getPersianMonth");
    }
}

WeekDay operator++(WeekDay & d, int)
{
    d = static_cast<WeekDay>((static_cast<short>(d) + 1) % 7);
    return d;
}
void Engine::splashScreen()
{
    cout << "      /------------------------\\\n"
                "      |                        |\n"
                "      |  SA SU MO TU WE TH FR  |          /----------\\\n"
                "      |      1  2  3  4  5  6  |         /|          |\\\n"
                "      |   7  8  9 10 11 12 13  |         ++  hengam  ++\n"
                "      |  14 15 16 17 18 19 20  |         ++  engine  ++\n"
                "      |  21 22 23 24 25 26 27  |         \\|          |/\n"
                "      |  28 29 30 31           |          |          |\n"
                "      |                        |          |  !!!!!!  |\n"
                "      \\------------------------//         |          |\n"
                "     / - - - - - - - - - - - -//          \\-------#--/\n"
                "    / - - - - - - - - - - - -//                   |\n"
                "   / - - - - - - - - - - - -//                    |\n"
                "  / - - - - - - - - - - - -// <-------------------+\n"
                " /---------/----/---------//\n"
                "/         /    /         // \n"
                "\\------------------------/\n";
    cout << endl; //https://asciiflow.com/#/
    cout << "|||  automatic creation processor of the hengam calendar  |||\n" << endl;
    cout << "hengam engine converts gregorian years to jalali calendar years\n"
    "and exports .ics file. you can import exported file to your laptop,\n"
    "pc or phone calendar app and use jalali calendar, plan and enjoy.\n" << endl;
}