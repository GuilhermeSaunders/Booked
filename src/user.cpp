#include "user.h"
using namespace std;

User::User(const string& i_id,
           const string& i_hash,
           const string& i_email,
           const string& i_cpf,
           const string& i_username,
           const string& i_fullname,
           const string& i_status): 
  id(i_id),
  hash(i_hash),
  email(i_email),
  cpf(i_cpf),
  username(i_username),
  fullname(i_fullname),
  status(i_status)
{}

// Initialize account data (CREATE)
Account::Account(
    const string& i_id, 
    const string& i_hash, 
    const string& i_email, 
    const string& i_cpf, 
    const string& i_username, 
    const string& i_fullname, 
    const string& i_status): User(i_id, i_hash, i_email, i_cpf, i_username, i_fullname, i_status){}

// Change account password (UPDATE)
bool Account::change_password(const string& current_hash, const string& new_hash) {
    if (hash != current_hash) {
        return false; // Current password incorrect
    }
    hash = new_hash; // Update to new password hash
    return true;     // Password changed successfully
};

// Change account username (UPDATE)
bool Account::change_username(const string& new_username){
    username = new_username; // Update username
    return true;
};

// Update CPF or full name based on the 'update' code (UPDATE)
int Account::update_info(const string& new_cpf, const string& new_fullname, int update){
    if(update == 0){
        return 0; // No update
    } else if(update == 1){
        cpf = new_cpf; // Update CPF
        return 1;
    } else if(update == 2){
        fullname = new_fullname; // Update full name
        return 2;
    } else{
        return update; // Invalid update code, return as it is
    }
}

// Delete account if username and password hash match (DELETE)
bool Account::delete_account(const string& current_username, const string& current_hash) {
    if(username == current_username && hash == current_hash){
        // Clear all account data
        id = cpf = email = username = fullname = hash = "";
        return true; // Account deleted successfully
    }
    return false; // Deletion failed
}
//Set Status as Actions Requires it
void Account::setStatus(string temp_status){
    status = temp_status;
}