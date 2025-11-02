#include <iostream>
#include <string>
#include "login.h"
#include "account.h"
#include "app.h"
using namespace std;

string App::accountPage() {
    string opcao;

    cout << "===== Tela de Login =====" << endl;
    cout << "Selecione uma opcão" << endl;
    cout << "1. Cadastro" << endl;
    cout << "2. Login" << endl;
    cout << "3. Sair" << endl;

    cin >> opcao;

    return opcao;
}

void App::loginPage(Login& loginSession, const Account& userAccount) {
    string username, password;

    cout << "===== Login =====" << endl;
    cout << "Digite seu nome de usuário: ";
    getline(cin, username);

    cout << "Digite sua senha: ";
    getline(cin, password);

    if (loginSession.login(userAccount, username, password)) {
        cout << "Login realizado com sucesso!" << endl;
    } else {
        cout << "Usuário ou senha incorretos. Tente novamente." << endl;
    }

    cout << "======================" << endl;
}
