#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <functional> // For std::hash
#include "account.h"

/**
 * @class Login
 * @brief Manages the user's authentication state (session).
 */
class Login {

private:
    bool loggedIn;
    int failedAttempts;
    const int maxFailedAttempts = 3;

    /**
     * @brief Hashes a password.
     * @details This is duplicated from Validate to avoid changing app.cpp.
     * @param password The plain-text password.
     * @return A size_t hash.
     */
    size_t hashPassword(const std::string& password);

public:
    /**
     * @brief Default constructor (required by App).
     */
    Login();

    bool login(const Account& account, const std::string& inputUsername, const std::string& inputPassword);
    void logout();
    bool isLoggedIn();
};

#endif