# Persona
World-class C++ Developer

# Background Information
In the following loadGameStateFromFile of the C++ History Object, we need to implement the deserialization logic based on how GameState is structured.

# Your Goal
Please implement the deserialization logic based on how the GameState  and Player Objects are structured.

```cpp
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
        _logger->logError("Unable to open file for reading: " + filename, __FUNCTION__);
    }
    return gameState;
}

std::vector<std::string> History::getSavedGameStatesList() {
    std::vector<std::string> fileList;
    std::string directory = "./"; // Directory where game state files are saved
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            fileList.push_back(entry.path().filename().string());
        }
    }
    return fileList;
}


## GameState including Player Objects that need a serialization process
```cpp
#ifndef GameState_h
#define GameState_h

#include "../Player/Player.h"
#include <map>
#include <string>

class GameState {
public:
    GameState();
    ~GameState() ;

    int getUpdateDisplayDelay() ;
    void stopGameRunning() ;
    int gameRunning() ;
    void setGameHistory(std::map<int, int> game_history) ;
    std::map<int, int> getGameHistory() ;
    void setCurrentAction(std::string currentAction) ;
    std::string getCurrentAction() ;
    void setCurrentSet(int current_set) ;
    int getCurrentSet() ;
    void setWinDelay(int winDelay) ;
    int getWinDelay() ;
    void setPointFlash(int pointFlash) ;
    int getPointFlash() ;
    void setServe(int serve) ;
    int getServe() ;
    void setTieBreak(int tieBreak) ;
    int getTieBreak() ;
    void setTieLEDsOn(int tieLEDsOn) ;
    int getTieLEDsOn() ;
    void setServeSwitch(int serveSwitch) ;
    int getServeSwitch() ;
    void setPlayerButton(int playerButton) ;
    int getPlayerButton() ;
    void setUndo(int undo) ;
    int getUndo() ;
    void setStarted(int started) ;
    int getStarted() ;
    void setSetTieBreak(int setTieBreak) ;
    int getSetTieBreak() ;
    void setRotaryChange(int rotaryChange) ;
    int getRotaryChange() ;
    void setPreviousTime(unsigned long previousTime) ;
    unsigned long getPreviousTime() ;
    void setRotaryPosition(int rotaryPosition) ;
    int getRotaryPosition() ;
    void setPrevRotaryPosition(int prevRotaryPosition) ;
    int getPrevRotaryPosition() ;
    void setFreezePlayerButton(int freezePlayerButton) ;
    int getFreezePlayerButton() ;
    void setP1PointsMem(int p1PointsMem) ;
    int getP1PointsMem() ;
    void setP2PointsMem(int p2PointsMem) ;
    int getP2PointsMem() ;
    void setP1GamesMem(int p1GamesMem) ;
    int getP1GamesMem() ;
    void setP2GamesMem(int p2GamesMem) ;
    int getP2GamesMem() ;
    void setP1SetsMem(int p1SetsMem) ;
    int getP1SetsMem() ;
    void setP2SetsMem(int p2SetsMem) ;
    int getP2SetsMem() ;
    void setNow(unsigned long now) ;
    unsigned long getNow() ;
    void setToggle(int toggle) ;
    int getToggle() ;
    void setTieBreakOnly(int tieBreakOnly) ;
    int getTieBreakOnly() ;
    void setButtonDelay(int buttonDelay) ;
    int getButtonDelay() ;
    void setFlashDelay(unsigned long flashDelay) ;
    unsigned long getFlashDelay() ;
    void setGameWinPulseCount(int gameWinPulseCount) ;
    int getGameWinPulseCount() ;
    void setTieBreakMem(int tieBreakMem) ;
    int getTieBreakMem() ;
    void setSetTieBreakMem(int setTieBreakMem) ;
    int getSetTieBreakMem() ;
    void setPlayer1Points(int player1Points) ;
    int getPlayer1Points() ;
    void setPlayer2Points(int player2Points) ;
    int getPlayer2Points() ;
    void setPlayer1Games(int player1Games) ;
    int getPlayer1Games() ;
    void setPlayer2Games(int player2Games) ;
    int getPlayer2Games() ;
    void setPlayer1Sets(int player1Sets) ;
    int getPlayer1Sets() ;
    void setPlayer2Sets(int player2Sets) ;
    int getPlayer2Sets() ;
    void setPlayer1Matches(int player1Matches) ;
    int getPlayer1Matches() ;
    void setPlayer2Matches(int player2Matches) ;
    int getPlayer2Matches() ;
    void setPlayer1SetHistory(std::map<int, int> player1_set_history) ;
    std::map<int, int> getPlayer1SetHistory() ;
    void setPlayer2SetHistory(std::map<int, int> player2_set_history) ;
    std::map<int, int> getPlayer2SetHistory() ;

private:
    int _current_set;
    int _player1_points;  
    int _player2_points;
    int _player1_games;   
    int _player2_games;
    int _player1_sets;    
    int _player2_sets;
    int _player1_matches; 
    int _player2_matches;
    int _pointFlash;
    int _serve;
    int _tieBreak;
    int _setTieBreak;
    int _tieLEDsOn;
    int _started;
    int _serveSwitch;
    int _playerButton;
    int _undo;
    int _rotaryPosition;
    int _prevRotaryPosition;
    int _rotaryChange;
    unsigned long _now;
    unsigned long _previousTime;
    int _freezePlayerButton;
    int _p1PointsMem; 
    int _p2PointsMem;
    int _p1GamesMem;  
    int _p2GamesMem;
    int _p1SetsMem;   
    int _p2SetsMem;
    int _toggle;
    int _tieBreakOnly;
    int _winDelay;
    int _buttonDelay;
    unsigned long _flashDelay;
    int _gameWinPulseCount;
    int _setTieBreakMem;
    int _tieBreakMem;
    int _gameRunning;
    std::string _currentAction;
    std::map<int, int> _player1_set_history;
    std::map<int, int> _player2_set_history;
    std::map<int, int> _game_history;
};
#endif

#ifndef Player_h
#define Player_h

#include "../GameState/GameState.h"
#include "../TennisConstants/TennisConstants.h"
#include <map>

class GameState;
class Player {
public:
    Player( GameState* gameState, int player_number );
    ~Player();
    
    void setOpponent(    Player* opponent                 );
    Player* getOpponent();

    void setSets( GameState* gameState, int sets );
    int getSets();

    void setPoints( int points  );
    int getPoints();

    void setGames( int game_value  ); // it knows the current set
    int getGames();

    void setServeSwitch( int serve_switch                 );
    int getServeSwitch();

    void setMatches(     int matches );
    int getMatches();

    void setMode(        int mode    );
    int getMode();

    void setSetting(     int setting );
    int getSetting();

    void setSetHistory(  int set, int score               );
    std::map<int, int> getSetHistory();

    void setGameHistory( int game, int score              );
    std::map<int, int> getGameHistory();

    int incrementSetting();

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

#endif
```