#include "board_game.h"
#include <utility> // For std::move

/**
 * @brief Constructor for Board_Game.
 */
Board_Game::Board_Game(float rentValue,
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
      duration(duration),
      num_players(num_players) {
}

// --- Getter Implementations ---

const std::string& Board_Game::getStyle() const {
    return style;
}

int Board_Game::getNumPlayers() const {
    return num_players;
}

int Board_Game::getDuration() const {
    return duration;
}
