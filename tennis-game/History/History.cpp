#include "History.h"

History::History() { _logger = new Logger( "History" ); }
History::~History() { delete _logger; }

void History::push( GameState state ) { _history.push( state );}

int History::size() { return _history.size(); }

GameState History::pop() {
    GameState lastGameState = _history.top();
    _history.pop();
    return lastGameState;
}

void History::decrementWinningPlayerScore( Player* player ) {
    GameState gamestate = _history.top();   // populate game state to modify
    _history.pop();                         // clear it, well put it back in a second..
    if ( player->number() == 0 ) {
        gamestate.setPlayer1Points( gamestate.getPlayer1Points() - 1 );
        gamestate.setP1PointsMem( gamestate.getP1PointsMem() - 1 );
    } else {
        gamestate.setPlayer2Points( gamestate.getPlayer2Points() - 1 );
        gamestate.setP2PointsMem( gamestate.getP2PointsMem() - 1 );
    }
    _history.pop(); // one more for some reason...
    _history.push( gamestate );
}

void History::saveGameStateToFile(const GameState& gameState, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(&gameState), sizeof(GameState));
        outFile.close();
    } else {
        _logger->setName( "saveGameStateToFile" );
        _logger->logUpdate( "Unable to open file for writing: " + filename );
    }
}

GameState History::loadGameStateFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    GameState gameState;
    if (inFile.is_open()) {
        inFile.read(reinterpret_cast<char*>(&gameState), sizeof(GameState));
        inFile.close();
    } else {
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
