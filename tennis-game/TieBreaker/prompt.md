Please read the following .cpp test files that I need a makefile for:
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

Here is the Makefile one directory above that works:
```
# google test
GTEST_DIR=/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest
GTEST_INCDIR=$(GTEST_DIR)/googletest/include
GTEST_LIBDIR=$(GTEST_DIR)/build/lib

#-lgtest_main -lgtest -lpthread <-- this is the order that works
GTEST_LIBS=-lgtest_main -lgtest -lpthread

CXX=g++
CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS) -I$(GTEST_INCDIR) -I$(RGB_INCDIR)

BINARIES=tennis-game Mode1ScoreTest

RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -L$(GTEST_LIBDIR) $(GTEST_LIBS) -ljsoncpp -lcurl -lpthread

MAIN_OBJECTS=GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o Drawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o

TEST_OBJECTS=TieLeds.o GameLedTranslator.o GameState.o Player.o GameTimer.o ScoreBoard.o SetDrawer.o SetHistoryText.o Drawer.o CanvasCreator.o FontLoader.o LogObject.o LogObjectContainer.o LogObjectFactory.o MonitorLedClassObject.o MonitorLed.o JsonParser.o LoggerFactory.o Model.o MonitoredObject.o SourceData.o FetchRunner.o PinInterface.o PinState.o History.o Mode1Score.o Mode1ScoreTest.o TieBreakerTest.o TranslateConstant.o Logger.o TieBreaker.o PointLeds.o GameLeds.o SetLeds.o Mode1WinSequences.o Undo.o ServeLeds.o GameWinSequence.o SetWin.o MatchWinSequence.o Inputs.o WatchTimer.o Reset.o



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
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<
                                                                                                                                                                                
TieBreakerTest.o: TieBreakerTest.cpp                                                                                                                                                
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $< 

Mode1ScoreTest.o: Mode1Score/Mode1ScoreTest.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

Mode1ScoreTest: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o Mode1ScoreTest $(TEST_OBJECTS) $(LDFLAGS)

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
	@echo $* = $($*)d
```

Here is the partial output of the Linux tree command to give you an idea of the folder structure one level up:
```bash
...
├── text_scoreboard_test.cpp
├── TieBreaker
│   ├── Makefile
│   ├── prompt.md
│   ├── TieBreaker.cpp
│   ├── TieBreaker.h
│   └── TieBreakerTest.cpp
├── TieLeds
│   ├── TieLeds.cpp
│   └── TieLeds.h
├── TranslateConstant
│   ├── TranslateConstant.cpp
│   └── TranslateConstant.h
├── Undo
│   ├── log.txt
│   ├── Makefile
│   ├── run_tests
│   ├── Undo.cpp
│   ├── Undo.h
│   └── UndoTest.cpp
├── unit_run_bug.md
├── use_arguments.md
├── WatchTimer
│   ├── WatchTimer.cpp
│   └── WatchTimer.h
├── WebLiquidCrystal
│   ├── WebLiquidCrystal.cpp
│   └── WebLiquidCrystal.h
├── WinSequence
│   ├── WinSequence.cpp
│   └── WinSequence.h
├── WinSequences
│   ├── WinSequences.cpp
│   └── WinSequences.h
...
```

Notice how I have added a new directory called TieBreaker.  I need you to rewrite the Makefile in this TieBreaker directory using the provided Makefile as a guide.  Please make the necessary path adjustments as the libries are in the same place as the provided Makefile, just one directoy up.  I am going to delete the file in the TieBreaker folder and replace it with the one that you write.  Please pay attention to the directory structure.



Please write the Makefile for this TieBreaker test.