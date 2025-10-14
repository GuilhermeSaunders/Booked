#include "Book.h"
#include <iostream>
    Book :: Book(
        string& name, string& description, string& author,
        string& ownersConsiderations, string& owner,
        string& genre, string& idiom, int rentValue,
        int recommendedAge, int id
    ){
        setName(name);
        setDescription(description);
        setAuthor(author);
        setOwnersConsiderations(ownersConsiderations);
        setOwner(owner);
        setGenre(genre);
        setIdiom(idiom);
        setRentValue(rentValue);
        setRecommendedAge(recommendedAge);
        setId(id);
    }

    void Book::setAuthor(string& author){this->author = author;}
    const string& Book::getAuthor() const {return author;}

    void Book::showInfor() const {
        cout << "Book Information: " << endl;
        cout << "Title: " << getName() << endl;
        cout << "Genre: " << getGenre() << endl;
        cout << endl;
        cout << "Description: " << getDescription() << endl;
        cout << endl;
        cout << "Author: " << getAuthor() << endl;
        cout << "Idiom: " << getIdiom() << endl;
        cout << endl;
        cout << "Owner's Considerations: " << getOwnersConsiderations() << endl;
    }