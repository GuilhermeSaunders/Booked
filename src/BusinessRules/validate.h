#ifndef VALIDATE_H
#define VALIDATE_H

#include <string>
#include "user.h"    // Required for createUser
#include "account.h" // Required for createAccount

/**
 * @class Validate
 * @brief A utility service class for validating user input and creating objects.
 *
 * This class is stateless and provides methods for validation (e.g., password, email)
 * and factory functions for creating User and Account objects.
 */
class Validate {

private:
    /**
     * @brief Generates a random numeric string ID.
     * @param length The desired length of the ID (default is 6).
     * @return A string representing the random numeric ID.
     */
    std::string generateId(const int length = 6);

public:
    /**
     * @brief Hashes a given password string.
     * @param password The plain-text password.
     * @return A size_t hash of the password.
     */
    size_t hashPassword(const std::string& password);

    /**
     * @brief Validates a username (3-15 characters).
     * @param username The username to check.
     * @return true if valid, false otherwise.
     */
    bool validateUsername(const std::string& username);

    /**
     * @brief Validates a password (min 6 chars, 1 upper, 1 lower, 1 digit, 1 special).
     * @param password The password to check.
     * @return true if strong enough, false otherwise.
     */
    bool validateStringPassword(const std::string& password);

    /**
     * @brief Validates an email (must contain '@' and ".com").
     * @param email The email to check.
     * @return true if valid, false otherwise.
     */
    bool validateEmail(const std::string& email);

    /**
     * @brief Validates a CPF (must be exactly 11 digits).
     * @param cpf The CPF to check.
     * @return true if valid, false otherwise.
     */
    bool validateCpf(const std::string& cpf);

    /**
     * @brief Validates a full name (min 5 chars, no leading/trailing spaces, no digits).
     * @param fullname The name to check.
     * @return true if valid, false otherwise.
     */
    bool validateFullname(const std::string& fullname);

    /**
     * @brief Factory method to create a User object.
     * @param email User's email.
     * @param cpf User's CPF.
     * @param fullname User's full name.
     * @param birthday User's birthday.
     * @return A User object.
     */
    User createUser(const std::string& email, const std::string& cpf, const std::string& fullname, const std::string& birthday);

    /**
     * @brief Factory method to create an Account object.
     * This method handles hashing the password and generating a new ID.
     * @param stringPassword The user's plain-text password.
     * @param username The user's chosen username.
     * @param user The associated User object.
     * @return An Account object.
     */
    Account createAccount(const std::string& stringPassword, const std::string& username, const User& user);
};

#endif