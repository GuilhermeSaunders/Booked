#include "Book.h"
#include <iostream>
#include <thread>
#include <chrono>

    Book :: Book(
        string name, string description, string author,
        string ownersConsiderations, string owner,
        string genre, string idiom, int rentValue,
        string recommendedAge, int id
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
        setProductType("Book");

        
        this_thread::sleep_for(chrono::seconds(2));
        cout << endl;
        cout << "Your book has been added to the system. Now other users can access it." << endl;
        cout << endl;
    }

    Book :: ~Book(){

        cout << endl;
        cout << "Deleting book..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Book removed from the system." << endl;
        cout << endl;
    }

    void Book::setAuthor(string author){this->author = move(author);}
    const string& Book::getAuthor() const {return author;}

    void Book::showInfor() const {
        cout << endl;
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