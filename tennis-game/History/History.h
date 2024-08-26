#ifndef History_h
#define History_h

#include <stack>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include "../GameState/GameState.h"
#include "../Logger/Logger.h"

class History {
 public:
    History();
    ~History();
    void push( GameState gameState );
    GameState pop();
    int size();
    void decrementWinningPlayerScore( Player* player );
    void saveGameStateToFile(const GameState& gameState, const std::string& filename);
    GameState loadGameStateFromFile(const std::string& filename);
    std::vector<std::string> getSavedGameStatesList();

 private:
    std::stack< GameState > _history;
    Logger* _logger; };
#endif
