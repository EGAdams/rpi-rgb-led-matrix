#ifndef GameState_h
#define GameState_h

#include "../Player/Player.h"
#include <map>
#include <string>

class GameState {
public:
    GameState();
    ~GameState() ;
    int getState() ;
    void setState(int state) ;
    void setMatchBlinkOn();
    void setMatchBlinkOff();
    int  getMatchBlink();
    int getUpdateDisplayDelay() ;
    void stopGameRunning() ;
    int gameRunning() ;
    void setGameHistory(std::map<int, int> game_history ) ;
    std::map<int, int> getGameHistory() ;
    void setCurrentAction(std::string current_action ) ;
    std::string getCurrentAction() const;
    void setCurrentSet(int current_set);
    int getCurrentSet() const;
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
    void setMatchTieBreak(int matchTieBreak) ;
    int getMatchTieBreak() ;
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
    void setMatchTieBreakMem(int setMatchTieBreakMem) ;
    int getMatchTieBreakMem() ;
    void setPlayer1Points(int player1Points) ;
    int getPlayer1Points() const;
    void setPlayer2Points(int player2Points) ;
    int getPlayer2Points() const;
    void setPlayer1Games(int player1Games) ;
    int getPlayer1Games() const;
    void setPlayer2Games(int player2Games) ;
    int getPlayer2Games() const;
    void setPlayer1Sets(int player1Sets) ;
    int getPlayer1Sets() const;
    void setPlayer2Sets(int player2Sets) ;
    int getPlayer2Sets() const;
    void setPlayer1Matches(int player1Matches) ;
    int getPlayer1Matches() const;
    void setPlayer2Matches(int player2Matches) ;
    int getPlayer2Matches() ;
    void setPlayer1SetHistory(std::map<int, int> player1_set_history) ;
    std::map<int, int> getPlayer1SetHistory() ;
    void setPlayer2SetHistory(std::map<int, int> player2_set_history) ;
    std::map<int, int> getPlayer2SetHistory() ;
    void startIgnoreScoreTimer( unsigned long start_time ) {
        _ignore_extra_score_timer = start_time;
    }
    unsigned long getIgnoreScoreTimer()  { return _ignore_extra_score_timer; }


private:
    int _state;
    int _match_blink_on;
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
    int _matchTieBreak;
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
    unsigned long _ignore_extra_score_timer;
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
    int _matchTieBreakMem;
    int _tieBreakMem;
    int _gameRunning;
    std::string _current_action;
    std::map<int, int> _player1_set_history;
    std::map<int, int> _player2_set_history;
    std::map<int, int> _game_history;
};
#endif
