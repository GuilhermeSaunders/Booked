#include "login.h"
#include "account.h"
#include <string>
#include <iostream>
#include <functional> // For std::hash

using namespace std;

/**
 * @brief Default constructor, required by App.
 * Initializes the session as logged out.
 */
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

    // Use the local hashPassword method
    bool userMatch = (inputUsername == account.getUsername());
    bool passMatch = (hashPassword(inputPassword) == account.getHash()); // <-- MUDADO DE VOLTA

    if (userMatch && passMatch) {
        loggedIn = true;
        failedAttempts = 0;
        return true;
    } else {
        loggedIn = false;
        failedAttempts++;
        return false;
    }
}

void Login::logout() {
    loggedIn = false;
    failedAttempts = 0;
}

bool Login::isLoggedIn() {
    return loggedIn; // Simplified logic
}

/**
 * @brief Hashes a password using std::hash.
 */
size_t Login::hashPassword(const string& password) {
    hash<string> hasher;
    return hasher(password);
}