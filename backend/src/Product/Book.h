#ifndef BOOK_H
#define BOOK_H

#include "Product.h"


class Book : public virtual  Product
{
private:
    int numPages;
    string author;

public:
    Book(
        string name, string description, string author,
        string ownersConsiderations, string owner,
        string genre, string idiom, int rentValue,
        string recommendedAge, int id
    );

    ~Book(){}

    void setAuthor(string author){this->author = author;}
    const string& getAuthor() const {return author;}


    // conversation about the dealTerms function is important
    void showInfor() const override;
    void dealTerms() const override;


};

#endif