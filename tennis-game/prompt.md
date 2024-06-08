You are an AI debugger who is trying to debug a make error for a user based on their C++ source files and the Makefile used to build the project. The user has provided you with the following files and their contents, finally followed by the output of the make command:
My files are as follows: Mode1Score.cpp:
#include "Mode1Score.h"

Mode1Score::Mode1Score(
    Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState,
    History* history )
    : _player1( player1 ),
    _player2( player2 ),
    _gameState( gameState ),
    _history( history ),
    _tieBreaker( player1, player2, pinInterface, gameState, history ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _mode1WinSequences( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {
        std::cout << "creating mode 1 score logger..." << std::endl;
        _logger = new Logger( "Mode1Score" );
        std::cout << "created new logger object with name [" << _logger->getName() << "]" << std::endl;
    }
Mode1Score::~Mode1Score() {}

TieBreaker* Mode1Score::getTieBreaker() { return &_tieBreaker; }

void Mode1Score::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard(          scoreBoard );
    _gameLeds.setScoreBoard(           scoreBoard );
    _mode1WinSequences.setScoreBoards( scoreBoard );
    _setLeds.setScoreBoard(            scoreBoard );
    _tieBreaker.setScoreBoards(        scoreBoard );   // all day debug. in the future.
    _undo.setScoreBoard(               scoreBoard ); } // find a way to avoid this.

ScoreBoard* Mode1Score::getScoreBoard() { return _scoreBoard; }

void Mode1Score::_resetGame() {
     GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _gameState->setPlayer1Points( 0 );
    _gameState->setPlayer2Points( 0 );
    _gameState->setServeSwitch( 1 );
    // _gameState->setServe( 0 ); // set serve in game win only
    _pointLeds.updatePoints(); }

void Mode1Score::updateScore( Player* currentPlayer ) {
    std::cout << "setting logger name to updateScore... " << std::endl;
    _logger->setName( "updateScore" );
    std::cout << "done setting name to updateScore." << std::endl;
    if ( _gameState->getTieBreak() == 1 ) {           // Tie Break
        _logger->logUpdate( "tie break run..." );
        _tieBreaker.run( currentPlayer );
    } else if ( _gameState->getSetTieBreak() == 1 ) { // Set Tie Break
        _logger->logUpdate( "set tie breaker..." );
        _tieBreaker.setTieBreaker();
    } else {                                          // Regular Game
        Player* otherPlayer = currentPlayer->getOpponent();
        int current_player_points = currentPlayer->getPoints();
        int other_player_points   = otherPlayer->getPoints();
        if ( current_player_points >= 3 ) {
            _logger->logUpdate( "player " + std::to_string( currentPlayer->number()) + " has 3 points or more." );
            if ( current_player_points == other_player_points ) {
                _logger->logUpdate( "player " + std::to_string( currentPlayer->number()) + " has 3 points and tied with " + std::to_string( otherPlayer->number()) + "." );
                currentPlayer->setPoints( 3 );
                otherPlayer->setPoints(   3 );
            } else if ( current_player_points > 3
                && ( current_player_points - other_player_points ) > 1 ) {
                _logger->logUpdate( "player " + std::to_string( currentPlayer->number()) + " has 3 points and won by " + std::to_string( current_player_points - other_player_points ) + "." );
                currentPlayer->setGames( currentPlayer->getGames() + 1 );
                _undo.memory();
                currentPlayer->number() == 0 ? playerOneGameWin() : playerTwoGameWin(); }

            if ( currentPlayer->getPoints() == 4 ) {
                _logger->logUpdate( "player " + std::to_string( currentPlayer->number()) + " has 4 points." );
                _gameState->setPointFlash( 1 );       // "Ad" mode
                _gameState->setPreviousTime( GameTimer::gameMillis());
                _gameState->setToggle( 0 );
            }
        }
        _pointLeds.updatePoints();
    }
}

void Mode1Score::playerOneScore() { updateScore( _player1 );}
void Mode1Score::playerTwoScore() { updateScore( _player2 );}


//////////////////////////////// GAME WIN SCENARIOS ///////////////////////////
void Mode1Score::playerGameWin( Player* player ) {
    Player* opponent = player->getOpponent();
    std::cout << "player " << std::to_string( player->number()) << " games: " << player->getGames() << std::endl;
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    if ( player->getGames() >= GAMES_TO_WIN_SET ) {
        if ( player->getGames() == GAMES_TO_WIN_SET && opponent->getGames() == GAMES_TO_WIN_SET ) {
            _gameState->setTieBreak( 1 );
            _tieBreaker.initializeTieBreakMode();
        }
        if (_gameState->getTieBreak() == 0) {  // if this is not a tie break game...
            if ((player->getGames() - opponent->getGames()) > 1 ) {  // player ahead by 2 games.
                player->setSets(_gameState, player->getSets() + 1 ); // Set win
                _setLeds.updateSets(); // sets the tiebreak, wins
                                       // the match, or just wins the set.
                if (player->getSets() == opponent->getSets()) {  // set tie break
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1TBSetWinSequence() : _mode1WinSequences.p2TBSetWinSequence();
                    _gameState->setSetTieBreak( 1 );
                    _tieBreaker.setTieBreakEnable();
                } else if (player->getSets() == SETS_TO_WIN_MATCH) {  // match win, done playing
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.playerOneMatchWin() : _mode1WinSequences.playerTwoMatchWin();
                    _gameState->stopGameRunning();
                } else {                                              // regular set win, then reset
                    _gameState->setPlayer1SetHistory(player->getSetHistory());
                    _gameState->setPlayer2SetHistory(opponent->getSetHistory());
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1SetWinSequence() : _mode1WinSequences.p2SetWinSequence();
                    _gameState->setCurrentSet(_gameState->getCurrentSet() + 1 );
                    _setLeds.updateSets();
                    GameTimer::gameDelay(_gameState->getWinDelay());
                    _resetGame();
                }
                player->setGames( 0 );
                opponent->setGames( 0 );
            } else {                        // player is ahead by 1 game, but not enough to win the set.
                player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1GameWinSequence() : _mode1WinSequences.p2GameWinSequence();
                _gameLeds.updateGames();
                _gameState->setPlayer1SetHistory(player->getSetHistory());
                _gameState->setPlayer2SetHistory(opponent->getSetHistory());
                _resetGame();
            }
        }
    } else { // this is a regualar game win...
        if ( player->number() == PLAYER_1_INITIALIZED ) {
            _mode1WinSequences.p1GameWinSequence();
            _gameState->setPlayer1SetHistory(player->getSetHistory());
            _gameState->setPlayer2SetHistory(opponent->getSetHistory());
        } else {
            _mode1WinSequences.p2GameWinSequence();
            _gameState->setPlayer2SetHistory(player->getSetHistory());
            _gameState->setPlayer1SetHistory(opponent->getSetHistory());
        }
        _gameLeds.updateGames();
        _resetGame();
    }
}
void Mode1Score::playerOneGameWin() { playerGameWin( _player1 ); }
void Mode1Score::playerTwoGameWin() { playerGameWin( _player2 ); }
//////////////////// END GAME WIN SCENARIOS ///////////////////////////////////

void Mode1Score::mode1TBP1Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 15 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );

        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p1TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }}
    if ( _player1->getGames() >= 10 &&
        ( _player1->getGames() - _player2->getGames()) > 1 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p1TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }}}

