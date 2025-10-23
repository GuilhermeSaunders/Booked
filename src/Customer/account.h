#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include "user.h"
using namespace std;

class Account{
    private:
    string id;         // Unique account identifier
    size_t hash;       // Password hash for authentication
    string username;   // Account username    
    User user;

    public:
    Account(const string& i_id,
                 const size_t& i_hash,
                 const string& i_username,
                 const User& i_user);

    const size_t& getHash() const;
    const string& getId() const;
    const string& getUsername() const;
    const User& getUser() const;


    // Update methods - UPDATE
    bool change_password(const size_t& current_hash, const size_t& new_hash); // Change password
    bool change_username(const string& new_username); // Change username

    // Delete method - DELETE
    bool delete_account(const string& current_username, const size_t& current_hash); // Delete account if credentials match

};
#endif