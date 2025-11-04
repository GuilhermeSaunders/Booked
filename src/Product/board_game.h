#ifndef BOARD_GAME_H
#define BOARD_GAME_H

#include "product.h"
#include <string>

/**
 * @class Board_Game
 * @brief Represents a Board Game product, inheriting from Product.
 *
 * Holds board-game-specific data like style and player count.
 * This object is immutable after construction.
 */
class Board_Game : public Product {

private:
    std::string style;
    int duration; // in minutes
    int num_players;

public:
    /**
     * @brief Constructor for Board_Game.
     */
    Board_Game(float rentValue,
               std::string owner,
               std::string name,
               std::string description,
               std::string idiom,
               std::string recommendedAge,
               std::string genre,
               std::string style,
               int num_players,
               int duration);

    ~Board_Game() override = default;

    // --- Getters (Used by Repository to save data) ---
    const std::string& getStyle() const;
    int getNumPlayers() const;
    int getDuration() const;

    /**
     * @brief Gets the type as a string.
     * @return "BOARDGAME"
     */
    std::string getType() const override {
        return "BOARDGAME";
    }
};

#endif