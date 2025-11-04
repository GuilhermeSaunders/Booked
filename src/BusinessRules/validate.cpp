#include "validate.h"
#include "user.h"
#include "account.h"
#include <functional> // For std::hash
#include <string>
#include <cctype>   // For isupper, islower, isdigit, ispunct
#include <random>   // For std::mt19937, std::random_device

// Using namespace std is generally acceptable in .cpp files
using namespace std;

/**
 * @brief Hashes a password using std::hash.
 */
size_t Validate::hashPassword(const string& password) {
    hash<string> hasher;
    return hasher(password);
}

/**
 * @brief Generates a random numeric ID string.
 */
string Validate::generateId(int length) {
    string id;
    // Create a random number generator
    mt19937 rng(std::random_device{}());
    // Create a distribution from 0 to 9
    uniform_int_distribution<int> dist(0, 9);

    for (int i = 0; i < length; ++i) {
        id += std::to_string(dist(rng));
    }

    return id;
}

/**
 * @brief Validates username length (3 to 15 chars).
 */
bool Validate::validateUsername(const string& username) {
    // Simplified logic: return the result of the boolean expression directly.
    return (username.length() >= 3 && username.length() <= 15);
}

/**
 * @brief Validates password complexity.
 */
bool Validate::validateStringPassword(const string& password) {
    // Check minimum length
    if (password.length() < 6) {
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    // Iterate over the password to check for required character types
    for (char ch : password) {
        if (isupper(ch)) {
            hasUpper = true;
        } else if (islower(ch)) { // Use 'else if' for minor optimization
            hasLower = true;
        } else if (isdigit(ch)) {
            hasDigit = true;
        } else if (ispunct(ch)) { // ispunct checks for special characters
            hasSpecial = true;
        }
    }

    // Return true only if all conditions are met
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

/**
 * @brief Validates email format (simple check for '@' and '.com').
 */
bool Validate::validateEmail(const string& email) {
    // Note: This is a very basic validation.
    // Original logic had a bug: (A && B) == false is not (!A && !B)
    // Corrected logic: must find BOTH '@' and ".com".
    bool foundAt = (email.find('@') != string::npos);
    bool foundCom = (email.find(".com") != string::npos);

    return foundAt && foundCom;
}

/**
 * @brief Validates CPF length (must be exactly 11 chars).
 */
bool Validate::validateCpf(const string& cpf) {
    // Note: This does not validate the CPF algorithm, only the length.
    return (cpf.length() == 11);
}

/**
 * @brief Validates full name format.
 */
bool Validate::validateFullname(const string& fullname) {
    // Check minimum length
    if (fullname.length() < 5) {
        return false;
    }
    // Check for leading or trailing spaces
    if (fullname.front() == ' ' || fullname.back() == ' ') {
        return false;
    }

    for (char ch : fullname) {
        // Check for digits
        if (std::isdigit(ch)) {
            return false;
        }
        // Check for punctuation (allowing only '-' and '\'')
        if (std::ispunct(ch)) {
            if (ch != '-' && ch != '\'') {
                return false;
            }
        }
    }

    return true; // Passed all checks
}

/**
 * @brief Factory for User objects.
 */
User Validate::createUser(const string& email, const string& cpf, const string& fullname, const string& birthday) {
    // Simply forwards arguments to the User constructor
    return User(email, cpf, fullname, birthday);
}

/**
 * @brief Factory for Account objects.
 */
Account Validate::createAccount(const string& stringPassword, const string& username, const User& user) {
    // Business logic: hash the password and generate an ID
    size_t hash = hashPassword(stringPassword);
    string id = generateId(); // Uses the private helper method

    return Account(id, hash, username, user);
}