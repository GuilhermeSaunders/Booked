#include "Movie.h"
#include <utility> // Para std::move

 Movie :: Movie(float rentValue,
                std::string owner,
                std::string name,
                std::string description,  
                std::string idiom,
                std::string recommendedAge,  
                std::string genre, 
                std::string director, 
                std::string mainActors, 
                int duration)

: Product(rentValue, 
        std::move(owner),
        std::move(name),
        std::move(description), 
        std::move(idiom),
        std::move(recommendedAge), 
        std::move(genre)),
        director(std::move(director)),
        mainActors(std::move(mainActors)),
        duration(duration){}

     

        void Movie :: setDirector(std::string director) {this->director = std::move(director);}
        const std::string& Movie :: getDirector() const {return this->director;}

        void Movie :: setMainActors(std::string mainActors) {this->mainActors = std::move(mainActors);}
        const std::string& Movie :: getMainActors() const {return this->mainActors;}

        void Movie :: setDuration(int duration) {this->duration = duration;}
        int Movie :: getDuration() const {return this->duration;}