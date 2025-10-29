#include "video_game.h"

        Video_Game :: Video_Game(float rentValue,
                std::string id,
                std::string owner,
                std::string name,
                std::string description,  std::string idiom,
                std::string recommendedAge,  std::string genre, bool isRented, std::string style, std::string plataform, int duration)

        : Product( rentValue, std::move(id),
        std::move(owner),
        std::move(name),
        std::move(description), std::move(idiom),
        std::move(recommendedAge), std::move(genre),  isRented),
        style(std::move(style)),
        plataform(std::move(plataform)),
        num_players(std::move(num_players)),
        duration(duration){}

        Video_Game::~Video_Game() = default;

        void Video_Game :: setStyle(std::string style) {this->style = std::move(style);}
        const std::string& Video_Game :: getStyle() const {return style;}

        void Video_Game :: setPlataform(std::string plataform) {this->plataform = std::move(plataform);}
        const std::string& Video_Game :: getPlataform() const {return plataform;}

        void Video_Game :: setNumPlayers(int num_players) {this->num_players = std::move(num_players);}
        int Video_Game :: getNumPlayers() const {return num_players;}

        void Video_Game :: setDuration(int duration) {this->duration = duration;}
        int Video_Game :: getDuration() const {return duration;}