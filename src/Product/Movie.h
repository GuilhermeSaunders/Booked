#ifndef MOVIE_H
#define MOVIE_H

#include "product.h"
#include <string>

class Movie : public Product{

    private:
        std::string director, mainActors;
        int duration;

    public:
         Movie(float rentValue,
         std::string id,
         std::string owner,
         std::string name,
         std::string description,  std::string idiom,
         std::string recommendedAge,  std::string genre, bool isRented, std::string director, std::string mainActors, int duration);

        ~Movie() override = default;

        void setDirector(std::string director);
        const std::string& getDirector() const;

        void setMainActors(std::string mainActors);
        const std::string& getMainActors() const;

        void setDuration(int duration);
        int getDuration() const;
};

#endif