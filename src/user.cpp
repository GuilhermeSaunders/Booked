#include "user.h"
using namespace std;

User::User(const string& i_email,
           const string& i_cpf,
           const string& i_fullname)
    : cpf(i_cpf),
      fullname(i_fullname),
      email(i_email) {}

// Update CPF or full name based on the 'update' code (UPDATE)
int User::update_info(const string& new_cpf, const string& new_fullname, int update){
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