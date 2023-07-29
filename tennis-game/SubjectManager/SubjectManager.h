#include "../GameState/GameState.h"
#include "../TennisConstants/TennisConstants.h"
#include "../Logger/Logger.h"
#include <fstream>
#include <string>

class SubjectManager {
public:
    SubjectManager();
    ~SubjectManager();
    void gameStateUpdate( GameState* gameState, IPlayer* player1, IPlayer* player2 );
private:
    Logger* _logger;
};
