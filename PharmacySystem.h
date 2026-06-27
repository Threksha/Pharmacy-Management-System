#ifndef PHARMACYSYSTEM_H
#define PHARMACYSYSTEM_H

#include "medicine.h"
#include "user.h"
#include "date.h"
#include <vector>
#include <string>

using namespace std;

//Template
template <typename T>
class Repository {
private:
    vector<T> items;
public:
    void add(const T& item) { items.push_back(item); }
    vector<T>& getAll() { return items; }

    T* searchByName(const string& name) {
        for (auto& item : items) {
            if (item.getName() == name) return &item;
        }
        return nullptr;
    }

    void removeByName(const string& name) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->getUsername() == name) {
                items.erase(it);
                return;
            }
        }
    }
};

struct Transaction {
    string type;
    double amount;
    string medicine;
};

class PharmacySystem {
private:
    Owner* owner;
    Repository<Staff> staff;
    Repository<Medicine> medicines;
    vector<Transaction> transactions;
    double bankBalance;
    double totalProfit = 0;
public:
    PharmacySystem();
    ~PharmacySystem();

    void loadInitialData();
    void login();
    void staffMenu(User* u);
    void ownerMenu(User* u);

    void searchMedicine();
    void displayMedicines();
    void alerts();
    void sellMedicine();
    void viewBankBalance();
    void viewStaff();
    void addStaff();
    void deleteStaff();
    void buyStock();
    void viewTransactions();
};

#endif