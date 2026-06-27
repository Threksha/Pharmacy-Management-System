#include "PharmacySystem.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>

using namespace std;

PharmacySystem::PharmacySystem() {
    owner = new Owner("owner", "ownerpass");
    bankBalance = 10000.0;
    loadInitialData();
}

PharmacySystem::~PharmacySystem() {
    delete owner;
}

void PharmacySystem::loadInitialData() {
    staff.add(Staff("staff1", "pass1"));
    staff.add(Staff("staff2", "pass2"));
    staff.add(Staff("staff3", "pass3"));

    try {
        medicines.add(Medicine("Paracetamol", 20.0, 50, Date::fromString("01-06-2026")));
        medicines.add(Medicine("Aspirin", 15.0, 40, Date::fromString("15-03-2027")));
        medicines.add(Medicine("Amoxicillin", 50.0, 30, Date::fromString("10-01-2025")));
    } catch (...) {
        Date t;
        medicines.add(Medicine("Paracetamol", 20.0, 50, t.addYears(1)));
        medicines.add(Medicine("Aspirin", 15.0, 40, t.addYears(1)));
        medicines.add(Medicine("Amoxicillin", 50.0, 30, t.addYears(1)));
    }
}

void PharmacySystem::login() {
    string user, pass;
    cout << "\n=== Pharmacy System Login ===\n";
    cout << "Enter username: ";
    cin >> user;
    cout << "Enter password: ";
    cin >> pass;

    if (owner->getUsername() == user && owner->checkPassword(pass)) {
        ownerMenu(owner);
        return;
    }

    for (auto& s : staff.getAll()) {
        if (s.getUsername() == user && s.checkPassword(pass)) {
            staffMenu(&s);
            return;
        }
    }

    cout << "Invalid login.\n";
}

void PharmacySystem::searchMedicine() {
    string name;
    cout << "Enter medicine name: ";
    cin >> name;

    Medicine* m = medicines.searchByName(name);
    if (m) cout << *m << endl;
    else cout << "Medicine not found.\n";
}

void PharmacySystem::displayMedicines() {
    cout << "\n--- Available Medicines ---\n";
    for (auto& m : medicines.getAll()) cout << m << endl;
}

void PharmacySystem::alerts() {
    cout << "\n--- Alerts ---\n";
    Date today;
    bool any = false;
    for (auto& m : medicines.getAll()) {
        if (m.getQuantity() < 10) {
            cout << m.getName() << " (with expiry "<< m.getExpiry().toString() << ") stock low! (Qty: " << m.getQuantity() << ")\n";
            any = true;
        }
        if (m.getExpiry() <= today) {
            cout << m.getName() << " expired on " << m.getExpiry().toString() << "!\n";
            any = true;
        }
    }
    if (!any) cout << "No alerts.\n";
}

void PharmacySystem::sellMedicine() {
    string name;
    int qty;
    cout << "Enter medicine name to sell: ";
    cin >> name;
    cout << "Enter quantity: ";
    cin >> qty;

    // Collect all medicines with same name
    vector<Medicine*> same;
    for (auto& m : medicines.getAll())
        if (m.getName() == name && Date() < m.getExpiry())
            same.push_back(&m);

    if (same.empty()) {
        cout << "Medicine not found or expired.\n";
        return;
    }

    // Sort by earliest expiry first
    sort(same.begin(), same.end(), [](Medicine* a, Medicine* b){
        return a->getExpiry() < b->getExpiry();
    });

    int remaining = qty;
    double saleAmount = 0;

    for (auto* m : same) {
        if (remaining == 0) break;

        int sellNow = min(remaining, m->getQuantity());
        if (sellNow > 0) {
            double cost = m->getPrice();
            double sellPrice = cost + 5; // 5 rupees extra
            saleAmount += sellNow * sellPrice;
            m->setQuantity(m->getQuantity() - sellNow);
            remaining -= sellNow;
        }
    }

    if (remaining > 0) {
        cout << "Not enough stock.\n";
        return;
    }

    bankBalance += saleAmount;
    totalProfit += saleAmount - (qty * (same[0]->getPrice())); // profit counted only from extra
    transactions.push_back({"Sale", saleAmount, name});
    cout << "Sold " << qty << " units of " << name << ". Amount: " << saleAmount << "\n";
}

