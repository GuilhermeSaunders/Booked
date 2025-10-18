#include "authentication.h"
#include <functional>
#include <string>

using namespace std;

Authentication::Authentication() {
    username = "";
    passwordHash = 0;
    email = "";
    cpf = "";
    birthday = "";
    
}

size_t Authentication::hashPassword(const string& password) {
    hash<string> hasher;
    return hasher(password); 
}

bool Authentication::createUser(string& username, string& password, string& email, string& cpf, string& birthday) {
    if (username.length() < 3 || username.length() > 15) { return false; } ;

    if (email.find('@') == string::npos) { return false; };

    if (password.length() <= 5) { return false; };

    if (cpf.length() != 11) { return false; };

    if ((birthday.length() != 10) && (birthday[2] != '/' || birthday[5] != '/')) { return false; };

    this->username = username;
    this->passwordHash = hashPassword(password);
    this->email = email;
    this->cpf = cpf;
    this->birthday = birthday;

    return true;
}

