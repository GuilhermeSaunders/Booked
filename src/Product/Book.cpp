#include "book.h"

    Book :: Book(float rentValue,
         std::string id,
         std::string owner,
         std::string name,
         std::string description,  std::string idiom,
         std::string recommendedAge,  std::string genre, bool isRented, int numPages, std::string author)

    :   Product( rentValue, std::move(id),
        std::move(owner),
        std::move(name),
        std::move(description), std::move(idiom),
        std::move(recommendedAge), std::move(genre),  isRented),
        numPages(numPages),
        author(std::move(author)){}


    Book :: ~Book() = default;

    void Book::setAuthor(std::string author){this->author = std::move(author);}
    const std::string& Book::getAuthor() const {return author;}

    void Book::setNumPages(int numPages){this->numPages = numPages;}
    int Book::getNumPages() const {return numPages;}