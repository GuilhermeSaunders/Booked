#include <string>
#include "login.h"
#include "account.h"

using namespace std;

class App {

    public:

    string accountPage();
    void loginPage(Login& loginSession, const Account& userAccount);
    bool registerPage();
};