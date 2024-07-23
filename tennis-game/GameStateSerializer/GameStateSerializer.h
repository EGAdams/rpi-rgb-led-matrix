#ifndef GAMESTATESERIALIZER_H
#define GAMESTATESERIALIZER_H

#include <string>
#include <fstream>
#include <iostream>

#include "../GameState/GameState.h"
#include "../FileReader/IFileReader.h"
#include "../nlohmann/json.hpp"

class GameStateSerializer {
public:
    explicit GameStateSerializer(IFileReader* fileReader) : fileReader_(fileReader) {}

    // Serialize the GameState object to a file
    static bool serialize(const GameState& gameState, const std::string& filePath);

    // Deserialize a GameState object from a file
    bool deserialize(GameState& gameState, const std::string& filePath);

private:
    IFileReader* fileReader_;
};

#endif // GAMESTATESERIALIZER_H
