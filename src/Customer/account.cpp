#include "account.h"
#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Initializes account data using an initializer list.
 */
Account::Account(
    const string& i_id,
    const size_t& i_hash,
    const string& i_username,
    const User& i_user)
    : id(i_id),
      hash(i_hash),
      username(i_username),
      user(i_user) {
}

/**
 * @brief Changes the account password after verifying the current one.
 */
bool Account::change_password(const size_t& current_hash, const size_t& new_hash) {
    if (hash != current_hash) {
        return false; // Current password incorrect
    }
    hash = new_hash; // Update to new password hash
    return true;     // Password changed successfully
}

/**
 * @brief Changes the account username.
 */
bool Account::change_username(const string& new_username) {
    username = new_username; // Update username
    return true;
}

/**
 * @brief Gets the stored password hash.
 */
const size_t& Account::getHash() const {
    return hash;
}

/**
 * @brief Gets the account's unique ID.
 */
const string& Account::getId() const {
    return id;
}

/**
 * @brief Gets the account's username.
 */
const string& Account::getUsername() const {
    return username;
}

/**
 * @brief Gets a const reference to the associated User object.
 */
const User& Account::getUser() const {
    return user;
}

/**
 * @brief Gets a mutable reference to the associated User object.
 */
User& Account::getUser() {
    // This allows App.cpp to call:
    // currentUser->getUser().update_fullname(...)
    return user;
}