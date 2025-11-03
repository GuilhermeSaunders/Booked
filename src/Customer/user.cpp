#include "user.h"
#include <utility> // Para std::move
using namespace std;

User::User(const string& i_email,
           const string& i_cpf,
           const string& i_fullname, const string& i_birthday)
    : id(-1),
      cpf(i_cpf),
      fullname(i_fullname),
      email(i_email),
      birthday(i_birthday)
{}

string User::getCpf() const { 
    return cpf; 
}
string User::getFullname() const { 
    return fullname; 
}
string User::getEmail() const { 
    return email;
}
string User::getBirthday() const { 
    return birthday; 
}
int User:: getId() const {
    return this->id;
}
void User:: setId(int ind)  {
    this->id = ind;
}

// Update CPF or full name based on the 'update' code (UPDATE)
    void User::update_cpf(const string& new_cpf){
        cpf = new_cpf;
    }; // Update CPF or full name
    void User::update_fullname(const string& new_fullname){
        fullname = new_fullname;
    };
    void User::update_birthday(const string& new_birthday){
        birthday = new_birthday;
    };
