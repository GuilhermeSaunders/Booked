#include <string>

using namespace std;

class Authentication {

    private:

    string username; 
    size_t passwordHash; // Stores the hashed representation of the user's password for secure authentication
    string email; 
    string cpf;
    string birthday; 
    
    size_t hashPassword(const string& password); 

    public:



    bool createUser(string& username, string& password, string& email, string& cpf, string& birthday);
 
    // getters

    string getUsername() const { return username; };
    string getEmail() const { return email; };
    string getCpf() const { return cpf; };
    string getBirthday() const { return birthday; };

};