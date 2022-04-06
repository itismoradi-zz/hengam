#include "date.hpp"
#include "../lib/src/converter.cpp"
#include <sstream>
#include <iostream>
#include <exception>
using namespace std;

Date::Date()
{
    Date("14010101", JALALI);   //default date
}

Date::Date(string dateStr, Type type)
{
    setDate(dateStr, type);
}

Date::Date(short y, short m, short d, Type type)
{
    setDate(dateString(y, m, d), type);
}

bool Date::operator==(Date date)
{
    if(date.year_ == this->year_ && date.month_ == this->month_ && date.day_ == this->day_)
    {
        return true;
    }
    
    return false;
}

bool Date::operator!=(Date date)
{
    return !(*this == date);
}

void Date::gotoTomorrow()
{
    string tomorrow;

    if(isJalali())
    {
        tomorrow = getJalaliTomorrowDate();
        //converting tomorrow date string to integer
        year_ = stoi(tomorrow.substr(0, 4));
        month_ = stoi(tomorrow.substr(4, 2));
        day_ = stoi(tomorrow.substr(6, 2));
    }
    else if(isGregorian())
    {
        tomorrow = getGregorianTomorrowDate();
        //converting tomorrow date string to integer
        year_ = stoi(tomorrow.substr(0, 4));
        month_ = stoi(tomorrow.substr(4, 2));
        day_ = stoi(tomorrow.substr(6, 2));
    }
}

string Date::getJalaliTomorrowDate() const
{
    short y = year_;
    short m = month_;
    short d = day_;

    if(m <= SHAHRIVAR)     //each month have 31 days
    {
        if(d < 31)
        {
            d++;
        }
        else if(d == 31)
        {
            d = 1;
            m++;
        }
    }
    else if(m >= MEHR && m != ESFAND)     //each month have 30 days
    {
        if(d < 30)
        {
            d++;
        }
        else if(d == 30)
        {
            d = 1;
            m++;
        }
    }
    else if(m == ESFAND)       //Esfand month has 29 days(30 days in leap years)
    {
        if(d < 29)
        {
            d++;
        }
        else if(isLeapJalaliYear(y))
        {
            if(d == 29)
            {
                d++;
            }
            else if(d == 30)
            {
                d = 1;
                m = 1;
                y++;
            }
        }
        else
        {
            if(d == 29)
            {
                d = 1;
                m = 1;
                y++;
            }
        }
    }

    //validate calculated date
    try
    {
        isValidDate(y, m, d, JALALI);
        return dateString(y, m, d);
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << endl;
        cerr << "error to go to tomorrow in Jalali calendar" << endl;
    }
}

string Date::getGregorianTomorrowDate() const
{
    short y = year_;
    short m = month_;
    short d = day_;
    short y2;
    short m2;
    short d2;

    if(isJalali())
    {
        long convertedDate[3];
        jalali_to_gregorian(y, m, d, convertedDate);
        y2 = convertedDate[0];
        m2 = convertedDate[1];
        d2 = convertedDate[2];
    }
    else if(isGregorian())
    {
        y2 = y;
        m2 = m;
        d2 = d;
    }
    
    //each month have 31 days
    if(m2 == JANUARY || m2 == MARCH || m2 == MAY || m2 == JULY || m2 == AUGUST || m2 == OCTOBER)
    {
        if(d2 < 31)
        {
            d2++;
        }
        else if(d2 == 31)
        {
            d2 = 1;
            m2++;
        }
    }
    //each month have 30 days
    else if(m2 == APRIL || m2 == JUNE || m2 == SEPTEMBER || m2 == NOVEMBER)
    {
        if(d2 < 30)
        {
            d2++;
        }
        else if(d2 == 30)
        {
            d2 = 1;
            m2++;
        }
    }
    //second month(FEBRUARY) in Gregorian calendar has 28 days but in leap year has 29 days
    else if(m2 == FEBRUARY)
    {
        if(d2 < 28)
        {
            d2++;
        }
        else if(isLeapGregorianYear(y2))
        {
            if(d2 == 28)
            {
                d2++;
            }
            else if(d2 == 29)
            {
                d2 = 1;
                m2 = MARCH;
            }
        }
        else
        {
            if(d2 == 28)
            {
                d2 = 1;
                m2 = MARCH;
            }
        }
    }
    //December is last month in year and have 31 days
    else if(m2 == DECEMBER)
    {
        if(d2 < 31)
        {
            d2++;
        }
        else if(d2 == 31)
        {
            d2 = 1;
            m2 = JANUARY;
            y2++;
        }
    }

    //validate calculated date
    try
    {
        isValidDate(y2, m2, d2, GREGORIAN);
        return dateString(y2, m2, d2);
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << endl;
        cerr << "error to go to tomorrow in Gregorian calendar" << endl;
    }
}


bool Date::isJalali() const
{
    if(type_ == JALALI)
        return true;
    else 
        return false;
}

bool Date::isGregorian() const
{
    return !isJalali();
}

Type Date::type() const
{
    return type_;
}

void Date::setDate(string dateStr, Type type)
{
    if(dateStr.size() != 8)     //valid date string : year[4] month[2] day[2] : "14001105"
        throw invalid_argument("invalid date string");

    //converting date string to integer
    short y = stoi(dateStr.substr(0, 4));
    short m = stoi(dateStr.substr(4, 2));
    short d = stoi(dateStr.substr(6, 2));
    
    //validation
    if(isValidDate(y, m, d, type))
    {
       year_ = y;
       month_ = m;
       day_ = d;
       type_ = type;
    }
}

