#ifndef DATE_H
#define DATE_H

using namespace std;

#include <string>

class Date {
private:
    int d;
    int m;
    int y;
public:
    Date(); // today
    Date(int day, int mon, int year);
    static Date fromString(const string &s); // expects "dd-mm-yyyy"
    string toString() const; // "dd-mm-yyyy"

    bool operator<(const Date &other) const;
    bool operator==(const Date &other) const;
    bool operator<=(const Date &other) const;

    Date addYears(int years) const;
};

#endif // DATE_H
