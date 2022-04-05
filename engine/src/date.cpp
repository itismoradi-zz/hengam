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
    try
    {
       isValidDate(y, m, d, type);
       year_ = y;
       month_ = m;
       day_ = d;
       type_ = type;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
        throw;
    }
}

bool Date::isValidDate(short y, short m, short d, Type type)
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
        if(y < 1178 || y > 1633)    //Jalali algorithm supports only [1178, 1633] range
        {
            throw invalid_argument("invalid Jalali year input");
        }
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
        if( (y == 1799 && m == MARCH && d < 21) || (y < 1799))   //first valid Gregorian date
        {
            throw invalid_argument("invalid Gregorian year input");
        }
        else if((y == 2254 && m == MARCH && d < 20) || (y > 2254))  //last valid Gregorian date
        {
            throw invalid_argument("invalid Gregorian year input");
        }
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

bool Date::isLeapJalaliYear(short year)
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

bool Date::isLeapGregorianYear(short year)
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
    if(isGregorian())
        convertDate();

    return dateString(year_, month_, day_);
}

string Date::getGregorianDate()
{
    if(isJalali())
        convertDate();

    return dateString(year_, month_, day_);
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