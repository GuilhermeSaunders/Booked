#include <iostream>
#include "login.h"
using namespace std;

Login::Login() {
    loggedIn = false;
    failedAttempts = 0;
}

bool Login::login(const Account& account, const string& inputUsername, const string& inputPassword) {
    if (loggedIn) {

        return true;
    }

    if (failedAttempts >= maxFailedAttempts) {

        return false;
    }

    if ((inputUsername == account.getUsername()) && (hashPassword(inputPassword) == account.getHash())) {

        loggedIn = true;
        return true;
    } else {
        loggedIn = false;
        failedAttempts++;
        return false;
    }

}

void Login::logout() {

    if (loggedIn == true) {

        loggedIn = false;
    }
}

bool Login::isLoggedIn() {

    if (loggedIn = true) {

        return true;
    } else {

        return false;
    }
}

size_t Login::hashPassword(const string& password) {
    hash<string> hasher;
    return hasher(password); 
}