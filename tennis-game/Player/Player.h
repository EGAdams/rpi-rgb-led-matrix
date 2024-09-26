#ifndef Player_h
#define Player_h

#include "../GameState/GameState.h"
#include "../TennisConstants/TennisConstants.h"
#include <map>

class GameState;
class Player {
public:
    Player(GameState* gameState, int player_number);
    ~Player();

    virtual void setOpponent(Player* opponent);  // Mark as virtual
    virtual Player* getOpponent();               // Mark as virtual

    virtual void setSets(GameState* gameState, int sets); // Mark as virtual
    virtual int getSets();                                // Mark as virtual

    virtual void setSets(int sets);             // Mark as virtual
    virtual void setPoints(int points);         // Mark as virtual
    virtual int getPoints();                    // Mark as virtual

    virtual void setGames(int game_value);      // Mark as virtual
    virtual int getGames();                     // Mark as virtual

    virtual void setServeSwitch(int serve_switch);  // Mark as virtual
    virtual int getServeSwitch();                   // Mark as virtual

    virtual void setMatches(int matches);           // Mark as virtual
    virtual int getMatches();                       // Mark as virtual

    virtual void setMode(int mode);                 // Mark as virtual
    virtual int getMode();                          // Mark as virtual

    virtual void setSetting(int setting);           // Mark as virtual
    virtual int getSetting();                       // Mark as virtual

    virtual void setSetHistory(int set, int score); // Mark as virtual
    virtual std::map<int, int> getSetHistory();     // Mark as virtual

    virtual void setSetHistory(std::map<int, int>);  // Mark as virtual
    virtual void setGameHistory(int game, int score); // Mark as virtual
    virtual std::map<int, int> getGameHistory();     // Mark as virtual

    virtual void clearGameHistory();                 // Mark as virtual
    virtual void clearSetHistory();                  // Mark as virtual

    virtual int incrementSetting();                  // Mark as virtual
    int number(); // only set during construction

private:
    GameState* _gameState;
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
#endif /* Player_h */