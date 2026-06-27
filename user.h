#ifndef USER_H
#define USER_H
using namespace std;

#include <string>

class User {
protected:
    string username;
    string password;
public:
    User(const string &u = "", const string &p = "");
    virtual ~User() {}
    virtual void menu() = 0;

    string getUsername() const;
    bool checkPassword(const string &p) const;
    void updatePassword();
};

class Staff : public User {
public:
    Staff(const string &u = "", const string &p = "");
    void menu() override;
};

class Owner : public User {
public:
    Owner(const string &u = "", const string &p = "");
    void menu() override;
};

#endif
