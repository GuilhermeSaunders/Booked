#include "product.h"
#include <utility> // For std::move

/**
 * @brief Constructor for the base Product class.
 * @details Initializes the object with all required data.
 * The ID is set to -1 by default until persisted.
 */
Product::Product(float rentValue,
                 std::string owner,
                 std::string name,
                 std::string description,
                 std::string idiom,
                 std::string recommendedAge,
                 std::string genre)
    : rentValue(rentValue),
      owner(std::move(owner)),
      genre(std::move(genre)),
      name(std::move(name)),
      description(std::move(description)),
      idiom(std::move(idiom)),
      recommendedAge(std::move(recommendedAge)),
      id(-1) // Default ID until set by database
{
}

/**
 * @brief Sets the database ID of the product.
 */
void Product::setId(int id) {
    this->id = id;
}

/**
 * @brief Gets the database ID of the product.
 */
int Product::getId() const {
    return this->id;
}

// --- Getter Implementations ---

float Product::getRentValue() const {
    return rentValue;
}
const std::string& Product::getOwner() const {
    return owner;
}
const std::string& Product::getGenre() const {
    return genre;
}
const std::string& Product::getName() const {
    return name;
}
const std::string& Product::getDescription() const {
    return description;
}
const std::string& Product::getIdiom() const {
    return idiom;
}
const std::string& Product::getRecommendedAge() const {
    return recommendedAge;
}