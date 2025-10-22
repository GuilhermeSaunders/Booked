#include <iostream>
#include "../src/Customer/account.h"
#include "../src/Customer/user.h"
#include "../src/BusinessRules/authentication.cpp"

using namespace std;

int main(){

    bool running = true;

    string input_cmmd;
    string input_username;
    string input_password;
    string input_email;
    string input_cpf;
    string input_birthday;

    bool is_auth = false;

    while(running){
        cout<<"Digite 'Login' ou 'Criar'.\n";
        cin>>input_cmmd;

        if (input_cmmd == "Criar"){
            cout<<"Digite seu Username\n";
            cin>>input_username;
            cout<<"Digite seu Email\n";
            cin>>input_email;
            cout<<"Digite sua senha\n";
            cin>>input_password;
            cout<<"Digite seu CPF\n";
            cin>>input_cpf;
            cout<<"Digite seu Aniversário no formato dd/mm/yyyy\n";
            cin>>input_birthday;

            Authentication authentication;
            is_auth = authentication.createUser(input_username, input_email, 
            input_password, input_cpf, input_birthday);
            cout << is_auth;
            
;
        };
    };
    return 0;
};