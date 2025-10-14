#ifndef MOVIE_H
#define MOVIE_H

#include "Product.h"

class Movie : public virtual  Product{
    private:
        string director, mainActors;

    public:
        Movie(
        string name, string description,
        string director, string mainActors,
        string ownersConsiderations, string owner,
        string genre, string idiom, int rentValue,
        string recommendedAge, int id
        ){}

        ~Movie(){}

        void setDirector(string director);
        const string& getDirector() const;

        void setMainActors(string mainActors);
        const string& getMainActors() const;


};

#endif