void PharmacySystem::viewBankBalance() {
    cout << "Bank Balance: " << bankBalance << "\n";
}

void PharmacySystem::viewStaff() {
    cout << "\n--- Staff Members ---\n";
    for (auto& s : staff.getAll()) cout << "- " << s.getUsername() << "\n";
}

void PharmacySystem::addStaff() {
    string u, p;
    cout << "New staff username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;
    staff.add(Staff(u, p));
    cout << "Staff added.\n";
}

void PharmacySystem::deleteStaff() {
    string u;
    cout << "Enter staff username to delete: ";
    cin >> u;
    staff.removeByName(u);
    cout << "Staff deleted if existed.\n";
}

void PharmacySystem::buyStock() {
    string name;
    int qty;
    double pricePerUnit;
    cout << "Enter medicine name: ";
    cin >> name;
    cout << "Enter quantity to buy: ";
    cin >> qty;
    cout << "Enter price per unit: ";
    cin >> pricePerUnit;

    string expiryStr;
    cout << "Enter expiry date (dd-mm-yyyy): ";
    cin >> expiryStr;

    Date expiry;
    try {
        expiry = Date::fromString(expiryStr);
    } catch (...) {
        cout << "Invalid expiry format. Aborting purchase.\n";
        return;
    }

    Date today;
    if (expiry <= today) {
        cout << "Invalid expiry date. Must be in the future.\n";
        return;
    }

    double totalCost = qty * pricePerUnit;
    if (totalCost > bankBalance) {
        cout << "Insufficient bank balance.\n";
        return;
    }

    bankBalance -= totalCost;
    totalProfit-=totalCost;
    transactions.push_back({"Purchase", totalCost, name});

    bool foundSame = false;
    for (auto &med : medicines.getAll()) {
        if (med.getName() == name && med.getPrice() == pricePerUnit && med.getExpiry() == expiry) {
            med.setQuantity(med.getQuantity() + qty);
            foundSame = true;
            cout << "Updated existing medicine batch.\n";
        break;
        }
    }

    if (!foundSame) {
        medicines.add(Medicine(name, pricePerUnit, qty, expiry));
        cout << "New medicine batch added.\n";
    }
}

void PharmacySystem::viewTransactions() {
    cout << "\n--- Transactions ---\n";
    if (transactions.empty()) {
        cout << "No transactions yet.\n";
        return;
    }
    for (auto& t : transactions)
        cout << t.type << " | Amount: " << t.amount << " | Medicine: " << t.medicine << "\n";

    cout << "\nTotal Profit: " << totalProfit << " rupees\n";
}

void PharmacySystem::staffMenu(User* u) {
    int choice = 0;
    while (choice != 6) {
        u->menu();
        std::cin >> choice;

        if (choice == 1) u->updatePassword();
        else if (choice == 2) searchMedicine();
        else if (choice == 3) alerts();
        else if (choice == 4) sellMedicine();
        else if (choice == 5) displayMedicines();
        else if (choice == 6) std::cout << "Logging out...\n";
        else std::cout << "Invalid option.\n";
    }
}

void PharmacySystem::ownerMenu(User* u) {
    int choice = 0;
    while (choice != 12) {
        u->menu();
        std::cin >> choice;

        switch (choice) {
            case 1: addStaff(); break;
            case 2: deleteStaff(); break;
            case 3: viewStaff(); break;
            case 4: viewBankBalance(); break;
            case 5: viewTransactions(); break;
            case 6: buyStock(); break;
            case 7: displayMedicines(); break;
            case 8: searchMedicine(); break;
            case 9: alerts(); break;
            case 10: sellMedicine(); break;
            case 11: u->updatePassword(); break;
            case 12: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid option.\n"; break;
        }
    }
}