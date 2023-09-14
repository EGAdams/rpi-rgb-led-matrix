#ifndef IGameState_h
#define IGameState_h

#include <map>
#include <string>

class IGameState {
public:
    virtual ~IGameState() = default;
    virtual int getUpdateDisplayDelay() = 0;
    virtual void stopGameRunning() = 0;
    virtual int gameRunning() = 0;
    virtual void setGameHistory(std::map<int, int> game_history) = 0;
    virtual std::map<int, int> getGameHistory() = 0;
    virtual void setCurrentAction(std::string currentAction) = 0;
    virtual std::string getCurrentAction() = 0;
    virtual void setCurrentSet(int current_set) = 0;
    virtual int getCurrentSet() = 0;
    virtual void setWinDelay(int winDelay) = 0;
    virtual int getWinDelay() = 0;
    virtual void setPointFlash(int pointFlash) = 0;
    virtual int getPointFlash() = 0;
    virtual void setServe(int serve) = 0;
    virtual int getServe() = 0;
    virtual void setTieBreak(int tieBreak) = 0;
    virtual int getTieBreak() = 0;
    virtual void setTieLEDsOn(int tieLEDsOn) = 0;
    virtual int getTieLEDsOn() = 0;
    virtual void setServeSwitch(int serveSwitch) = 0;
    virtual int getServeSwitch() = 0;
    virtual void setPlayerButton(int playerButton) = 0;
    virtual int getPlayerButton() = 0;
    virtual void setUndo(int undo) = 0;
    virtual int getUndo() = 0;
    virtual void setStarted(int started) = 0;
    virtual int getStarted() = 0;
    virtual void setSetTieBreak(int setTieBreak) = 0;
    virtual int getSetTieBreak() = 0;
    virtual void setRotaryChange(int rotaryChange) = 0;
    virtual int getRotaryChange() = 0;
    virtual void setPreviousTime(unsigned long previousTime) = 0;
    virtual unsigned long getPreviousTime() = 0;
    virtual void setRotaryPosition(int rotaryPosition) = 0;
    virtual int getRotaryPosition() = 0;
    virtual void setPrevRotaryPosition(int prevRotaryPosition) = 0;
    virtual int getPrevRotaryPosition() = 0;
    virtual void setFreezePlayerButton(int freezePlayerButton) = 0;
    virtual int getFreezePlayerButton() = 0;
    virtual void setP1PointsMem(int p1PointsMem) = 0;
    virtual int getP1PointsMem() = 0;
    virtual void setP2PointsMem(int p2PointsMem) = 0;
    virtual int getP2PointsMem() = 0;
    virtual void setP1GamesMem(int p1GamesMem) = 0;
    virtual int getP1GamesMem() = 0;
    virtual void setP2GamesMem(int p2GamesMem) = 0;
    virtual int getP2GamesMem() = 0;
    virtual void setP1SetsMem(int p1SetsMem) = 0;
    virtual int getP1SetsMem() = 0;
    virtual void setP2SetsMem(int p2SetsMem) = 0;
    virtual int getP2SetsMem() = 0;
    virtual void setNow(unsigned long now) = 0;
    virtual unsigned long getNow() = 0;
    virtual void setToggle(int toggle) = 0;
    virtual int getToggle() = 0;
    virtual void setTieBreakOnly(int tieBreakOnly) = 0;
    virtual int getTieBreakOnly() = 0;
    virtual void setButtonDelay(int buttonDelay) = 0;
    virtual int getButtonDelay() = 0;
    virtual void setFlashDelay(unsigned long flashDelay) = 0;
    virtual unsigned long getFlashDelay() = 0;
    virtual void setGameWinPulseCount(int gameWinPulseCount) = 0;
    virtual int getGameWinPulseCount() = 0;
    virtual void setTieBreakMem(int tieBreakMem) = 0;
    virtual int getTieBreakMem() = 0;
    virtual void setSetTieBreakMem(int setTieBreakMem) = 0;
    virtual int getSetTieBreakMem() = 0;
    virtual void setPlayer1Points(int player1Points) = 0;
    virtual int getPlayer1Points() = 0;
    virtual void setPlayer2Points(int player2Points) = 0;
    virtual int getPlayer2Points() = 0;
    virtual void setPlayer1Games(int player1Games) = 0;
    virtual int getPlayer1Games() = 0;
    virtual void setPlayer2Games(int player2Games) = 0;
    virtual int getPlayer2Games() = 0;
    virtual void setPlayer1Sets(int player1Sets) = 0;
    virtual int getPlayer1Sets() = 0;
    virtual void setPlayer2Sets(int player2Sets) = 0;
    virtual int getPlayer2Sets() = 0;
    virtual void setPlayer1Matches(int player1Matches) = 0;
    virtual int getPlayer1Matches() = 0;
    virtual void setPlayer2Matches(int player2Matches) = 0;
    virtual int getPlayer2Matches() = 0;
    virtual void setPlayer1SetHistory(std::map<int, int> player1_set_history) = 0;
    virtual std::map<int, int> getPlayer1SetHistory() = 0;
    virtual void setPlayer2SetHistory(std::map<int, int> player2_set_history) = 0;
    virtual std::map<int, int> getPlayer2SetHistory() = 0;
};

#endif