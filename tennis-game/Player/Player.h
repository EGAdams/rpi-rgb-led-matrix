#ifndef Player_h
#define Player_h

#include "Player.h"
#include "../GameState/IGameState.h"
#include "../TennisConstants/TennisConstants.h"

class Player {
public:
    Player( IGameState* gameState, int player_number );
    ~Player();

    void setOpponent(    Player* opponent                 );
    Player* getOpponent();

    void setSets( IGameState * gameState, int sets );
    int getSets();

    void setPoints(      int points                       );
    int getPoints();

    void setGames( int game_value  ); // it knows the current set
    int getGames();

    void setServeSwitch( int serve_switch                 );
    int getServeSwitch();

    void setMatches(     int matches                      );
    int getMatches();

    void setMode(        int mode                         );
    int getMode();

    void setSetting(     int setting                      );
    int getSetting();

    void setSetHistory(  int set, int score               );
    std::map<int, int> getSetHistory();

    void setGameHistory( int game, int score              );
    std::map<int, int> getGameHistory();

    int incrementSetting();

    int number(); // only set during construction

private:
    IGameState* _gameState;
    int _playerNumber;
    Player* _opponent;
    int _points;
    int _games;
    int _sets;
    int _matches;
    int _mode;
    int _setting;
    int _serve_switch;
    std::map<int, int> _set_history;
    std::map<int, int> _game_history;
};

#endif