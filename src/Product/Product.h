#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

/**
 * @class Product
 * @brief The abstract base class for all rentable items.
 *
 * This class holds the common data shared by all products,
 * such as name, description, owner, and rent value.
 * It is designed to be immutable after construction, except for the ID,
 * which is set by the Repository.
 */
class Product {

private:
    float rentValue;
    std::string owner; // (owner_cpf)
    std::string genre;
    std::string name;
    std::string description;
    std::string idiom;
    std::string recommendedAge;
    int id; // Database ID

    // The 'STATUS' in the ITEM table
    // in the database is the single source of truth.

public:
    /**
     * @brief Constructor for the base Product class.
     */
    Product(float rentValue,
            std::string owner,
            std::string name,
            std::string description,
            std::string idiom,
            std::string recommendedAge,
            std::string genre);

    /**
     * @brief Virtual destructor, defaulted.
     */
    virtual ~Product() = default;

    // --- Getters (Used by Repository to read data for saving) ---
    float getRentValue() const;
    const std::string& getOwner() const;
    const std::string& getGenre() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::string& getIdiom() const;
    const std::string& getRecommendedAge() const;
    int getId() const;

    /**
     * @brief Sets the database ID.
     * @details This is the ONLY setter. It is called by the Repository
     * after the item is inserted into the database to update the
     * object in memory with its new ID.
     * @param id The new ID from the database.
     */
    void setId(int id);

    /**
     * @brief Pure virtual method to get the product's type.
     * @return A string (e.g., "BOOK", "MOVIE").
     */
    virtual std::string getType() const = 0;


};

#endif