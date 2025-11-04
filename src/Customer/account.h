#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string> // ADICIONE ISSO
#include "user.h"

// using namespace std; // DELETE ISSO

class Account{
private:
    std::string id;         // ADICIONE std::
    size_t hash;       
    std::string username;   // ADICIONE std::
    User user;

public:
    Account(const std::string& i_id,    // ADICIONE std::
                 const size_t& i_hash,
                 const std::string& i_username, // ADICIONE std::
                 const User& i_user);

    const size_t& getHash() const;
    const std::string& getId() const;       // ADICIONE std::
    const std::string& getUsername() const; // ADICIONE std::
    const User& getUser() const;
    User& getUser();

    // Métodos
    bool change_password(const size_t& current_hash, const size_t& new_hash);
    bool change_username(const std::string& new_username); // ADICIONE std::
    bool delete_account(const std::string& current_username, const size_t& current_hash); // ADICIONE std::
};
#endif