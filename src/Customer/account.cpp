#include "account.h"
#include <iostream>
using namespace std;

// Initialize account data (CREATE
Account::Account(
    const string& i_id,
    const string& i_hash,
    const string& i_username,
    const User& i_user)
    : id(i_id),
      hash(i_hash),
      username(i_username),
      user(i_user) {}
    
//Change account password (UPDATE)
bool Account::change_password(const string& current_hash, const string& new_hash) {
    if (hash != current_hash) {
        return false; // Current password incorrect
    }
    hash = new_hash; // Update to new password hash
    return true;     // Password changed successfully
}

// Change account username (UPDATE)
bool Account::change_username(const string& new_username){
    username = new_username; // Update username
    return true;
}

// Delete account if username and password hash match (DELETE)
bool Account::delete_account(const string& current_username, const string& current_hash) {
    if (username == current_username && hash == current_hash) {
        id.clear();
        username.clear();
        hash.clear();
        user = User("", "", ""); // recria o usuário “vazio”
        return true;
    }
    return false;
}

