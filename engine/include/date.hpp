#ifndef DATE_IG
#define DATE_IG

#include <string>

enum JalaliMonth {FARVARDIN = 1, ORDIBEHESHT, KHORDAD, TIR, MORDAD, SHAHRIVAR, MEHR, ABAN, AZAR, DEY, BAHMAN, ESFAND};
enum GregorianMonth {JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER};
enum Type {JALALI = 0, GREGORIAN = 1};

class Date 
{
public:
    Date();
    Date(std::string, Type);  //example: ("14001125", JALALI)
    Date(short, short, short, Type);

    bool isJalali() const;
    bool isGregorian() const;
    Type type() const;
    void setDate(std::string, Type);
    bool isValidDate(short, short, short, Type);
    bool isLeapJalaliYear(short);
    bool isLeapGregorianYear(short);
    std::string getJalaliDate();
    std::string getGregorianDate();
    void convertDate();
    short day() const;
    short month() const;
    std::string monthStr() const;
    short year() const;
    static std::string dateString(short, short, short);
private:
    Type type_;
    short day_;
    short month_;
    short year_;
    
};

#endif