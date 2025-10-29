#ifndef VIDEO_GAME_H
#define VIDEO_GAME_H

#include "product.h"
#include <string>

class Video_Game : public Product{

    private:
        std::string  style, plataform; // if it is a battle royale, a fps game and etc
        int duration, num_players;

    public:
         Video_Game(float rentValue,
         std::string id,
         std::string owner,
         std::string name,
         std::string description,  std::string idiom,
         std::string recommendedAge,  std::string genre, bool isRented, std::string style, std::string plataform, int duration);

        ~Video_Game() override = default;

        void setStyle(std::string style);
        const std::string& getStyle() const;

        void setPlataform(std::string plataform);
        const std::string& getPlataform() const;

        void setNumPlayers(int num_players);
        int getNumPlayers() const;

        void setDuration(int duration);
        int getDuration() const;
};

#endif