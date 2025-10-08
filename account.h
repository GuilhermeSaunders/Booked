#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
#include <string>
using namespace std;

class Account {
private:
    string id;         // Unique account identifier
    string hash;       // Password hash for authentication
    string email;      // Account email
    string cpf;        // User CPF
    string username;   // Account username
    string fullname;   // Full name of the user
    string status;     // Account status: "renting" or "lending"

public:
    // Constructor - CREATE
    bool init(const string& i_id, const string& i_hash, const string& i_email, 
              const string& i_cpf, const string& i_username, const string& i_fullname, const string& i_status);
    
    // Getters - READ
    string getId() const { return id; };
    string getCpf() const { return cpf; };
    string getUsername() const { return username; };
    string getFullname() const { return fullname; };
    string getEmail() const { return email; };
    string getHash() const { return hash; };
    string getStatus() const { return status; };  // Returns if user is renting or lending

    // Update methods - UPDATE
    bool change_password(const string& current_hash, const string& new_hash); // Change password
    bool change_username(const string& new_username); // Change username
    int update_info(const string& new_cpf, const string& new_fullname, int update); // Update CPF or full name

    // Delete method - DELETE
    bool delete_account(const string& current_username, const string& current_hash); // Delete account if credentials match

    // Set account status: renting or lending
    void setStatus(string status); 
};
#endif