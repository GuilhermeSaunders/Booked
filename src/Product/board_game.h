#ifndef BOARD_GAME_H
#define BOARD_GAME_H

#include "product.h"
#include <string>

class Board_Game : public Product{

    private:
        std::string  style; // if it is a battle royale, a fps game and etc
        int duration, num_players;

    public:
         Board_Game(float rentValue,
         std::string id,
         std::string owner,
         std::string name,
         std::string description,  std::string idiom,
         std::string recommendedAge,  std::string genre, bool isRented, std::string style, int num_players, int duration);

        ~Board_Game() override = default;

        void setStyle(std::string style);
        const std::string& getStyle() const;

        void setNumPlayers(int num_players);
        int getNumPlayers() const;

        void setDuration(int duration);
        int getDuration() const;
};

#endif