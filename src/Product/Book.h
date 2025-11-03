#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "Product.h"

class Book : public Product
{

private:
    int numPages;
    std::string author;

public:

    Book(float rentValue,
         std::string owner,
         std::string name,
         std::string description,  std::string idiom,
         std::string recommendedAge,  std::string genre, 
         int numPages, std::string author);


    ~Book() override = default;


    void setAuthor(std::string author);
    const std::string& getAuthor() const;

    void setNumPages(int numPages);
    int getNumPages() const;

    std::string getType() const override {return "BOOK";}
    
};

#endif