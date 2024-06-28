You are an AI debugger who is trying to debug a make error for a user based on their C++ source files and the Makefile used to build the project. The user has provided you with the following files and their contents, finally followed by the output of the make command:

# Source Files
```cpp
TieBreaker.h:
#ifndef TieBreaker_h
#define TieBreaker_h
#include "../Arduino/Arduino.h"
#include "../GameLeds/GameLeds.h"
#include "../GameTimer/GameTimer.h"
#include "../PointLeds/PointLeds.h"
#include "../PinInterface/PinInterface.h"
#include "../Player/Player.h"
#include "../ServeLeds/ServeLeds.h"
#include "../SetLeds/SetLeds.h"
#include "../WatchTimer/WatchTimer.h"
#include "../WinSequences/WinSequences.h"
#include "../Undo/Undo.h"
#include "../Inputs/Inputs.h"
#include "../ScoreBoard/ScoreBoard.h"

class TieBreaker {
 public:
  TieBreaker(Player* player1, Player* player2, PinInterface* pinInterface, GameState* gameState, History* history);
  ~TieBreaker();
  void setIteration(int iteration);
  int getIteration();
  void incrementIteration();
  void setScoreBoards(ScoreBoard* scoreBoard);
  void tieLEDsOn();
  void tieLEDsOff();
  void celebrate(Player* currentPlayer);
  void incrementSet();
  void run(Player* currentPlayer);
  void mode1SetTBButtonFunction();
  void setTieBreaker();
  void initializeTieBreakMode();
  void setTieBreakEnable();
  void endTieBreak();
  void mode1TBP1Games();
  void mode1TBP2Games();
  void mode1SetTBP2Games();
  void mode1SetTBP1Games();

 private:
  int  _getServe();    // determine serve based on iteration
  Player* _player1;
  Player* _player2;
  PinInterface* _pinInterface;
  GameState* _gameState;
  History* _history;
  PointLeds _pointLeds;
  GameLeds _gameLeds;
  ServeLeds _serveLeds;
  SetLeds _setLeds;
  Mode1WinSequences _mode1WinSequences;
  Undo _undo;
  ScoreBoard* _scoreBoard;
  int _iteration;
};

#endif

TieBreaker.cpp:
#include "TieBreaker.h"

TieBreaker::TieBreaker( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState,
    History* history )
    : _player1( player1 ),
    _player2( player2 ),
    _pinInterface( pinInterface ),
    _gameState( gameState ),
    _history( history ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _serveLeds( pinInterface, gameState ),
    _setLeds( player1, player2, pinInterface ),
    _mode1WinSequences( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {}

TieBreaker::~TieBreaker() {}

int TieBreaker::_getServe() { // bot code, beware...
    // Array representing serve sequence in pairs
    const int servePattern[] = { PLAYER_2_SERVE, PLAYER_2_SERVE, PLAYER_1_SERVE, PLAYER_1_SERVE };

    if (_iteration < 1) {
        std::cerr << "*** ERROR: _getServe() in TieBreaker.cpp is returning default value. ***"
                  << std::endl;
        std::cerr << "The iteration is: " << _iteration << std::endl;
        return PLAYER_1_SERVE; // Default to PLAYER_1_SERVE for invalid iterations
    }

    // Adjust the iteration for zero-based indexing and calculate serve index
    int serveIndex = ( _iteration - 1 ) % 4;

    
    return servePattern[serveIndex];
}

void TieBreaker::setIteration( int iteration ) { _iteration = iteration; }
int  TieBreaker::getIteration() { return _iteration; }
void TieBreaker::incrementIteration() { 
    // std::cout << "Incrementing iteration in TieBreaker::incrementIteration()... " << std::endl;
    _iteration++; }

void TieBreaker::setScoreBoards( ScoreBoard* scoreBoard ) {
    _pointLeds.setScoreBoard( scoreBoard );
    _setLeds.setScoreBoard(   scoreBoard ); 
    _gameLeds.setScoreBoard(  scoreBoard ); 
    _undo.setScoreBoard(      scoreBoard ); 
    _scoreBoard =             scoreBoard; 
}

void TieBreaker::tieLEDsOn() {
    _gameState->setTieLEDsOn( 1 );
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH ); }

void TieBreaker::tieLEDsOff() {
    _gameState->setTieLEDsOn( 0 );
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW ); }

void TieBreaker::celebrate( Player* currentPlayer) {
    std::cout << "*** celebrateWin() called. ***" << std::endl;
    GameTimer::gameDelay( _gameState->getWinDelay() );
    SetWin setWin( &_undo, _gameState, &_setLeds );
    setWin.execute( currentPlayer, _scoreBoard );
    std::cout << "*** done celebrating. *** " << std::endl;
}

void TieBreaker::incrementSet() {
    _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 ); // increment set
}

void TieBreaker::run( Player* currentPlayer ) { 
    _undo.memory();
    std::cout << "Current Player Address: " << currentPlayer << std::endl;
    if (currentPlayer == nullptr) {
        std::cerr << "*** ERROR: Current player is null in TieBreaker::run(). ***" << std::endl;
        exit(1);
    }
    Player* opponent = currentPlayer->getOpponent();
    std::cout << "Opponent Address: " << opponent << std::endl;
    if (opponent == nullptr) {
        std::cerr << "*** ERROR: Opponent is null in TieBreaker::run(). ***" << std::endl;
        exit(1);
    }
    std::cout << "Current Player Points: " << currentPlayer->getPoints() << std::endl;
    std::cout << "Opponent Points: " << opponent->getPoints() << std::endl;
    std::cout << "TieBreaker iteration before setting serve: " << _iteration << std::endl;
    int serve = _getServe();
    std::cout << "Serve value from _getServe(): " << serve << std::endl;
    if (serve != PLAYER_1_SERVE && serve != PLAYER_2_SERVE) {
        std::cerr << "*** ERROR: Invalid serve value in TieBreaker::run(). ***" << std::endl;
        std::cerr << "Serve value: " << serve << std::endl;
        exit(1);
    }
    _gameState->setServe(serve); // set the serve bar depending tie-break iteration
    if (_scoreBoard == nullptr) {
        std::cerr << "*** ERROR: ScoreBoard is null in TieBreaker::run(). ***" << std::endl;
        exit(1);
    }
    std::cout << "Updating ScoreBoard..." << std::endl;
    try {
        _scoreBoard->update();
        std::cout << "ScoreBoard updated." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "*** EXCEPTION: " << e.what() << " ***" << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "*** UNKNOWN EXCEPTION in ScoreBoard update ***" << std::endl;
        exit(1);
    }

    std::cout << "TieBreaker iteration after setting serve: " << _iteration << std::endl;

    std::cout << "Current Player Address: " << currentPlayer << std::endl;
    std::cout << "Current Player Points: " << currentPlayer->getPoints() << std::endl;
    std::cout << "Opponent Address: " << opponent << std::endl;
    std::cout << "Opponent Points: " << opponent->getPoints() << std::endl;

    if (currentPlayer->getPoints() == TIE_BREAK_MAX_POINTS) {
        std::cout << "Current Player has reached TIE_BREAK_MAX_POINTS." << std::endl;
        _undo.snapshot(_history);
        currentPlayer->setGames(currentPlayer->getGames() + 1); // increment games
        incrementSet();
        if (_scoreBoard != nullptr) {
            _scoreBoard->update();
        }
        celebrate(currentPlayer); // this is a win no matter what.
        GameTimer::gameDelay(3000);
        endTieBreak();
    } else if (currentPlayer->getPoints() >= TIE_BREAK_WIN_BY_TWO &&
               (currentPlayer->getPoints() - opponent->getPoints() >= 2)) {
        std::cout << "Current Player has won by two points." << std::endl;
        _undo.snapshot(_history);
        currentPlayer->setGames(currentPlayer->getGames() + 1); // increment games
        incrementSet();
        if (_scoreBoard != nullptr) {
            _scoreBoard->update();
        }
        celebrate(currentPlayer);
        GameTimer::gameDelay(3000);
        endTieBreak();
    } else {
        std::cout << "Incrementing iteration for next serve." << std::endl;
        incrementIteration(); // need this to determine serve bar location
    }
}

void TieBreaker::mode1SetTBButtonFunction() {
    switch ( _gameState->getPlayerButton()) {
    case 0:
        break;

    case 1:
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.snapshot( _history );
        _player1->setGames( _player1->getGames() + 1 );
        mode1SetTBP1Games();
        break;

    case 2:
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );
        break;

    case 3:
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.snapshot( _history );
        _player2->setGames( _player2->getGames() + 1 );
        mode1SetTBP2Games();
        break;

    case 4:
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );
        break;
    }
    _gameState->setPlayerButton( 0 );
}

void TieBreaker::setTieBreaker() { 
    _undo.memory(); 
    mode1SetTBButtonFunction(); }

void TieBreaker::initializeTieBreakMode() {
    std::cout << "*** initializeTieBreakMode() called. ***" << std::endl;
    _iteration = 1;  // this is initialized to zero before, so it could be checked as another flag
    std::cout << "TieBreaker iteration initialized to: " << _iteration << std::endl;
    std::cout << "TieBreaker iteration initialized to: " << _iteration << std::endl;
                     // _iteration is used to determine which serve bar to light up
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    std::cout << "*** calling _pointLeds.updatePoints() from inside initializeTieBreakMode()... ***" << std::endl;
    _pointLeds.updatePoints();
    std::cout << "*** after update points in tie breaker!!! *** " << std::endl;
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    _serveLeds.serveSwitch();
    if ( _gameState->getTieLEDsOn() == 0 ) { tieLEDsOn(); }
    if( _player1->getGames() != 6 ) {
        std::cout << "*** ERROR: player1 games is not 6 in initializeTieBreakMode() ***  exiting... " << std::endl;
        exit( 1 ); }
    if( _player1->getGames() != 6 ) {
        std::cout << "*** ERROR: player1 games is not 6 in initializeTieBreakMode() ***  exiting... " << std::endl;
        exit( 1 ); }
    _gameLeds.updateGames();
    Inputs _inputs( _player1, _player2, _pinInterface, _gameState );
    WatchTimer _watchTimer;
    for ( int currentPulseCount = 0; currentPulseCount < TIE_PULSE_COUNT; currentPulseCount++ ) {
        tieLEDsOff();
        if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }
        tieLEDsOn();
        if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }}
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    tieLEDsOn(); } // not coming on?

void TieBreaker::setTieBreakEnable() {
    _player1->setPoints( 4 );
    _player2->setPoints( 4 );
    std::cout << "*** calling _pointLeds.updatePoints() from inside setTieBreakEnable()... ***" << std::endl;
    _pointLeds.updatePoints();
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    _serveLeds.serveSwitch();
    Inputs _inputs( _player1, _player2, _pinInterface, _gameState );
    WatchTimer _watchTimer;
    for ( int currentPulseCount = 0; currentPulseCount < TIE_PULSE_COUNT; currentPulseCount++ ) {
        tieLEDsOff();
        if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }
        tieLEDsOn();
        if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }}
    if ( _gameState->getTieLEDsOn() == 0 ) { tieLEDsOn(); }
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY  ); }

void TieBreaker::endTieBreak() {
    tieLEDsOff();
    _iteration = 0;
    std::cout << "TieBreaker iteration reset to: " << _iteration << std::endl;
    std::cout << "TieBreaker iteration reset to: " << _iteration << std::endl;
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _player1->setGames(  0 );
    _player2->setGames(  0 );
    std::cout << "*** calling _pointLeds.updatePoints() from inside endTieBreak()... ***" << std::endl;
    _pointLeds.updatePoints();
    _gameLeds.updateGames();
    _gameState->setTieBreak(    0 );
    _gameState->setSetTieBreak( 0 );
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    std::cout << "Before updating ScoreBoard in TieBreaker::run()." << std::endl;
    if (_scoreBoard == nullptr) {
        std::cerr << "*** ERROR: ScoreBoard is null in TieBreaker::run() before update. ***" << std::endl;
        exit(1);
    }
    try {
        _scoreBoard->update();
        std::cout << "ScoreBoard updated successfully in TieBreaker::run()." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "*** EXCEPTION: " << e.what() << " ***" << std::endl;
        exit(1);
    } catch (...) {
        std::cerr << "*** UNKNOWN EXCEPTION in ScoreBoard update ***" << std::endl;
        exit(1);
    }
    std::cout << "After updating ScoreBoard in TieBreaker::run()." << std::endl;
}

void TieBreaker::mode1TBP1Games() {
    _gameLeds.updateGames();  // UpdateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );

    if ( _player1->getGames() == 15 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );;

        if ( _player2->getSets() == _player1->getSets()) {
            endTieBreak();
            _mode1WinSequences.p1TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );          
            setTieBreakEnable();                     
        } else {
            _mode1WinSequences.p1SetWinSequence();  
            endTieBreak(); }}
    if ( _player1->getGames() >= 10 && ( _player1->getGames() - _player2->getGames() ) > 1 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets()) {
            endTieBreak();                         
            _mode1WinSequences.p1TBSetWinSequence(); 
            _gameState->setSetTieBreak( 1 );           
            setTieBreakEnable();                    
        } else {
            _mode1WinSequences.p1SetWinSequence();  
            endTieBreak(); }}}

void TieBreaker::mode1TBP2Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );

    if ( _player2->getGames() == 15 ) {
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames());
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames());
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            endTieBreak();                        
            _mode1WinSequences.p2TBSetWinSequence(); 
            _gameState->setSetTieBreak( 1 );
            setTieBreakEnable();
        } else {
            _mode1WinSequences.p2SetWinSequence();
            endTieBreak(); }}
    if ( _player2->getGames() >= 10 && ( _player2->getGames() - _player1->getGames() ) > 1 ) {
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames());
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames());
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            endTieBreak();
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            setTieBreakEnable();
        } else {
            _mode1WinSequences.p2SetWinSequence();
            endTieBreak();
        }}}

void TieBreaker::mode1SetTBP2Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 7 ) {
        std::cout << "inside mode1SetTBP2Games()...  player 2 games  is player 2... 7" << std::endl;
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames());
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames());
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        _scoreBoard->update();
        _mode1WinSequences.p2SetTBWinSequence();
        tieLEDsOff();
        _mode1WinSequences.playerTwoMatchWin(); 
        _gameState->stopGameRunning(); }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 ); }

void TieBreaker::mode1SetTBP1Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 7 ) {
        std::cout << "inside mode1SetTBP1Games()...  player 1 games  is player 1... 7" << std::endl;
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames());
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames());
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        _scoreBoard->update();
        _mode1WinSequences.p1SetTBWinSequence();
        tieLEDsOff();
        _mode1WinSequences.playerOneMatchWin(); 
        _gameState->stopGameRunning(); }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 ); }

TieBreakerTest.cpp:
#include <gtest/gtest.h>
#include "TieBreaker.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"

class TieBreakerTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    GameState* _gameState;
    PinInterface* _pinInterface;
    TieBreaker* _tieBreaker;

    void SetUp() override {
        _gameState = new GameState();
        _player1 = new Player(_gameState, PLAYER_1_INITIALIZED);
        _player2 = new Player(_gameState, PLAYER_2_INITIALIZED);
        _player1->setOpponent(_player2);
        _player2->setOpponent(_player1);
        std::map<std::string, int> pin_map;
        PinState* pin_state = new PinState(pin_map);
        _pinInterface = new PinInterface(pin_state);
        _tieBreaker = new TieBreaker(_player1, _player2, _pinInterface, _gameState, NULL);
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _pinInterface;
        delete _player2;
        delete _player1;
        delete _gameState;
    }
};

TEST_F(TieBreakerTest, SetUpSetTieBreakScenario) {
    _gameObject->getScoreBoard()->clearScreen();
    _player1->setSetHistory(1, 6);
    _player2->setSetHistory(1, 4);
    _player1->setSetHistory(2, 5);
    _player2->setSetHistory(2, 5);
    _gameState->setCurrentSet(2);
    _player1->setGames(5);
    _player2->setGames(5);
    _player1->setPoints(2);
    _player2->setPoints(3);
    
    std::cout << "Updating scoreboard..." << std::endl;
    _gameObject->getScoreBoard()->update();
    std::cout << "Done updating scoreboard." << std::endl;

    // Simulate Player 2 winning the next point
    _player2->setPoints(4);
    std::cout << "Player 2 scores the next point..." << std::endl;
    
    // Check if Player 2 wins the set and if the tie-breaker is ready
    if (_player2->getPoints() >= 4 && (_player2->getPoints() - _player1->getPoints()) >= 2) {
        _player2->setGames(_player2->getGames() + 1);
        EXPECT_EQ(_player2->getGames(), 6);  // Player 2 should now have 6 games
    }

    // Update and check the tie-breaker initiation
    _tieBreaker->initializeTieBreakMode();
    EXPECT_EQ(_tieBreaker->getIteration(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_gameState->getServeSwitch(), 1);
    EXPECT_EQ(_gameState->getServe(), 0);
    EXPECT_EQ(_gameState->getTieLEDsOn(), 1);
    std::cout << "Tie-breaker mode initialized." << std::endl;
}

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "../Arduino/Arduino.h"
#include "../GameModes/GameModes.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../History/History.h"
#include "../Inputs/Inputs.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"
#include "../Player/Player.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../SubjectManager/SubjectManager.h"
#include "../WebLiquidCrystal/WebLiquidCrystal.h"
#include "../../include/led-matrix.h"
#include "../../include/graphics.h"
#include "../TennisConstants/TennisConstants.h"
#include "../Logger/Logger.h"
#include <map>

#define GAME_LOOP_DELAY 100
class GameObject {
 public:
  GameObject( Player*       player1,
              Player*       player2,
              PinState*     pinState,
              PinInterface* pinInterface,
              GameState*    gameState,
              GameTimer*    gameTimer,
              Inputs*       gameInputs,
              GameModes*    gameModes,
              ScoreBoard*   scoreBoard,
              WebLiquidCrystal* lcd );

  GameObject( GameState* gameState );
  ~GameObject();
  void start();
  PinInterface* getPinInterface();
  void loopGame();
  void playerScore( int playerNumber );
  static void _signalHandler( int signal );
  static volatile int gSignalStatus;
  ScoreBoard* getScoreBoard();
  void setScoreBoard( ScoreBoard* scoreBoard );
  Player* getPlayer1();
  Player* getPlayer2();
  GameState* getGameState();
  Undo* createUndo();
  History* getHistory();
  void undo();

 private:
  Player*           _player1;
  Player*           _player2;
  PinState*         _pinState;
  PinInterface*     _pinInterface;
  GameState*        _gameState;
  GameTimer*        _gameTimer;
  Inputs*           _gameInputs;
  GameModes*        _gameModes;
  ScoreBoard*       _scoreBoard;
  WebLiquidCrystal* _webLiquidCrystal;
  SubjectManager*   _subjectManager;
  Logger*           _logger;
  History*          _history;
  std::map<std::string, int> _pin_map; };
#endif

```