void Mode1Score::mode1SetTBP1Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 7 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        _setLeds.updateSets();
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p1SetTBWinSequence();
        _tieBreaker.tieLEDsOff();
        _mode1WinSequences.playerOneMatchWin();
        _gameState->stopGameRunning(); }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 ); }

void Mode1Score::mode1TBP2Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 15 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        } else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak(); }}
    if ( _player2->getGames() >= 10 &&
        ( _player2->getGames() - _player1->getGames()) > 1 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        } else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());  // set set history, set++
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak(); }}}

void Mode1Score::mode1SetTBP2Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 7 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        _setLeds.updateSets();
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p2SetTBWinSequence();
        _tieBreaker.tieLEDsOff();
        _mode1WinSequences.playerTwoMatchWin();
        _gameState->stopGameRunning();
    }
    std::cout << "setting serve switch to: " << _gameState->getServeSwitch() + 1 << std::endl;
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}

Mode1Score.h:
#ifndef MODE_1_SCORE_H
#define MODE_1_SCORE_H

#include "../Arduino/Arduino.h"
#include "../GameLeds/GameLeds.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../TieBreaker/TieBreaker.h"
#include "../WinSequences/WinSequences.h"
#include "../PointLeds/PointLeds.h"
#include "../PinState/PinState.h"
#include "../PinInterface/PinInterface.h"
#include "../Player/Player.h"
#include "../SetLeds/SetLeds.h"
#include "../Undo/Undo.h"
#include "../TennisConstants/TennisConstants.h"
#include "../Logger/Logger.h"
#include <iostream>
#include "../../tennis-game/googletest/googletest/include/gtest/gtest.h"

class Mode1Score {
 public:
    Mode1Score( Player* player1,
                Player* player2,
                PinInterface* pinInterface,
                GameState* gameState,
                History* history );
    ~Mode1Score();
    void playerOneScore();
    void playerOneGameWin();
    void mode1TBP1Games();
    void mode1SetTBP1Games();
    void playerTwoScore();
    void playerTwoGameWin();
    void playerGameWin( Player* currentPlayer );
    void mode1TBP2Games();
    void mode1SetTBP2Games();
    void updateScore(  Player* currentPlayer  );
    void setScoreBoard( ScoreBoard* scoreBoard );
    ScoreBoard* getScoreBoard();
    TieBreaker* getTieBreaker();

 private:
    void _resetGame();
    Player* _player1;
    Player* _player2;
    GameState* _gameState;
    History* _history;
    TieBreaker _tieBreaker;
    PointLeds _pointLeds;
    GameLeds _gameLeds;
    SetLeds _setLeds;
    Mode1WinSequences _mode1WinSequences;
    Undo _undo;
    ScoreBoard* _scoreBoard;
    Logger* _logger; };
#endif

Mode1ScoreTest.cpp:
#include <gtest/gtest.h>
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


TEST(BasicTest, TestWorks) {
    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


Makefile: # google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CXX=g++
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

SCORE_TEST_OBJECTS=GameState.o Player.o GameState.o GameTimer.o ScoreBoard.o SetDrawer.o Mode1Score.o tests/Mode1ScoreTest.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o PinInterface.o PinState.o History.o

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
	$(CXX) $(CXXFLAGS) -o Mode1ScoreTest $(SCORE_TEST_OBJECTS) $(LDFLAGS)

tests/Mode1ScoreTest.o: tests/Mode1ScoreTest.cpp
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


My issue is as follows: Please fix this make error ```make output: make: *** No rule to make target 'tests/Mode1ScoreTest.cpp', needed by 'tests/Mode1ScoreTest.o'.  Stop.
```

Give me ideas for what could be wrong and what fixes to do in which files.