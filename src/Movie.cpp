#include "Movie.h"
#include <iostream>
#include <thread>
#include <chrono>

    /*~Movie(){}

        void setDirector(string director);
        const string& getDirector() const;

        void setMainActors(string mainActors);
        const string& getMainActors() const;*/
    
    Movie :: Movie(
        string name, string description,
        string director, string mainActors,
        string ownersConsiderations, string owner,
        string genre, string idiom, int rentValue,
        string recommendedAge, int id
    ){  
        
        setName(name);
        setDescription(description);
        setDirector(director); // not definided 
        setMainActors(mainActors); // not definided 
        setOwnersConsiderations(ownersConsiderations);
        setOwner(owner);
        setGenre(genre);
        setIdiom(idiom);
        setRentValue(rentValue);
        setRecommendedAge(recommendedAge);
        setId(id);

        cout << "Your book has been added to the system. Now other users can access it." << endl; 
    }

    Movie :: ~Movie(){
        
    }