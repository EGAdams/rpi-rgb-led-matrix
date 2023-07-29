#ifndef Player_h
#define Player_h

#include "IPlayer.h"
#include "../GameState/IGameState.h"
#include "../TennisConstants/TennisConstants.h"

class Player : public IPlayer {
public:
    Player( IGameState* gameState, int player_number );
    ~Player() override;

    void setOpponent(    IPlayer* opponent                 ) override;
    IPlayer* getOpponent() override;

    void setSets( IGameState * gameState, int sets ) override;
    int getSets() override;

    void setPoints(      int points                       ) override;
    int getPoints() override;

    void setGames( int game_value  ) override; // it knows the current set
    int getGames() override;

    void setServeSwitch( int serve_switch                 ) override;
    int getServeSwitch() override;

    void setMatches(     int matches                      ) override;
    int getMatches() override;

    void setMode(        int mode                         ) override;
    int getMode() override;

    void setSetting(     int setting                      ) override;
    int getSetting() override;

    void setSetHistory(  int set, int score               ) override;
    std::map<int, int> getSetHistory() override;

    void setGameHistory( int game, int score              ) override;
    std::map<int, int> getGameHistory() override;

    int incrementSetting() override;

    int number() override; // only set during construction

private:
    IGameState* _gameState;
    int _playerNumber;
    IPlayer* _opponent;
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