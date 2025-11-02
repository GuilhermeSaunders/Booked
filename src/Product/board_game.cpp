#include "board_game.h"

Board_Game :: Board_Game(float rentValue,
                        std::string owner,
                        std::string name,
                        std::string description,  
                        std::string idiom,
                        std::string recommendedAge,  
                        std::string genre, 
                        std::string style, 
                        int num_players, 
                        int duration)

: Product(rentValue, 
        std::move(owner),
        std::move(name),
        std::move(description), 
        std::move(idiom),
        std::move(recommendedAge), 
        std::move(genre)),
        style(std::move(style)),
        num_players(num_players),
        duration(duration){}

        Board_Game::~Board_Game() = default;

        void Board_Game :: setStyle(std::string style) {this->style = std::move(style);}
        const std::string& Board_Game :: getStyle() const {return style;}

        void Board_Game :: setNumPlayers(int num_players) {this->num_players = std::move(num_players);}
        int Board_Game :: getNumPlayers() const {return num_players;}

        void Board_Game :: setDuration(int duration) {this->duration = duration;}
        int Board_Game :: getDuration() const {return duration;}