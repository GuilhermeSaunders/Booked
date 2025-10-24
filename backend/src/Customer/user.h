#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

class User {
private:
    string cpf;        // User CPF
    string fullname;   // Full name of the user
    string email;      // User email
    string birthday;   // user birthday

public:
    // Constructor - CREATE
    User(const string& i_email,
            const string& i_cpf,
            const string& i_fullname, const string& i_birthday);
    
    // Getters - READ
    string getCpf() const;
    string getFullname() const;
    string getEmail() const;
    string getBirthday() const;

    //cod de daniel

    //COLOCAR BIRTHDAY!!!!!!!!!!!!!! 
    void update_cpf(const string& new_cpf); // Update CPF or full name
    void update_fullname(const string& new_fullname);
    void update_birthday(const string& new_birthday);
};

#endif