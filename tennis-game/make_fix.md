Please help me fix a C++ make error.

Here is the project structure:
```bash
adamsl@DESKTOP-SHDBATI:~$ tree -d -L 2 rpi-rgb-led-matrix/
rpi-rgb-led-matrix/
├── TieBreaker
├── adapter
│   ├── active-3
│   ├── kicad-scripts
│   ├── passive-3
│   └── passive-rpi1
├── bindings
│   ├── c#
│   └── python
├── examples-api-use
├── fonts
├── img
├── include
├── js-project
├── lib
│   └── lib_docs
├── linuxadmin
│   └── debug_rgb_lib_missing
├── logic_analyzer
├── tennis-game
│   ├── Arduino
│   ├── BatteryTest
│   ├── Blinker
│   ├── CanvasCreator
│   ├── ColorManager
│   ├── ConsoleDisplay
│   ├── Drawer
│   ├── FetchRunner
│   ├── FileReader
│   ├── FontLoader
│   ├── FontManager
│   ├── GameLedTranslator
│   ├── GameLeds
│   ├── GameModes
│   ├── GameObject
│   ├── GameState
│   ├── GameStateSerializer
│   ├── GameTimer
│   ├── GameWinSequence
│   ├── History
│   ├── IDisplay
│   ├── ITextDrawer
│   ├── InputWithTimer
│   ├── Inputs
│   ├── JsonParser
│   ├── LogObject
│   ├── LogObjectContainer
│   ├── LogObjectFactory
│   ├── Logger
│   ├── LoggerFactory
│   ├── MatchWinSequence
│   ├── MatrixDisplay
│   ├── MatrixTextDrawer
│   ├── Mode1Functions
│   ├── Mode1Score
│   ├── Mode2Functions
│   ├── Model
│   ├── MonitorLed
│   ├── MonitorLedClassObject
│   ├── MonitoredObject
│   ├── PairingBlinker
│   ├── PinInterface
│   ├── PinState
│   ├── Player
│   ├── PointLeds
│   ├── RemoteCodeTranslator
│   ├── RemotePairingScreen
│   ├── Reset
│   ├── ScoreBoard
│   ├── ScoreboardBlinker
│   ├── ServeLeds
│   ├── SetDrawer
│   ├── SetHistoryText
│   ├── SetLeds
│   ├── SetWin
│   ├── SourceData
│   ├── SubjectManager
│   ├── TennisConstants
│   ├── ThreadSafeQueue
│   ├── TieBreaker
│   ├── TieLeds
│   ├── TranslateConstant
│   ├── Undo
│   ├── WatchTimer
│   ├── WebLiquidCrystal
│   ├── WinSequence
│   ├── WinSequences
│   ├── __pycache__
│   ├── agency-swarm
│   ├── command_runner
│   ├── documentation
│   ├── fonts
│   ├── googletest
│   ├── nlohmann
│   ├── smart_menu
│   ├── update_notes
│   └── z_end_list_instruct
├── utils
└── z__build_tools

99 directories
adamsl@DESKTOP-SHDBATI:~$
```

Location of the Makefile:
```bash
adamsl@DESKTOP-SHDBATI:~$ ll rpi-rgb-led-matrix/tennis-game/Makefile
-rw-r--r-- 1 adamsl adamsl 10128 Dec 18 14:49 rpi-rgb-led-matrix/tennis-game/Makefile
adamsl@DESKTOP-SHDBATI:~$
```

