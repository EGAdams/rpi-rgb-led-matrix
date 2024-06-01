#include "History.h"

History::History() { _logger = new Logger( "History" ); }
History::~History() { delete _logger; }

void History::push( GameState state ) { _history.push( state );}

int History::size() { return _history.size(); }

GameState History::pop() {
    GameState lastGameState = _history.top();
    _history.pop();
    return lastGameState; }

void History::saveGameStateToFile(const GameState& gameState, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile.is_open()) {
        // Serialize gameState object to file
        // You need to implement the serialization logic based on how GameState is structured
        // This is a placeholder for the serialization logic
        outFile.write(reinterpret_cast<const char*>(&gameState), sizeof(GameState));
        outFile.close();
    } else {
        std::cout << "setting logger name to saveGameStateToFile..." << std:: endl;
        _logger->setName( "saveGameStateToFile" );
        _logger->logUpdate( "Unable to open file for writing: " + filename );
    }
}

GameState History::loadGameStateFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    GameState gameState;
    if (inFile.is_open()) {
        // Deserialize gameState object from file
        // You need to implement the deserialization logic based on how GameState is structured
        // This is a placeholder for the deserialization logic
        inFile.read(reinterpret_cast<char*>(&gameState), sizeof(GameState));
        inFile.close();
    } else {
        std::cout << "setting logger name to loadGameStateFromFile... " << std::endl;
        _logger->setName( "loadGameStateFromFile" );
        _logger->logUpdate( "Unable to open file for reading: " + filename );
    }
    return gameState;
}

std::vector<std::string> History::getSavedGameStatesList() {
    std::vector<std::string> fileList;
    std::string directory = "./"; // Directory where game state files are saved

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            // Check if the entry is a regular file
            if (entry->d_type == DT_REG) {
                fileList.push_back(entry->d_name);
            }
        }
        closedir(dir);
    } else {
        // Handle error in opening directory
    }

    return fileList;
}