# Makefile Source
```bash
# google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CXX=g++
CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR) -I$(RGB_INCDIR)

RGB_LIB_DISTRIBUTION=../../
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -L$(GTEST_LIBDIR) $(GTEST_LIBS) -ljsoncpp -lcurl -lpthread

TEST_OBJECTS=../TieLeds/TieLeds.o ../GameLedTranslator/GameLedTranslator.o ../GameState/GameState.o ../Player/Player.o ../GameTimer/GameTimer.o ../ScoreBoard/ScoreBoard.o ../SetDrawer/SetDrawer.o ../SetHistoryText/SetHistoryText.o ../Drawer/Drawer.o ../CanvasCreator/CanvasCreator.o ../FontLoader/FontLoader.o ../LogObject/LogObject.o ../LogObjectContainer/LogObjectContainer.o ../LogObjectFactory/LogObjectFactory.o ../MonitorLedClassObject/MonitorLedClassObject.o ../MonitorLed/MonitorLed.o ../JsonParser/JsonParser.o ../LoggerFactory/LoggerFactory.o ../Model/Model.o ../MonitoredObject/MonitoredObject.o ../SourceData/SourceData.o ../FetchRunner/FetchRunner.o ../PinInterface/PinInterface.o ../PinState/PinState.o ../History/History.o ../Mode1Score/Mode1Score.o TieBreakerTest.o ../TranslateConstant/TranslateConstant.o ../Logger/Logger.o TieBreaker.o ../PointLeds/PointLeds.o ../GameLeds/GameLeds.o ../SetLeds/SetLeds.o ../WinSequences/WinSequences.o ../Undo/Undo.o ../ServeLeds/ServeLeds.o ../GameWinSequence/GameWinSequence.o ../SetWin/SetWin.o ../MatchWinSequence/MatchWinSequence.o ../Inputs/Inputs.o ../WatchTimer/WatchTimer.o ../Reset/Reset.o

all: TieBreakerTest

TieBreakerTest: $(TEST_OBJECTS)
        $(CXX) $(CXXFLAGS) -o TieBreakerTest $(TEST_OBJECTS) $(LDFLAGS)

TieBreakerTest.o: TieBreakerTest.cpp
        $(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TieBreaker.o: TieBreaker.cpp
        $(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
        rm -f $(TEST_OBJECTS) TieBreakerTest

FORCE:
.PHONY: FORCE

print-%:
        @echo $* = $($*)

```
 
# Make Output
```bash
g++ -I../..//include -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -I../..//include -c -o TieBreakerTest.o TieBreakerTest.cpp
TieBreakerTest.cpp: In member function ‘virtual void TieBreakerTest_SetUpSetTieBreakScenario_Test::TestBody()’:
TieBreakerTest.cpp:38:5: error: ‘_gameObject’ was not declared in this scope
   38 |     _gameObject->getScoreBoard()->clearScreen();
      |     ^~~~~~~~~~~
make: *** [Makefile:29: TieBreakerTest.o] Error 1
```

Please help me debug this make error.
