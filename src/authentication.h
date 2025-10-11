#include <string>

using namespace std;

class Authentication {

    private:

    string username; // Username
    size_t passwordHash; // The validated password
    string email; // Users's e-mail
    bool loggedIn; // True if the user's credentials are correct / if user is logged in
    int failedAttempts; // The amount of failed login attempts
    const int maxFailedAttempts = 3; // Maximum number of allowed failed login attempts

    size_t hashPassword(const string& password); // Function that creates the password's hash
 

    public:

    Authentication();

    void createUser(string username, string password, string email);

    bool login(string username, string password);

    void logout();

    bool isLoggedIn();

    // getters

    string getUsername() const { return username; };
    string getEmail() const { return email; };
    bool getStatus() const { return loggedIn; };    

};