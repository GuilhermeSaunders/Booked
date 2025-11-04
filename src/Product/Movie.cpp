#include "movie.h"
#include <utility> // For std::move

/**
 * @brief Constructor for Movie.
 */
Movie::Movie(float rentValue,
             std::string owner,
             std::string name,
             std::string description,
             std::string idiom,
             std::string recommendedAge,
             std::string genre,
             std::string director,
             std::string mainActors,
             int duration)

    : Product(rentValue,
              std::move(owner),
              std::move(name),
              std::move(description),
              std::move(idiom),
              std::move(recommendedAge),
              std::move(genre)),
      director(std::move(director)),
      mainActors(std::move(mainActors)),
      duration(duration) {
}

// --- Getter Implementations ---

const std::string& Movie::getDirector() const {
    return this->director;
}

const std::string& Movie::getMainActors() const {
    return this->mainActors;
}

int Movie::getDuration() const {
    return this->duration;
}
