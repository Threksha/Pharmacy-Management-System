#include "medicine.h"
#include <iomanip>

Medicine::Medicine() : name(""), price(0.0), quantity(0), expiry() {}

Medicine::Medicine(const string &n, double p, int q, const Date &e)
    : name(n), price(p), quantity(q), expiry(e) {}

string Medicine::getName() const { return name; }
double Medicine::getPrice() const { return price; }
int Medicine::getQuantity() const { return quantity; }
Date Medicine::getExpiry() const { return expiry; }

void Medicine::setQuantity(int q) { quantity = q; }
void Medicine::setExpiry(const Date &d) { expiry = d; }

ostream& operator<<(ostream& out, const Medicine& m) {
    out << "Name: " << m.name
        << " | Price: " << m.price + 5
        << " | Qty: " << m.quantity
        << " | Expiry: " << m.expiry.toString();
    return out;
}
