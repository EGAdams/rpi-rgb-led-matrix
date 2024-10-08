# Your role
- Expert C++ Developer
- Expert in CMake
- Expert in Make
- Expert in C++ Debugging


# Your task
- Debug the following make error

## Make error
```bash
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -c -o ScoreBoard.o ScoreBoard/ScoreBoard.cpp
ScoreBoard/ScoreBoard.cpp:79:50: error: ISO C++ forbids declaration of ‘showMatchWinner’ with no type [-fpermissive]
   79 | ScoreBoard::showMatchWinner( std::string message ) {
      |                                                  ^
ScoreBoard/ScoreBoard.cpp:79:1: error: no declaration matches ‘int ScoreBoard::showMatchWinner(std::string)’
   79 | ScoreBoard::showMatchWinner( std::string message ) {
      | ^~~~~~~~~~
In file included from ScoreBoard/ScoreBoard.cpp:1:
ScoreBoard/ScoreBoard.h:29:10: note: candidate is: ‘void ScoreBoard::showMatchWinner(std::string)’
   29 |     void showMatchWinner( std::string message );
      |          ^~~~~~~~~~~~~~~
ScoreBoard/ScoreBoard.h:20:7: note: ‘class ScoreBoard’ defined here
   20 | class ScoreBoard {
      |       ^~~~~~~~~~
make: *** [Makefile:104: ScoreBoard.o] Error 1
```

## ScoreBoard.h
```cpp
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../GameTimer/GameTimer.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../Drawer/Drawer.h"
#include "../CanvasCreator/CanvasCreator.h"
#include "../FontLoader/FontLoader.h"
#include "../SetDrawer/SetDrawer.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#define PLAYER_1_SERVE  0
#define PLAYER_2_SERVE  1
#define BIG_NUMBER_FONT "fonts/fgm_27_ee.bdf"

class ScoreBoard {
public:
    ScoreBoard( Player* player1, Player* player2, GameState* gameState );
    ~ScoreBoard();
    void update();
    bool hasCanvas();
    void clearScreen();
    void drawGames();
    void writeMessage( std::string message );
    void showMatchWinner( std::string message );
    std::string drawPlayerScore(   Player* player );

private:
    Player*                        _player1;
    Player*                        _player2;
    GameState*                     _gameState;
    rgb_matrix::Font               _big_number_font;
    std::unique_ptr<Drawer>  _smallDrawer;
    std::unique_ptr<Drawer>  _playerOneScoreDrawer;
    std::unique_ptr<Drawer>  _playerTwoScoreDrawer;
    std::unique_ptr<Drawer>  _pipeDrawer;
    std::unique_ptr<Drawer>  _bluePipeDrawer;
    std::unique_ptr<SetDrawer>     _setDrawer;
    std::unique_ptr<RGBMatrix>     _canvas;

    void _drawTieBreakerBar( /* void */            );
    int  _characterOffset(   std::string character );
    std::string _translate(  int raw_score         ); };
#endif
```

### Relevant C++ code
## ScoreBoard.cpp
```cpp
ScoreBoard::showMatchWinner( std::string message ) {
    Color color( 255, 255, 0 );
    Color bg_color( 0, 0, 0 );
    Drawer smallDrawer (
        _canvas.get(),
        &_big_number_font,
        Drawer::SMALL,
        color,
        bg_color
    );
    smallDrawer.drawNumber( message, first_offset, baseline + _big_number_font.height());
    delete smallDrawer;
}
```

## Makefile
```makefile
    # google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR)
# OBJECTS=SetHistoryTextTest.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o
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

MAIN_OBJECTS=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o

MAIN_OBJECTS_MANUAL=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o

TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o Drawer.o CanvasCreator.o FontLoader.o GameTimer.o
SCORE_TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o  Mode1Score.o Mode1ScoreTest.o

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

tennis-game: $(MAIN_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

tennis-game-manual: GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o
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

Drawer.o : Drawer/Drawer.cpp
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