#ifndef BOOK_H
#define BOOK_H

#include "product.h"


class Book : public virtual  product
{
private:
    int numPages;
    string author;

protected:
        void setAuthor(string& author){this->author = author;}
        const string& getAuthor() const {return author;}

public:
    Book(
        string& name, string& description, string& author,
        string& ownersConsiderations, string& owner,
        string& genre, string& idiom, int rentValue,
        int recommendedAge, int id
    );















    void showInfor() const override;
    void dealTerms() const override;


};

#endif