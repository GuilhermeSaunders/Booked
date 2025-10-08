#include "account.h"
#include <iostream>
using namespace std;

bool Account::init(const string& i_id, const string& i_hash, const string& i_email, const string& i_cpf, const string& i_username, const string& i_fullname) {
    bool conditional = true;

    hash = i_hash;
    cpf = i_cpf;
    username = i_username;
    email = i_email;
    id = i_id;
    fullname = i_fullname;
    return true;
};

bool Account::change_password(const string& current_hash, const string& new_hash) {
    if (hash != current_hash) {
        return false; // senha atual incorreta
    }
    hash = new_hash;
    return true; // sucesso
};

bool Account::change_username(const string& new_username){
    username = new_username;
    return true;
};

int Account::update_info(const string& new_cpf, const string& new_fullname, int update){

    if(update==0){
        return 0;
    }else if(update==1){
        cpf = new_cpf;
        return 1;
    }else if(update==2){
        fullname = new_fullname;
        return 2;
    }else{
        return update;
        }
    }

bool Account::delete_account(const string& current_username, const string& current_hash) {
    if(username == current_username && hash == current_hash){
        id = cpf = email = username = fullname = hash = "";
        return true;
    }
    return false;
}
