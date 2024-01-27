#include "./GameStateSerializer.h"

using json = nlohmann::json;

bool GameStateSerializer::serialize( const GameState& gameState, const std::string& filePath ) {
    json nLohmannJsonObject;

    // Serialize each field of GameState into the JSON object `nLohmannJsonObject`
    nLohmannJsonObject["current_set"]     = gameState.getCurrentSet();
    nLohmannJsonObject["current_action"]  = gameState.getCurrentAction();
    nLohmannJsonObject["player_1_points"] = gameState.getPlayer1Points();
    nLohmannJsonObject["player_2_points"] = gameState.getPlayer2Points();
    
    // Serialize other fields...

    std::ofstream file( filePath );
    if ( !file.is_open() ) {
        std::cerr << "Error opening file for writing." << std::endl;
        return false;
    }

    try {
        file << nLohmannJsonObject.dump(); // Convert JSON object to string and write to file
    }
    catch ( const std::exception& e ) {
        std::cerr << "Exception occurred during serialization: " << e.what() << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}






bool GameStateSerializer::deserialize(GameState& gameState, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return false;
    }

    json nLohmannJsonObject;
    try {
        file >> nLohmannJsonObject; // Read the file content into the JSON object `nLohmannJsonObject`
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred during deserialization: " << e.what() << std::endl;
        file.close();
        return false;
    }

    // Deserialize each field from the JSON object `nLohmannJsonObject` into GameState
    gameState.setCurrentSet(nLohmannJsonObject["current_set"]);
    gameState.setCurrentAction(nLohmannJsonObject["current_action"]);
    // Deserialize other fields...
    
    file.close();
    return true;
}
