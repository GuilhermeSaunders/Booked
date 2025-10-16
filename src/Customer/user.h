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
            const string& i_fullname);
    
    // Getters - READ
    string getCpf() const { return cpf; };
    string getFullname() const { return fullname; };
    string getEmail() const { return email; };

    //cod de daniel

    //COLOCAR BIRTHDAY!!!!!!!!!!!!!! 
    int update_info(const string& new_cpf, const string& new_fullname, int update); // Update CPF or full name

};

#endif