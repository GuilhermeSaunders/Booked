#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
#include <string>
using namespace std;

class Account{
    private:

    string id;
    string hash;
    string email;
    string cpf;
    string username;
    string fullname;
    string status;

    public:
    //construtor - CREATE
    bool init(const string& i_id, const string& i_hash, const string& i_email, const string& i_cpf, const string& i_username, const string& i_fullname);

    //getters - READ
    string getId() const { return id; };
    string getCpf() const { return cpf; };
    string getUsername() const { return username; };
    string getFullname() const { return fullname; };
    string getEmail() const { return email; };
    string getHash() const { return hash;}
    string getStatus() const { return status;}

    //UPDATE
    bool change_password(const string& current_hash, const string& new_hash);
    bool change_username(const string& new_username);
    int update_info(const string& new_cpf, const string& new_fullname, int update);

    //DELETE
    bool delete_account(const string& current_username, const string& current_hash);

    //Buyer or Seller
    void setStatus(string status);

};
#endif