#include "authentication.h"
#include <functional>
#include <string>

using namespace std;

Authentication::Authentication() {
    username = "";
    passwordHash = 0;
    email = "";
    loggedIn = false;
    failedAttempts = 0;
}

size_t Authentication::hashPassword(const string& password) {
    hash<string> hasher;
    return hasher(password); // retorna um número único representando a senha
}

void Authentication::createUser(string username, string password, string email) {
    if ((username.length() > 0 && username.length() < 15) && (password.length() > 5)) {
        this->username = username;
        passwordHash = hashPassword(password); 
        this->email = email;
        loggedIn = false;
        failedAttempts = 0;
    }
}

bool Authentication::login(string username, string password) {
    if ((this->username == username) && (hashPassword(password) == passwordHash)) {
        loggedIn = true;
        return true;
    } else {
        return false;
    }
}

void Authentication::logout() {
    if (loggedIn == true) {
        loggedIn = false;
    }
}