#ifndef USER_H
#define USER_H

#include <string>

/**
 * @class User
 * @brief Represents the personal profile data for a customer.
 */
class User {
private:
    int id;
    std::string cpf;
    std::string fullname;
    std::string email;
    std::string birthday;

public:
    User(const std::string& i_email,
         const std::string& i_cpf,
         const std::string& i_fullname,
         const std::string& i_birthday);

    // --- Getters ---
    std::string getCpf() const;
    std::string getFullname() const;
    std::string getEmail() const;
    std::string getBirthday() const;

    int getId() const;

    // --- Setters ---
    void setId(int ind);
    void update_fullname(const std::string& new_fullname);
};

#endif