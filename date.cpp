#include "date.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

Date::Date() {
    time_t t = time(nullptr);
    std::tm local = *std::localtime(&t);
    d = local.tm_mday;
    m = local.tm_mon + 1;
    y = local.tm_year + 1900;
}

Date::Date(int day, int mon, int year) : d(day), m(mon), y(year) {
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 0)
        throw invalid_argument("Invalid date");
}

Date Date::fromString(const string &s) {
    istringstream iss(s);
    int day, mon, year;
    char sep1, sep2;
    if (!(iss >> day >> sep1 >> mon >> sep2 >> year) || sep1 != '-' || sep2 != '-')
        throw invalid_argument("Date must be in dd-mm-yyyy format");
    return Date(day, mon, year);
}

string Date::toString() const {
    ostringstream oss;
    oss << setw(2) << setfill('0') << d << '-'
        << setw(2) << setfill('0') << m << '-'
        << setw(4) << setfill('0') << y;
    return oss.str();
}

bool Date::operator<(const Date &other) const {
    if (y != other.y) return y < other.y;
    if (m != other.m) return m < other.m;
    return d < other.d;
}

bool Date::operator==(const Date &other) const {
    return d == other.d && m == other.m && y == other.y;
}

bool Date::operator<=(const Date &other) const {
    return (*this < other) || (*this == other);
}

Date Date::addYears(int years) const {
    int newY = y + years;
    int newD = d;
    int newM = m;
    if (newM == 2 && newD == 29) {
        bool isLeap = ((newY % 4 == 0 && newY % 100 != 0) || (newY % 400 == 0));
        if (!isLeap) newD = 28;
    }
    return Date(newD, newM, newY);
}
