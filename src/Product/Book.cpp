#include "Book.h"
#include <utility> // Para std::move

// SEM ESPAÇO EXTRA AQUI
Book::Book(float rentValue,
     std::string owner,
     std::string name,
     std::string description,  std::string idiom,
     std::string recommendedAge,  std::string genre, int numPages, std::string author)
    : Product(rentValue,
            std::move(owner),
            std::move(name),
            std::move(description), 
            std::move(idiom),
            std::move(recommendedAge), 
            std::move(genre)),
            numPages(numPages),
            author(std::move(author))
{} // Construtor

// O DESTRUTOR FOI REMOVIDO DAQUI (JÁ ESTÁ NO .h)

void Book::setAuthor(std::string author){
    this->author = std::move(author);
}
const std::string& Book::getAuthor() const {
    return author;
}

void Book::setNumPages(int numPages){
    this->numPages = numPages;
}
int Book::getNumPages() const {
    return numPages;
}