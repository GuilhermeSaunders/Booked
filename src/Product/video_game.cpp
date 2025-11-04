#include "video_game.h"
#include <utility> // For std::move

/**
 * @brief Constructor for Video_Game.
 */
Video_Game::Video_Game(float rentValue,
                       std::string owner,
                       std::string name,
                       std::string description,
                       std::string idiom,
                       std::string recommendedAge,
                       std::string genre,
                       std::string style,
                       std::string plataform,
                       int duration,
                       int num_players)
    : Product(rentValue,
              std::move(owner),
              std::move(name),
              std::move(description),
              std::move(idiom),
              std::move(recommendedAge),
              std::move(genre)),
      style(std::move(style)),
      plataform(std::move(plataform)), // Matches .h spelling
      duration(duration),
      num_players(num_players) {
}

// --- Getter Implementations ---

const std::string& Video_Game::getStyle() const {
    return style;
}

const std::string& Video_Game::getPlataform() const {
    return plataform;
}

int Video_Game::getNumPlayers() const {
    return num_players;
}

int Video_Game::getDuration() const {
    return duration;
}