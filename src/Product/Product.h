#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>    

class Product {

private:
    float rentValue;
    std::string owner; // (owner_cpf)
    std::string genre;
    std::string name;
    std::string description;
    std::string idiom;
    std::string recommendedAge;
    int id;
    bool isRented;

public:
    Product(float rentValue,
         std::string owner,
         std::string name,
         std::string description,  std::string idiom,
         std::string recommendedAge,  std::string genre);

    virtual ~Product() = default;

    // Getters e Setters
    void setRentValue(float rentvalue);
    float getRentValue() const;

    void setOwner(std::string owner);
    const std::string& getOwner() const;

    void setGenre(std::string genre);
    const std::string& getGenre() const;

    void setName(std::string name);
    const std::string& getName() const;

    void setDescription(std::string description);
    const std::string& getDescription() const;

    void setIdiom(std::string idiom);
    const std::string& getIdiom() const;

    void setRecommendedAge(std::string recommendedAge);
    const std::string& getRecommendedAge() const;

    void setId(int id);
    int getId() const;

    void setIsRented(bool isRented);
    bool getIsRented() const;

    virtual std::string getType() const = 0;
};

#endif