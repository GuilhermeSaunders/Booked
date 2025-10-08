#include <string>
#include "books.h"

using namespace std;


    //methods for each attribute
    void Books :: setGenre(string genre) { this -> genre = genre; }
    string Books :: getGenre() {return genre;}

    void Books :: setTitle(string title){this -> title = title;}
    string Books :: getTitle(){return title;}

    void Books :: setAuthor(string author){this -> author = author;}
    string Books :: getAuthor() {return author;}

    void Books :: setDescription(string description){this -> description = description;}
    string Books :: getDescription() {return description;}

    void Books :: setOwner(string owner){this -> owner = owner;}
    string Books :: getOwner(){return owner;}

    void Books :: setValue(float value){this -> value = value;}
    float Books :: getValue(){return value;}

    void Books :: setId(int id){this -> id = id;}
    int Books :: getId(){return id;}

    void Books :: setPages(int pages){this -> pages = pages;}
    int Books :: getPages(){return pages;}

    void Books :: setStatus(bool isRented){this -> isRented = isRented;}
    bool Books :: getStatus(){return isRented;}


