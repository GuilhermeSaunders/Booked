#include <string>

using namespace std;

class Validate {

    private:

    string username; 
    size_t passwordHash; // Stores the hashed representation of the user's password for secure authentication
    string email; 
    string cpf;
    string birthday;
    string fullname;
    
    size_t hashPassword(const string& password);
    string generateId(const int lenght = 6);

    public:

    bool validateUsername(const string& username);
    bool validateStringPassword(const string& password);
    bool validateEmail(const string& email);
    bool validateCpf(const string& cpf);
    bool validateBirthday(const string& birthday);
    bool validateFullname(const string& fullname);

    User createUser(const string& email, const string& cpf, const string& fullname, const string& birthday);
    Account createAccount(const string& stringPassword, const string& username, const User& user);
 
    // getters

    string getUsername() const { return username; };
    string getEmail() const { return email; };
    string getCpf() const { return cpf; };
    string getBirthday() const { return birthday; };

};