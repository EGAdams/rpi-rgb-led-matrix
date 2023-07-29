#ifndef IPlayer_h
#define IPlayer_h

#include <map>
#include "../GameState/IGameState.h"
#include "../TennisConstants/TennisConstants.h"

class IPlayer {
public:
    virtual ~IPlayer() = default;

    virtual void setOpponent(    IPlayer* opponent               ) = 0;
    virtual IPlayer* getOpponent() = 0;

    virtual void setPoints(      int points                      ) = 0;
    virtual int getPoints() = 0;

    virtual void setSets( IGameState* gameState, int game_value ) = 0;
    virtual int  getSets() = 0;

    virtual void setGames( int games                      ) = 0;
    virtual int getGames() = 0;

    virtual void setServeSwitch( int serve_switch                ) = 0;
    virtual int getServeSwitch() = 0;

    virtual void setMatches(     int matches                     ) = 0;
    virtual int getMatches() = 0;

    virtual void setMode(        int mode                        ) = 0;
    virtual int getMode() = 0;

    virtual void setSetting(     int setting                     ) = 0;
    virtual int getSetting() = 0;

    virtual void setSetHistory(  int set, int score              ) = 0;
    virtual std::map<int, int> getSetHistory() = 0;

    virtual void setGameHistory( int game, int score             ) = 0;
    virtual std::map<int, int> getGameHistory() = 0;

    virtual int incrementSetting() = 0;

    virtual int number() = 0; // only set during construction
};

#endif