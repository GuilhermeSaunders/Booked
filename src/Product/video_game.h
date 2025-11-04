#ifndef VIDEO_GAME_H
#define VIDEO_GAME_H

#include "product.h"
#include <string>

/**
 * @class Video_Game
 * @brief Represents a Video Game product, inheriting from Product.
 *
 * Holds video-game-specific data like style and platform.
 * This object is immutable after construction.
 */
class Video_Game : public Product {

private:
    std::string style;
    std::string plataform; // Matches DBSchema PLATAFORM
    int duration;      // in hours
    int num_players;

public:
    /**
     * @brief Constructor for Video_Game.
     */
    Video_Game(float rentValue,
               std::string owner,
               std::string name,
               std::string description,
               std::string idiom,
               std::string recommendedAge,
               std::string genre,
               std::string style,
               std::string plataform,
               int duration,
               int num_players);

    ~Video_Game() override = default;

    // --- Getters (Used by Repository to save data) ---
    const std::string& getStyle() const;
    const std::string& getPlataform() const;
    int getNumPlayers() const;
    int getDuration() const;

    /**
     * @brief Gets the type as a string.
     * @return "VIDEOGAME"
     */
    std::string getType() const override {
        return "VIDEOGAME";
    }
};

#endif