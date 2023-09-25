# Your role
- Expert C++ debugger

Your task
- Fix the make error
  
```bash
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o GameWinSequence.o GameWinSequence/GameWinSequence.cpp
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o SetWin.o SetWin/SetWin.cpp
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o SetHistoryText.o SetHistoryText/SetHistoryText.cpp
In file included from SetHistoryText/../GameState/GameState.h:5,
                 from SetHistoryText/SetHistoryText.h:6,
                 from SetHistoryText/SetHistoryText.cpp:1:
SetHistoryText/../GameState/../Player/Player.h:10:22: error: expected ‘)’ before ‘*’ token
   10 |     Player( GameState* gameState, int player_number );
      |           ~          ^
      |                      )
SetHistoryText/../GameState/../Player/Player.h:16:19: error: ‘GameState’ has not been declared
   16 |     void setSets( GameState * gameState, int sets );
      |                   ^~~~~~~~~
SetHistoryText/../GameState/../Player/Player.h:48:5: error: ‘GameState’ does not name a type; did you mean ‘GameState_h’?
   48 |     GameState* _gameState;
      |     ^~~~~~~~~
      |     GameState_h
make: *** [Makefile:173: SetHistoryText.o] Error 1
adamsl@DESKTOP-BKHEBT0:~/rpi-rgb-led-matrix/tennis-game$ make clean
rm -f GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o NumberDrawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o NumberDrawer.o CanvasCreator.o FontLoader.o GameTimer.o tennis-game tennis-game-manual Mode1ScoreTest
adamsl@DESKTOP-BKHEBT0:~/rpi-rgb-led-matrix/tennis-game$ make
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o GameWinSequence.o GameWinSequence/GameWinSequence.cpp
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o SetWin.o SetWin/SetWin.cpp
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o SetHistoryText.o SetHistoryText/SetHistoryText.cpp
In file included from SetHistoryText/../GameState/GameState.h:5,
                 from SetHistoryText/SetHistoryText.h:6,
                 from SetHistoryText/SetHistoryText.cpp:1:
SetHistoryText/../GameState/../Player/Player.h:10:22: error: expected ‘)’ before ‘*’ token
   10 |     Player( GameState* gameState, int player_number );
      |           ~          ^
      |                      )
SetHistoryText/../GameState/../Player/Player.h:16:19: error: ‘GameState’ has not been declared
   16 |     void setSets( GameState * gameState, int sets );
      |                   ^~~~~~~~~
SetHistoryText/../GameState/../Player/Player.h:48:5: error: ‘GameState’ does not name a type; did you mean ‘GameState_h’?
   48 |     GameState* _gameState;
      |     ^~~~~~~~~
      |     GameState_h
make: *** [Makefile:173: SetHistoryText.o] Error 1
```

## Relevant C++ files
```cpp
class Player {
public:
    Player( GameState* gameState, int player_number );
    ~Player();

    void setOpponent(    Player* opponent                 );
    Player* getOpponent();

    void setSets( GameState* gameState, int sets );
    int getSets();

    void setPoints( int points                       );
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


class SetHistoryText {
 public:
    SetHistoryText( GameState* gameState );
    ~SetHistoryText();
    std::string getSetHistoryText( int player );
    std::string _buildString(      int player );

 private: 
    GameState* _gameState; };
#endif

# google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR)
# OBJECTS=SetHistoryTextTest.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o NumberDrawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o
BINARIES=tennis-game tennis-game-manual Mode1ScoreTest

# Where our library resides. It is assumed here that $(RGB_LIB_DISTRIBUTION) has a
# compiled library in lib/
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread -L$(GTEST_LIBDIR) $(GTEST_LIBS)

# To compile image-example
MAGICK_CXXFLAGS?=$(shell GraphicsMagick++-config --cppflags --cxxflags)
MAGICK_LDFLAGS?=$(shell GraphicsMagick++-config --ldflags --libs)

MAIN_OBJECTS=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o NumberDrawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o

MAIN_OBJECTS_MANUAL=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o NumberDrawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o

TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o NumberDrawer.o CanvasCreator.o FontLoader.o GameTimer.o
SCORE_TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o  Mode1Score.o Mode1ScoreTest.o

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

tennis-game: $(MAIN_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

tennis-game-manual: GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o NumberDrawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o
	$(CXX) $^ -o $@ $(LDFLAGS)


test-manual: GameWinSequence.o SetWin.o
	@echo "Temporary rule worked."

Mode1ScoreTest: $( SCORE_TEST_OBJECTS )
	$(CXX) $^ -o $@ $(LDFLAGS)

Mode1ScoreTest.o: Mode1ScoreTest/Mode1ScoreTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

GameWinSequence.o : GameWinSequence/GameWinSequence.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetWin.o : SetWin/SetWin.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetDrawer.o : SetDrawer/SetDrawer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

GameLedTranslator.o : GameLedTranslator/GameLedTranslator.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

CanvasCreator.o : CanvasCreator/CanvasCreator.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

FontLoader.o : FontLoader/FontLoader.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

NumberDrawer.o : NumberDrawer/NumberDrawer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TextDrawer.o : TextDrawer/TextDrawer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

GameTimer.o : GameTimer/GameTimer.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameState.o : GameState/GameState.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameObject.o : GameObject/GameObject.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameModes.o : GameModes/GameModes.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameLeds.o : GameLeds/GameLeds.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

Arduino.o : Arduino/Arduino.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Logger.o : Logger/Logger.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

History.o : History/History.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

ScoreBoard.o : ScoreBoard/ScoreBoard.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Player.o : Player/Player.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

PinInterface.o : PinInterface/PinInterface.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TranslateConstant.o : TranslateConstant/TranslateConstant.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

PointLeds.o : PointLeds/PointLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

PinState.o : PinState/PinState.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TennisConstants.o : TennisConstants/TennisConstants.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1Functions.o : Mode1Functions/Mode1Functions.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode2Functions.o : Mode2Functions/Mode2Functions.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1WinSequences.o : WinSequences/WinSequences.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetLeds.o : SetLeds/SetLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TieLeds.o : TieLeds/TieLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Reset.o : Reset/Reset.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MatchWinSequence.o : MatchWinSequence/MatchWinSequence.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

BatteryTest.o : BatteryTest/BatteryTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

WebLiquidCrystal.o : WebLiquidCrystal/WebLiquidCrystal.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Undo.o : Undo/Undo.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

ServeLeds.o : ServeLeds/ServeLeds.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1Score.o : Mode1Score/Mode1Score.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TieBreaker.o : TieBreaker/TieBreaker.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Inputs.o : Inputs/Inputs.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

WatchTimer.o : WatchTimer/WatchTimer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SubjectManager.o : SubjectManager/SubjectManager.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SetHistoryText.o : SetHistoryText/SetHistoryText.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

tennis-game.o : tennis-game.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

tennis-game-manual.o : tennis-game-manual.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<



clean:
	rm -f $(MAIN_OBJECTS) $(TEST_OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE

print-%:
	@echo $* = $($*)

```