#include "book.h"
#include <utility> // For std::move

/**
 * @brief Constructor for Book.
 */
Book::Book(float rentValue,
           std::string owner,
           std::string name,
           std::string description,
           std::string idiom,
           std::string recommendedAge,
           std::string genre,
           int numPages,
           std::string author)
    : Product(rentValue,
              std::move(owner),
              std::move(name),
              std::move(description),
              std::move(idiom),
              std::move(recommendedAge),
              std::move(genre)),
      numPages(numPages),
      author(std::move(author)) {
} // Constructor

// --- Getter Implementations ---

const std::string& Book::getAuthor() const {
    return author;
}

int Book::getNumPages() const {
    return numPages;
}
