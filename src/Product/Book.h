#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "product.h"

/**
 * @class Book
 * @brief Represents a Book product, inheriting from Product.
 *
 * Holds book-specific data like author and page count.
 * This object is immutable after construction.
 */
class Book : public Product {
private:
    int numPages;
    std::string author;

public:
    /**
     * @brief Constructor for Book.
     */
    Book(float rentValue,
         std::string owner,
         std::string name,
         std::string description,
         std::string idiom,
         std::string recommendedAge,
         std::string genre,
         int numPages,
         std::string author);

    ~Book() override = default;

    // --- Getters (Used by Repository to save data) ---
    const std::string& getAuthor() const;
    int getNumPages() const;

    /**
     * @brief Gets the type as a string.
     * @return "BOOK"
     */
    std::string getType() const override {
        return "BOOK";
    };
};

#endif