bool Date::isValidDate(short y, short m, short d, Type type) const
{
    bool returnValue = false;

    if(m < 1 || m > 12)
    {
        throw invalid_argument("invalid month input");
    }
    if(d < 1 || d > 31)
    {
        throw invalid_argument("invalid day input");
    }

    if(type == JALALI)
    {
        //Jalali calendar rules
        if((m < MEHR && d > 31) || (m < ESFAND && m > SHAHRIVAR && d > 30))
        {
            throw invalid_argument("invalid Jalali day of month input");
        }
        //check leap year
        if(m == ESFAND && d > 29)
        {
            if(d != 30)
            {
                throw invalid_argument("invalid Gregorian day of Esfand month input");
            }
            else if(!isLeapJalaliYear(y))
            {
                //last month(Esfand) in jalali calendar has 29 days but in leap year has 30 days
                throw invalid_argument("invalid Jalali non leap year day input");
            }
        }
    }
    else if(type == GREGORIAN)
    {
        //Gregorian calendar rules
        if(m == JANUARY || m == MARCH || m == MAY || m == JULY || m == AUGUST || m == OCTOBER || m == DECEMBER)
        {
            if(d > 31)
            {
               throw invalid_argument("invalid Gregorian day of month input");
            }
        }
        if(m == APRIL || m == JUNE || m == SEPTEMBER || m == NOVEMBER)
        {
            if(d > 30)
            {
               throw invalid_argument("invalid Gregorian day of month input");
            }
        }
        //check leap year
        if(m == FEBRUARY && d > 28)
        {
            if(d != 29)
            {
                throw invalid_argument("invalid Gregorian day of Feb month input");
            }
            else if(!isLeapGregorianYear(y))
            {
                //second month(FEBRUARY) in Gregorian calendar has 28 days but in leap year has 29 days
                throw invalid_argument("invalid Jalali non leap year day input");
            }
        }
    }

    returnValue = true;
    return returnValue;
}

bool Date::isLeapJalaliYear(short year) const
{  
    //https://github.com/SCR-IR/date-algorithms-article
    if((((year + 12) % 33) % 4) == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Date::isLeapGregorianYear(short year) const
{
    //https://www.tutorialspoint.com/cplusplus-program-to-check-leap-year
    if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

string Date::getJalaliDate()
{
    if(isJalali())
    {
        return dateString(year_, month_, day_);
    }
    else if(isGregorian())
    {
        long convertedDate[3];
        gregorian_to_jalali(year_, month_, day_, convertedDate);
        short y = convertedDate[0];
        short m = convertedDate[1];
        short d = convertedDate[2];
        return dateString(y, m, d);
    }
}

string Date::getGregorianDate()
{
    if(isGregorian())
    {
        return dateString(year_, month_, day_);
    }
    else if(isJalali())
    {
        long convertedDate[3];
        jalali_to_gregorian(year_, month_, day_, convertedDate);
        short y = convertedDate[0];
        short m = convertedDate[1];
        short d = convertedDate[2];
        return dateString(y, m, d);
    }
}

std::string Date::dateString(short y, short m, short d)
{
    string dateString;      //return value
    stringstream dateStream;

    //add year
    dateStream << y;

    //add month
    if(m < 10)
    {
        dateStream << 0;
    }
    dateStream << m;

    //add day
    if(d < 10)
    {
        dateStream << 0;
    }
    dateStream << d;

    dateStream >> dateString;
    return dateString;
}

void Date::convertDate()
{
    if(isJalali())
    {
        long convertedDate[3];
        jalali_to_gregorian(year_, month_, day_, convertedDate);
        year_ = convertedDate[0];
        month_ = convertedDate[1];
        day_ = convertedDate[2];
        type_ = GREGORIAN;
    }
    else if(isGregorian())
    {
        long convertedDate[3];
        gregorian_to_jalali(year_, month_, day_, convertedDate);
        year_ = convertedDate[0];
        month_ = convertedDate[1];
        day_ = convertedDate[2];
        type_ = JALALI;
    }
}

struct tm Date::getSystemLocalDateTime()
{
    time_t now = time(0);
    tm * itm = localtime(&now);
    return *itm;
}

std::string Date::getSystemLocalDateTimeStr()
{
    struct tm dt = getSystemLocalDateTime();
    string str = dateString(1900 + dt.tm_year, dt.tm_mon + 1, dt.tm_mday);
    string time;
    str += "T";
    stringstream i;

    //add hour
    if(dt.tm_hour < 10)
    {
        i << 0;
    }
    i << dt.tm_hour;
    //add minute
    if(dt.tm_min < 10)
    {
        i << 0;
    }
    i << dt.tm_min;
    //add second
    if(dt.tm_sec < 10)
    {
        i << 0;
    }
    i << dt.tm_sec;

    i >> time;
    str.append(time);
    str += "Z";
    return str;
}


short Date::day() const
{
    return day_;
}

short Date::month() const
{
    return month_;
}

std::string Date::monthStr() const
{
    if(isJalali())
    {
        switch (month_)
        {
            case 1: return "Farvardin";
            case 2: return "Ordibehesht";
            case 3: return "Khordad";
            case 4: return "Tir";
            case 5: return "Mordad";
            case 6: return "Shahrivar";
            case 7: return "Mehr";
            case 8: return "Aban";
            case 9: return "Azar";
            case 10: return "Dey";
            case 11: return "Bahman";
            case 12: return "Esfand";
            default:
                throw;
        }
    }
    else if(isGregorian())
    {
        switch (month_)
        {
            case 1: return "January";
            case 2: return "February";
            case 3: return "March";
            case 4: return "April";
            case 5: return "May";
            case 6: return "June";
            case 7: return "July";
            case 8: return "August";
            case 9: return "September";
            case 10: return "October";
            case 11: return "November";
            case 12: return "December";
            default:
                throw;
        }
    }
}

short Date::year() const
{
    return year_;
}
