#include "movie.h"

        Movie :: Movie(float rentValue,
                std::string id,
                std::string owner,
                std::string name,
                std::string description,  std::string idiom,
                std::string recommendedAge,  std::string genre, bool isRented, std::string director, std::string mainActors, int duration)

        : Product( rentValue, std::move(id),
        std::move(owner),
        std::move(name),
        std::move(description), std::move(idiom),
        std::move(recommendedAge), std::move(genre),  isRented),
        director(std::move(director)),
        mainActors(std::move(mainActors)),
        duration(duration){}

        Movie::~Movie() = default;

        void Movie :: setDirector(std::string director) {this->director = std::move(director);}
        const std::string& Movie :: getDirector() const {return director;}

        void Movie :: setMainActors(std::string mainActors) {this->mainActors = std::move(mainActors);}
        const std::string& Movie :: getMainActors() const {return mainActors;}



        void Movie :: setDuration(int duration) {this->duration = duration;}

        int Movie :: getDuration() const {return duration;}