Here is the Makefile:
```make
# google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CXX=g++ -std=c++17
CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR) -I$(RGB_INCDIR)

BINARIES=tennis-game Mode1ScoreTest

RGB_LIB_DISTRIBUTION=~/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -L$(GTEST_LIBDIR) $(GTEST_LIBS) -ljsoncpp -lcurl -lpthread

MAIN_OBJECTS=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o InputWithTimer.o ScoreboardBlinker.o RemotePairingScreen.o PairingBlinker.o FontManager.o ColorManager.o ConsoleDisplay.o RemoteCodeTranslator.o

TEST_OBJECTS=TieLeds.o GameLedTranslator.o GameState.o Player.o GameTimer.o ScoreBoard.o SetDrawer.o SetHistoryText.o Drawer.o CanvasCreator.o FontLoader.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o PinInterface.o PinState.o History.o Mode1Score.o Mode1ScoreTest.o TieBreakerTest.o TranslateConstant.o Logger.o TieBreaker.o PointLeds.o GameLeds.o SetLeds.o Mode1WinSequences.o Undo.o ServeLeds.o GameWinSequence.o SetWin.o MatchWinSequence.o Inputs.o WatchTimer.o Reset.o  InputWithTimer.o ScoreboardBlinker.o RemotePairingScreen.o PairingBlinker.o ConsoleDisplay.o FontManager.o ColorManager.o GameObject.o GameModes.o WebLiquidCrystal.o SubjectManager.o



all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

tennis-game: $(MAIN_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

tennis-game-manual: GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o RemotePairingScreen.o PairingBlinker.o  ConsoleDisplay.o FontManager.o ColorManager.o

	$(CXX) $^ -o $@ $(LDFLAGS)

FetchRunner.o : FetchRunner/FetchRunner.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

RemoteCodeTranslator.o : RemoteCodeTranslator/RemoteCodeTranslator.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SourceData.o : SourceData/SourceData.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

ColorManager.o : ColorManager/ColorManager.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

ConsoleDisplay.o: ConsoleDisplay/ConsoleDisplay.cpp ConsoleDisplay/ConsoleDisplay.h
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

FontManager.o : FontManager/FontManager.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Model.o : Model/Model.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MonitoredObject.o : MonitoredObject/MonitoredObject.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LoggerFactory.o : LoggerFactory/LoggerFactory.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

JsonParser.o : JsonParser/JsonParser.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MonitorLed.o : MonitorLed/MonitorLed.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

MonitorLedClassObject.o : MonitorLedClassObject/MonitorLedClassObject.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LogObjectFactory.o : LogObjectFactory/LogObjectFactory.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LogObjectContainer.o : LogObjectContainer/LogObjectContainer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

LogObject.o : LogObject/LogObject.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

test-manual: GameWinSequence.o SetWin.o
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<
                                                                                                                                                                                
TieBreakerTest.o: TieBreaker/TieBreakerTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1ScoreTest.o: Mode1Score/Mode1ScoreTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1ScoreTest: TieBreakerTest.o Player.o GameState.o PinInterface.o PinState.o TieBreaker.o TranslateConstant.o PointLeds.o GameLeds.o ServeLeds.o SetLeds.o Undo.o GameTimer.o SetWin.o Inputs.o WatchTimer.o ScoreBoard.o Mode1Functions.o Mode2Functions.o BatteryTest.o
	$(CXX) $(CXXFLAGS) -I../ -I../../lib -I../../lib/gtest/include -o $@ $^ -L../../lib/gtest -lgtest -lgtest_main -pthread -lrgbmatrix -lrt -lm -ljsoncpp -lcurl
	
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

GameTimer.o : GameTimer/GameTimer.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameState.o : GameState/GameState.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameObject.o : GameObject/GameObject.cpp
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

PairingBlinker.o : PairingBlinker/PairingBlinker.cpp Blinker/Blinker.h
	$(CXX) -I$(RGB_INCDIR) -I../Arduino $(CXXFLAGS) -c -o $@ $<

GameModes.o: GameModes/GameModes.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

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

RemotePairingScreen.o : RemotePairingScreen/RemotePairingScreen.cpp
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

WebLiquidCrystal.o: WebLiquidCrystal/WebLiquidCrystal.cpp
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

InputWithTimer.o : InputWithTimer/InputWithTimer.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

ScoreboardBlinker.o : ScoreboardBlinker/ScoreboardBlinker.cpp Blinker/Blinker.h
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SubjectManager.o: SubjectManager/SubjectManager.cpp
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

Here is the error when running `make`:
```bash
adamsl@DESKTOP-SHDBATI:~$ cd rpi-rgb-led-matrix/
adamsl@DESKTOP-SHDBATI:~/rpi-rgb-led-matrix$ cd tennis-game/
adamsl@DESKTOP-SHDBATI:~/rpi-rgb-led-matrix/tennis-game$ make
g++ -std=c++17 GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o InputWithTimer.o ScoreboardBlinker.o RemotePairingScreen.o PairingBlinker.o FontManager.o ColorManager.o ConsoleDisplay.o RemoteCodeTranslator.o -o tennis-game -L~/rpi-rgb-led-matrix/lib -lrgbmatrix -lrt -lm -L/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/build/lib -lgtest_main -lgtest -lpthread -ljsoncpp -lcurl -lpthread
/usr/bin/ld: cannot find -lrgbmatrix: No such file or directory
collect2: error: ld returned 1 exit status
make: *** [Makefile:35: tennis-game] Error 1
adamsl@DESKTOP-SHDBATI:~/rpi-rgb-led-matrix/tennis-game$
```

Please only show me the code that is needed to fix the error.
