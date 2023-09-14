#ifndef GameState_h
#define GameState_h

#include "IGameState.h"
#include "../Player/Player.h"
#include <map>
#include <string>

class GameState : public IGameState {
public:
    GameState();
    ~GameState() override;

    int getUpdateDisplayDelay() override;
    void stopGameRunning() override;
    int gameRunning() override;
    void setGameHistory(std::map<int, int> game_history) override;
    std::map<int, int> getGameHistory() override;
    void setCurrentAction(std::string currentAction) override;
    std::string getCurrentAction() override;
    void setCurrentSet(int current_set) override;
    int getCurrentSet() override;
    void setWinDelay(int winDelay) override;
    int getWinDelay() override;
    void setPointFlash(int pointFlash) override;
    int getPointFlash() override;
    void setServe(int serve) override;
    int getServe() override;
    void setTieBreak(int tieBreak) override;
    int getTieBreak() override;
    void setTieLEDsOn(int tieLEDsOn) override;
    int getTieLEDsOn() override;
    void setServeSwitch(int serveSwitch) override;
    int getServeSwitch() override;
    void setPlayerButton(int playerButton) override;
    int getPlayerButton() override;
    void setUndo(int undo) override;
    int getUndo() override;
    void setStarted(int started) override;
    int getStarted() override;
    void setSetTieBreak(int setTieBreak) override;
    int getSetTieBreak() override;
    void setRotaryChange(int rotaryChange) override;
    int getRotaryChange() override;
    void setPreviousTime(unsigned long previousTime) override;
    unsigned long getPreviousTime() override;
    void setRotaryPosition(int rotaryPosition) override;
    int getRotaryPosition() override;
    void setPrevRotaryPosition(int prevRotaryPosition) override;
    int getPrevRotaryPosition() override;
    void setFreezePlayerButton(int freezePlayerButton) override;
    int getFreezePlayerButton() override;
    void setP1PointsMem(int p1PointsMem) override;
    int getP1PointsMem() override;
    void setP2PointsMem(int p2PointsMem) override;
    int getP2PointsMem() override;
    void setP1GamesMem(int p1GamesMem) override;
    int getP1GamesMem() override;
    void setP2GamesMem(int p2GamesMem) override;
    int getP2GamesMem() override;
    void setP1SetsMem(int p1SetsMem) override;
    int getP1SetsMem() override;
    void setP2SetsMem(int p2SetsMem) override;
    int getP2SetsMem() override;
    void setNow(unsigned long now) override;
    unsigned long getNow() override;
    void setToggle(int toggle) override;
    int getToggle() override;
    void setTieBreakOnly(int tieBreakOnly) override;
    int getTieBreakOnly() override;
    void setButtonDelay(int buttonDelay) override;
    int getButtonDelay() override;
    void setFlashDelay(unsigned long flashDelay) override;
    unsigned long getFlashDelay() override;
    void setGameWinPulseCount(int gameWinPulseCount) override;
    int getGameWinPulseCount() override;
    void setTieBreakMem(int tieBreakMem) override;
    int getTieBreakMem() override;
    void setSetTieBreakMem(int setTieBreakMem) override;
    int getSetTieBreakMem() override;
    void setPlayer1Points(int player1Points) override;
    int getPlayer1Points() override;
    void setPlayer2Points(int player2Points) override;
    int getPlayer2Points() override;
    void setPlayer1Games(int player1Games) override;
    int getPlayer1Games() override;
    void setPlayer2Games(int player2Games) override;
    int getPlayer2Games() override;
    void setPlayer1Sets(int player1Sets) override;
    int getPlayer1Sets() override;
    void setPlayer2Sets(int player2Sets) override;
    int getPlayer2Sets() override;
    void setPlayer1Matches(int player1Matches) override;
    int getPlayer1Matches() override;
    void setPlayer2Matches(int player2Matches) override;
    int getPlayer2Matches() override;
    void setPlayer1SetHistory(std::map<int, int> player1_set_history) override;
    std::map<int, int> getPlayer1SetHistory() override;
    void setPlayer2SetHistory(std::map<int, int> player2_set_history) override;
    std::map<int, int> getPlayer2SetHistory() override;

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
