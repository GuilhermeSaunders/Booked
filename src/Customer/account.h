#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include "user.h"
using namespace std;

class Account{
    private:
    string id;         // Unique account identifier
    string hash;       // Password hash for authentication
    string username;   // Account username    
    User user;

    public:
    Account(const string& i_id,
                 const size_t& i_hash,
                 const string& i_username,
                 const User& i_user);

    const string& getHash() const { return hash; };
    const string& getId() const { return id; };
    const string& getUsername() const { return username; };
    const User& getUser() const { return user; };


    // Update methods - UPDATE
    bool change_password(const size_t& current_hash, const size_t& new_hash); // Change password
    bool change_username(const string& new_username); // Change username

    // Delete method - DELETE
    bool delete_account(const string& current_username, const size_t& current_hash); // Delete account if credentials match

};
#endif