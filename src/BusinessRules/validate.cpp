#include "validate.h"
#include "../Customer/user.h"
#include "../Customer/account.h"
#include <functional>
#include <string>
#include <cctype>
#include <random>

using namespace std;



size_t Validate::hashPassword(const string& password) {
    hash<string> hasher;
    return hasher(password); 
}

string Validate::generateId(int length = 6) { // generates a random ID
    string id;
    mt19937 rng(std::random_device{}());
    uniform_int_distribution<int> dist(0, 9);

    for (int i = 0; i < length; ++i)
        id += std::to_string(dist(rng));

    return id;
}

bool Validate::validateUsername(const string& username) {
    if (username.length() < 3 || username.length() > 15) {
        return false;
    } else {
        return true;
    }
}

bool Validate::validateStringPassword(const string& password) {
    if (password.length() < 6) {
        return false;
    } 

    bool hasUpper = false;   
    bool hasLower = false;   
    bool hasDigit = false;    
    bool hasSpecial = false;  

    for (char ch : password) {
        if (isupper(ch)) {
            hasUpper = true;
        } if (islower(ch)) {
            hasLower = true;
        } if (isdigit(ch)) {
            hasDigit = true;
        } if (ispunct(ch)) {
            hasSpecial = true;
        }
    }
    
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

bool Validate::validateEmail(const string& email) {
    if ((email.find('@') == string::npos) && (email.find(".com") == string::npos)) {
        return false;
    } else {
        return true;
    }
}

bool Validate::validateCpf(const string& cpf) {
    if (cpf.length() != 11) { 
        return false; 
    } else {
        return true;
    }
}

bool Validate::validateBirthday(const string& birthday) {
    if ((birthday.length() != 10) && (birthday[2] != '/' || birthday[5] != '/')) {
        return false;
    }

    for (char ch : birthday) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
    
}

bool Validate::validateFullname(const string& fullname) {

    if (fullname.length() < 5) {
        return false;
    } if (fullname.front() == ' ' || fullname.back() == ' ') {
        return false;
    }

    for (char ch : fullname) {
        
        if (std::isdigit(ch)) {
            return false;
        } if (std::ispunct(ch)) {

            if (ch != '-' && ch != '\'') {
                return false;
            }
        }
    }

    return true;
}

User Validate::createUser(const string& email, const string& cpf, const string& fullname, const string& birthday) {


    return User(email, cpf, fullname, birthday);
}

Account Validate::createAccount(const string& stringPassword, const string& username, const User& user) {
    size_t hash = hashPassword(stringPassword);
    string id = generateId();

    return Account(id, hash, username, user);
}
