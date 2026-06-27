#include "user.h"
#include <iostream>

using namespace std;

User::User(const string &u, const string &p) : username(u), password(p) {}

string User::getUsername() const { return username; }
bool User::checkPassword(const string &p) const { return p == password; }

void User::updatePassword() {
    string newpass;
    cout << "Enter new password: ";
    cin >> newpass;
    password = newpass;
    cout << "Password updated successfully.\n";
}

Staff::Staff(const string &u, const string &p) : User(u, p) {}
void Staff::menu() { 
std::cout << "\n--- Staff Menu ---\n";
        std::cout << "1. Update Password\n2. Search Medicine\n3. Alerts\n4. Sell Medicine\n5. Display Medicines\n6. Logout\nEnter: ";

}

Owner::Owner(const string &u, const string &p) : User(u, p) {}
void Owner::menu() { 
    std::cout << "\n--- Owner Menu ---\n";
    std::cout << "1. Add Staff\n2. Delete Staff\n3. View Staff\n4. View Bank Balance\n5. Transactions\n6. Buy Stock\n7. Display Medicines\n8. Search Medicine\n9. Alerts\n10. Sell Medicine\n11. Update Password\n12. Logout\nEnter: ";
 }
