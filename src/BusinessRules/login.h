#include <string>
#include "account.h"

using namespace std;

class Login {

    private: 

    bool loggedIn;          // True if a user is currently logged in
    int failedAttempts;     // Counts failed login attempts
    const int maxFailedAttempts = 3; // Maximum allowed failed attempts

    size_t hashPassword(const string& password);

    public:

    Login();

    bool login(const Account& account, const string& inputUsername, const string& inputPassword);

    void logout();

    bool isLoggedIn();
};