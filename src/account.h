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
    string status;     // Account status: "renting" or "lending"
    User user;

    public:
    Account(const string& i_id,
                 const string& i_hash,
                 const string& i_username,
                 const string& i_status,
                 const User& i_user);

    const string& getHash() const { return hash; };
    const string& getId() const { return id; };
    const string& getStatus() const { return status; };  // Returns if user is renting or lending
    const string& getUsername() const { return username; };
    const User& getUser() const { return user; };


    // Update methods - UPDATE
    bool change_password(const string& current_hash, const string& new_hash); // Change password
    bool change_username(const string& new_username); // Change username

    // Delete method - DELETE
    bool delete_account(const string& current_username, const string& current_hash); // Delete account if credentials match

    // Set account status: renting or lending
    void setStatus(const string& temp_status); 
};
#endif