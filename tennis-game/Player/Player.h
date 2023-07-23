#ifndef IPlayer_h
#define IPlayer_h

#include <map>
#include "../GameState/GameState.h"
#include "../TennisConstants/TennisConstants.h"

class GameState;

class IPlayer {
public:
    virtual ~IPlayer() = default;

    virtual void setOpponent(    IPlayer* opponent                 ) = 0; virtual IPlayer* getOpponent() = 0;
    virtual void setPoints(      int points                       ) = 0; virtual int getPoints() = 0;
    virtual void setGames(       int games                        ) = 0; virtual int getGames() = 0;
    virtual void setMatches(     int matches                      ) = 0; virtual int getMatches() = 0;
    virtual void setMode(        int mode                         ) = 0; virtual int getMode() = 0;
    virtual void setSetting(     int setting                      ) = 0; virtual int getSetting() = 0;
    virtual void setSetHistory(  int set, int score               ) = 0; virtual std::map<int, int> getSetHistory() = 0;
    virtual void setGameHistory( int game, int score              ) = 0; virtual std::map<int, int> getGameHistory() = 0;
    virtual int incrementSetting() = 0;
    virtual int number() = 0; // only set during construction
};

class Player : public IPlayer {
public:
    Player( GameState* gameState, int player_number );
    ~Player() override;

    void setOpponent(    IPlayer* opponent                 ) override; IPlayer* getOpponent() override;
    void setPoints(      int points                       ) override; int getPoints() override;
    void setGames(       int games                        ) override; int getGames() override;
    void setMatches(     int matches                      ) override; int getMatches() override;
    void setMode(        int mode                         ) override; int getMode() override;
    void setSetting(     int setting                      ) override; int getSetting() override;
    void setSetHistory(  int set, int score               ) override; std::map<int, int> getSetHistory() override;
    void setGameHistory( int game, int score              ) override; std::map<int, int> getGameHistory() override;
    int incrementSetting() override;
    int number() override; // only set during construction

private:
    GameState* _gameState;
    int _playerNumber;
    IPlayer* _opponent;
    int _points;
    int _games;
    int _sets;
    int _matches;
    int _mode;
    int _setting;
    std::map<int, int> _set_history;
    std::map<int, int> _game_history;
};

#endif