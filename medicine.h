#ifndef MEDICINE_H
#define MEDICINE_H

#include <string>
#include "date.h"
#include <iostream>

using namespace std;

class Medicine {
private:
    string name;
    double price;
    int quantity;
    Date expiry;
public:
    Medicine();
    Medicine(const string &n, double p, int q, const Date &e);

    string getName() const;
    double getPrice() const;
    int getQuantity() const;
    Date getExpiry() const;

    void setQuantity(int q);
    void setExpiry(const Date &d);

    friend ostream& operator<<(ostream& out, const Medicine& m);
};

#endif 
