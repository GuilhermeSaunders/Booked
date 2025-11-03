#ifndef USER_H
#define USER_H
#include <string> // ADICIONE ISSO

// using namespace std; // DELETE ISSO

class User {
private:
    int id;
    std::string cpf;        // ADICIONE std::
    std::string fullname;   // ADICIONE std::
    std::string email;      // ADICIONE std::
    std::string birthday;   // ADICIONE std::

public:
    // Construtor
    User(const std::string& i_email,   // ADICIONE std::
        const std::string& i_cpf,      // ADICIONE std::
        const std::string& i_fullname, // ADICIONE std::
        const std::string& i_birthday);// ADICIONE std::
    
    // Getters
    std::string getCpf() const;        // ADICIONE std::
    std::string getFullname() const;   // ADICIONE std::
    std::string getEmail() const;      // ADICIONE std::
    std::string getBirthday() const;   // ADICIONE std::
    int getId() const;
    void setId(int ind);

    // Setters
    void update_cpf(const std::string& new_cpf); // ADICIONE std::
    void update_fullname(const std::string& new_fullname); // ADICIONE std::
    void update_birthday(const std::string& new_birthday); // ADICIONE std::
};

#endif