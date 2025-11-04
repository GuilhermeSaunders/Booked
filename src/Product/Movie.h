#ifndef MOVIE_H
#define MOVIE_H

#include "product.h"
#include <string>

/**
 * @class Movie
 * @brief Represents a Movie product, inheriting from Product.
 *
 * Holds movie-specific data like director and duration.
 * This object is immutable after construction.
 */
class Movie : public Product {

private:
    std::string director, mainActors;
    int duration; // in minutes

public:
    /**
     * @brief Constructor for Movie.
     */
    Movie(float rentValue,
          std::string owner,
          std::string name,
          std::string description,
          std::string idiom,
          std::string recommendedAge,
          std::string genre,
          std::string director,
          std::string mainActors,
          int duration);

    ~Movie() override = default;

    // --- Getters (Used by Repository to save data) ---
    const std::string& getDirector() const;
    const std::string& getMainActors() const;
    int getDuration() const;

    /**
     * @brief Gets the type as a string.
     * @return "MOVIE"
     */
    std::string getType() const override {
        return "MOVIE";
    }
};

#endif