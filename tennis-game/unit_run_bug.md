Please find out why this google unit test does not run any tests.  It just reports zero tests.

```makefile
# google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR)
# OBJECTS=SetHistoryTextTest.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o LoggerFactory.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o Model.o MonitoredObject.o SourceData.o FetchRunner.o
BINARIES=tennis-game Mode1ScoreTest

# Where our library resides. It is assumed here that $(RGB_LIB_DISTRIBUTION) has a
# compiled library in lib/
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread -L$(GTEST_LIBDIR) $(GTEST_LIBS) -ljsoncpp -lcurl

# To compile image-example
MAGICK_CXXFLAGS?=$(shell GraphicsMagick++-config --cppflags --cxxflags)
MAGICK_LDFLAGS?=$(shell GraphicsMagick++-config --ldflags --libs)

MAIN_OBJECTS=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o

MAIN_OBJECTS_MANUAL=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o, LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o

TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o Drawer.o CanvasCreator.o FontLoader.o GameTimer.o, LogObject.o LogObjectContainer.o LogObjectFactory.o
SCORE_TEST_OBJECTS=Mode1ScoreTest.o GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o Mode1Score.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

tennis-game: $(MAIN_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

tennis-game-manual: GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game-manual.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o
	$(CXX) $^ -o $@ $(LDFLAGS)

FetchRunner.o : FetchRunner/FetchRunner.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

SourceData.o : SourceData/SourceData.cpp
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
	@echo "Temporary rule worked."

Mode1ScoreTest: $(SCORE_TEST_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)
	./Mode1ScoreTest

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

Here is the C++ source code for the Google unit tests:
```cpp
#include "Mode1Score.h"
class Mode1ScoreTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    PinInterface* _pinInterface;
    GameState* _gameState;
    History* _history;
    Mode1Score* _mode1Score;

    void SetUp() override;
    void TearDown() override;
};

void Mode1ScoreTest::SetUp() {
    _gameState = new GameState();
    _player1 = new Player( _gameState, PLAYER_1_INITIALIZED );
    _player2 = new Player( _gameState, PLAYER_2_INITIALIZED );
    _player1->setOpponent( _player2 ); _player2->setOpponent( _player1 );
    std::map<std::string, int> pin_map;
    PinState* pin_state = new PinState( pin_map );
    _pinInterface = new PinInterface( pin_state );
    _history = new History();
    _mode1Score = new Mode1Score( _player1, _player2, _pinInterface, _gameState, _history );
}

// Tear down the test fixture
void Mode1ScoreTest::TearDown() {
    delete _mode1Score;
    delete _history;
    delete _gameState;
    delete _pinInterface;
    delete _player2;
    delete _player1;
}


///////////////// Test case: Test Mode 1 P1 Score Less Than 3 Points //////////
TEST_F( Mode1ScoreTest, TestMode1P1Score_LessThan3Points ) {
    _player1->setPoints( 2 );
    _player2->setPoints( 1 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 2);
    EXPECT_EQ( _player2->getPoints(), 1 );
    // Check other changes made by the method
}
///////////////////////////////////////////////////////////////////////////////


// Test case: Mode1P1ScoreTest
TEST_F( Mode1ScoreTest, Mode1P1ScoreTest) {
    // Test implementation here
}

// Test case: TestMode1P1Score_LessThan3Points
TEST_F( Mode1ScoreTest, TestMode1P1Score_LessThan3Points ) {
    _player1->setPoints( 2 );
    _player2->setPoints( 1 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 2);
    EXPECT_EQ( _player2->getPoints(), 1 );
    // Check other changes made by the method
}

// Test case: TestMode1P1Score_3Points_LessThan3PointsP2
TEST_F( Mode1ScoreTest, TestMode1P1Score_3Points_LessThan3PointsP2) {
    _player1->setPoints(3);
    _player2->setPoints( 2 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 3);
    EXPECT_EQ( _player2->getPoints(), 2);
    // Check other changes made by the method
}

// Test case: TestMode1P1Score_3Points_EqualPoints
TEST_F( Mode1ScoreTest, TestMode1P1Score_3Points_EqualPoints ) {
    _player1->setPoints(3);
    _player2->setPoints(3);
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 3);
    EXPECT_EQ( _player2->getPoints(), 3);
    // Check other changes made by the method
}

// Test case: TestMode1P1Score_MoreThan3Points_DifferenceMoreThan1
TEST_F( Mode1ScoreTest, TestMode1P1Score_MoreThan3Points_DifferenceMoreThan1) {
    _player1->setPoints(5);
    _player2->setPoints(3);
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 5);
    EXPECT_EQ( _player2->getPoints(), 3);
    EXPECT_EQ( _player1->getGames(), 1 ); // Assuming getGames starts from 0
    // Check other changes made by the method
}

// Test case: TestMode1P1Score_4Points
TEST_F( Mode1ScoreTest, TestMode1P1Score_4Points ) {
    _player1->setPoints(4);
    _player2->setPoints( 2 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 4);
    EXPECT_EQ( _player2->getPoints(), 2);
    EXPECT_EQ( _gameState->getPointFlash(), 1 ); // Assuming getPointFlash returns the current pointFlash
    // Check other changes made by the method
}
```

Answer in 25 words or less please.
