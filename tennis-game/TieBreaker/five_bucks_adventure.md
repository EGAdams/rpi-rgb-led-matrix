You are an AI debugger trying to debug a make error for a user based on their C++ source files and the Makefile used to build the project. The user has provided you with the following files and their contents, finally followed by the output of the make command:

My files are as follows: 
# TieBreaker.cpp:
```cpp
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
    const int servePattern[] = {PLAYER_2_SERVE, PLAYER_2_SERVE, PLAYER_1_SERVE, PLAYER_1_SERVE};

    if (_iteration < 1) {
        std::cerr << "*** ERROR: _getServe() in TieBreaker.cpp is returning default value. ***"
                  << std::endl;
        std::cerr << "The iteration is: " << _iteration << std::endl;
        return PLAYER_1_SERVE; // Default to PLAYER_1_SERVE for invalid iterations
    }

    // Adjust the iteration for zero-based indexing and calculate serve index
    int serveIndex = (_iteration - 1) % 4;

    
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
```
# TieBreaker.h:
```cpp
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
        _tieBreaker = new TieBreaker( _player1, _player2, _pinInterface, _gameState, NULL );
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _pinInterface;
        delete _player2;
        delete _player1;
        delete _gameState;
    }
};


TEST_F(TieBreakerTest, Player1WinsTieBreaker) {
    _gameState->setTieBreak(1);
    _player1->setPoints(6);
    _player2->setPoints(6);
    _tieBreaker->run(_player1);  // Player 1 scores, now 7-6
    EXPECT_EQ(_player1->getPoints(), 7);
    EXPECT_EQ(_player2->getPoints(), 6);

    _tieBreaker->run(_player1);  // Player 1 scores again, now 8-6, wins the tie-break and set
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_player1->getGames(), 0);
    EXPECT_EQ(_player2->getGames(), 0);
    EXPECT_EQ(_player1->getSets(), 1);
}

TEST_F(TieBreakerTest, Player2WinsTieBreaker) {
    _gameState->setTieBreak(1);
    _player1->setPoints(6);
    _player2->setPoints(6);
    _tieBreaker->run(_player2);  // Player 2 scores, now 7-6
    EXPECT_EQ(_player1->getPoints(), 6);
    EXPECT_EQ(_player2->getPoints(), 7);

    _tieBreaker->run(_player2);  // Player 2 scores again, now 8-6, wins the tie-break and set
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_player1->getGames(), 0);
    EXPECT_EQ(_player2->getGames(), 0);
    EXPECT_EQ(_player2->getSets(), 1);
}
```
# Makefile:
## Makefile for TieBreaker tests
```
CXX = g++
CXXFLAGS = -std=c++14 -I../ -I../../lib -I../../lib/gtest/include
LDFLAGS = -L../../lib/gtest -lgtest -lgtest_main -pthread

SRCS = TieBreakerTest.cpp ../Player/Player.cpp ../GameState/GameState.cpp ../PinInterface/PinInterface.cpp ../PinState/PinState.cpp ../TieBreaker/TieBreaker.cpp ../TranslateConstant/TranslateConstant.cpp ../GameLeds/GameLeds.cpp ../ServeLeds/ServeLeds.cpp ../SetLeds/SetLeds.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = TieBreakerTest

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
```

Please help me debug this.