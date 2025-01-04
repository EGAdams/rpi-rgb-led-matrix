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

    if ( _iteration < 1 ) {
        std::cerr << "*** ERROR: _getServe() in TieBreaker.cpp is returning default value. ***"
            << std::endl;
        std::cerr << "The iteration is: " << _iteration << std::endl;
        return PLAYER_1_SERVE; // Default to PLAYER_1_SERVE for invalid iterations
    }
    int serveIndex = ( _iteration - 1 ) % 4; // Adjust the iteration for zero-based indexing and calculate serve index
    return servePattern[serveIndex];
}

void TieBreaker::setIteration( int iteration ) { _iteration = iteration; }
int  TieBreaker::getIteration() { return _iteration; }
void TieBreaker::incrementIteration() { _iteration++; }

void TieBreaker::setScoreBoards( ScoreBoard* scoreBoard ) {
    _pointLeds.setScoreBoard( scoreBoard );
    _setLeds.setScoreBoard( scoreBoard );
    _gameLeds.setScoreBoard( scoreBoard );
    _undo.setScoreBoard( scoreBoard );
    _scoreBoard = scoreBoard;
}

void TieBreaker::tieLEDsOn() {
    _gameState->setTieLEDsOn( 1 );
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH );
    if (_scoreBoard != nullptr) { _scoreBoard->update();
    if ( _scoreBoard->hasCanvas()) { _scoreBoard->drawTieBreakerBar(); }
    } else { print( "*** ERROR: _scoreBoard is NULL in TieBreaker::tieLEDsOn! ***" );}
}

void TieBreaker::tieLEDsOff() {
    _gameState->setTieLEDsOn( 0 );
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW );
    _scoreBoard->update();
}

void TieBreaker::celebrate( Player* currentPlayer ) {
    GameTimer::gameDelay( _gameState->getWinDelay() );
    SetWin setWin( &_undo, _gameState, &_setLeds );
    setWin.execute( currentPlayer, _scoreBoard );
}

void TieBreaker::incrementSet() {
    _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 ); // increment set
}

// the mode 1 tb part updates games.  this is ok for when we have the LEDs,
// but when there is a matrix, we update the points instead.
void TieBreaker::run( Player* currentPlayer ) {
    _undo.memory();

    // ERROR CHECKS
    if ( currentPlayer == nullptr ) { std::cerr << "*** ERROR: Current player is null in TieBreaker::run(). ***" << std::endl; exit( 1 );}
    Player* opponent = currentPlayer->getOpponent(); if ( opponent == nullptr ) { std::cerr << "*** ERROR: Opponent is null in TieBreaker::run(). ***" << std::endl; exit( 1 );}
    int serve = _getServe(); if ( serve != PLAYER_1_SERVE && serve != PLAYER_2_SERVE ) { std::cerr << "*** ERROR: Invalid serve value in TieBreaker::run(). ***" << std::endl; std::cerr << "Serve value: " << serve << std::endl; exit( 1 );}
    if ( _scoreBoard == nullptr ) { std::cerr << "*** ERROR: ScoreBoard is null in TieBreaker::run(). ***" << std::endl; return;}
    
    // START OF run() TIE BREAKER
    _gameState->setServe( serve ); // set the serve bar depending tie-break iteration
    try { _scoreBoard->update();} catch ( const std::exception& e ) { std::cerr << "*** EXCEPTION: " << e.what() << " ***" << std::endl; exit( 1 );} catch ( ... ) { std::cerr << "*** UNKNOWN EXCEPTION in ScoreBoard update ***" << std::endl; exit( 1 );}
    if ( currentPlayer->getPoints() == TIE_BREAK_MAX_POINTS ) { _tieBreakWin( currentPlayer );}
    else if ( currentPlayer->getPoints() >= TIE_BREAK_WIN_BY_TWO &&
            ( currentPlayer->getPoints() - opponent->getPoints() >= 2 ) ) {
        _tieBreakWin( currentPlayer );
    } else { incrementIteration();} // need this to determine serve bar location
}

void TieBreaker::_tieBreakWin( Player* currentPlayer ) {
    if ( _gameState->getMatchTieBreak() == true ) {               // Match Win
    
        // increment the set
        currentPlayer->setGames( 1 ); // set games to 1 for Match Win
        if ( currentPlayer->number() == PLAYER_1_INITIALIZED ) {
            _gameState->setPlayer1SetHistory( currentPlayer->getSetHistory());
            _gameState->setPlayer2SetHistory( currentPlayer->getOpponent()->getSetHistory());
        } else {
            _gameState->setPlayer2SetHistory( currentPlayer->getSetHistory());
            _gameState->setPlayer1SetHistory( currentPlayer->getOpponent()->getSetHistory());
        }
        _scoreBoard->update();
        MatchWinSequence  mws;
        mws.run( currentPlayer, _gameState, &_gameLeds, &_setLeds );
        _gameState->setCurrentAction( SLEEP_MODE );
    } else {                                                      // regular TB win
        currentPlayer->setGames( currentPlayer->getGames() + 1 ); // increment games

        // increment the set
        currentPlayer->setSets( currentPlayer->getSets() + 1 ); // increment sets
        if ( currentPlayer->number() == PLAYER_1_INITIALIZED ) {
            _gameState->setPlayer1SetHistory( currentPlayer->getSetHistory());
            _gameState->setPlayer2SetHistory( currentPlayer->getOpponent()->getSetHistory());
        } else {
            _gameState->setPlayer2SetHistory( currentPlayer->getSetHistory());
            _gameState->setPlayer1SetHistory( currentPlayer->getOpponent()->getSetHistory());
        }
        
        // update before  pause...
        print( "*** UPDATING Before PAUSE ***" );
        if ( _scoreBoard != nullptr ) { _scoreBoard->update();}
        
        // pause for 2 seconds
        print( "*** PAUSING FOR 2 SECONDS ***" );
        GameTimer::gameDelay( 2000 );
        

        endTieBreak();
        celebrate( currentPlayer );
        incrementSet();
        GameTimer::gameDelay( 3000 );
        std::cout << "calling end tie break Object in regular tie break win.. " << std::endl;
    }
}

/**
# **Method Documentation: `mode1SetTBButtonFunction()`**

 * Initializes the tie-break mode of the tennis game.
 * This function sets up the initial state of the tie-break, including resetting the player points, setting the serve, and updating the game LEDs. It also blinks the tie-break LEDs a certain number of times to indicate the start of the tie-break.


### **Description:
**Handles the button functionality for the tie-break mode of the tennis game.**
This function is invoked when a button is pressed during the tie-break mode. It performs various actions based on the button pressed, such as incrementing the games for a player, undoing the previous action, or updating the game LEDs.

### **Purpose:
The code `mode1SetTBButtonFunction()` is a method that manages different actions based on the value of a button input. It is likely part of a larger program that oversees a tennis game or scoreboard.

### **Functionality:
The primary goal of this code is to update the scores or execute specific actions depending on the button pressed by the user.

### **Inputs:
- **`_gameState->getPlayerButton()`: Represents the button pressed by the user.

### **Outputs:
- **Updated scores** or execution of specific actions, such as undoing a previous action or delaying the program for a certain amount of time.

### **Implementation:
The code achieves its purpose through a **switch statement** that checks the value of `_gameState->getPlayerButton()`. Depending on the value, different cases are executed:

- ## Case 0: No action is taken.
- ## Case 1:
  - Delay for a certain amount of time (`_gameState->getButtonDelay()`).
  - Take a snapshot of the current state (`_undo.snapshot(_history)`).
  - Increment the games score for player 1 (`_player1->setGames(_player1->getGames() + 1)`).
  - Update the game LEDs for player 1 (`mode1SetTBP1Games()`).
- ## Case 2:
  - Delay for a certain amount of time (`_gameState->getButtonDelay()`).
  - Set the scoreboard for the undo operation (`_undo.setScoreBoard(_scoreBoard)`).
  - Perform an undo operation (`_undo.mode1Undo(_history)`).
- ## Case 3:
  - Delay for a certain amount of time (`_gameState->getButtonDelay()`).
  - Take a snapshot of the current state (`_undo.snapshot(_history)`).
  - Increment the games score for player 2 (`_player2->setGames(_player2->getGames() + 1)`).
  - Update the game LEDs for player 2 (`mode1SetTBP2Games()`).
- ## Case 4:
  - Delay for a certain amount of time (`_gameState->getButtonDelay()`).
  - Perform an undo operation (`_undo.mode1Undo(_history)`).

### **Logic Flow:
The important logic flow is the **switch statement** that checks the button input and executes the corresponding case. The data transformations happening are the updates to the game scores (`_player1->setGames()` and `_player2->setGames()`), the snapshots of the current state (`_undo.snapshot(_history)`), and the undo operations (`_undo.mode1Undo(_history)`).

### **Post-Execution:
After executing the appropriate case, the function **resets the button input value** to 0 (`_gameState->setPlayerButton(0)`).
*/
void TieBreaker::mode1SetTBButtonFunction() {
    switch ( _gameState->getPlayerButton() ) {
    case 0:
        break;

    case 1:
        GameTimer::gameDelay( _gameState->getButtonDelay() );
        _undo.snapshot( _history );
        _player1->setGames( _player1->getGames() + 1 );
        mode1SetTBP1Games(); // updates game LEDs in the other game.  we need points here!
        break;

    case 2:
        GameTimer::gameDelay( _gameState->getButtonDelay() );
        _undo.setScoreBoard( _scoreBoard );
        _undo.mode1Undo( _history );
        break;

    case 3:
        GameTimer::gameDelay( _gameState->getButtonDelay() );
        _undo.snapshot( _history );
        _player2->setGames( _player2->getGames() + 1 );
        mode1SetTBP2Games();
        break;

    case 4:
        GameTimer::gameDelay( _gameState->getButtonDelay() );
        _undo.mode1Undo( _history );
        break;
    }
    _gameState->setPlayerButton( 0 );
}

void TieBreaker::setTieBreaker() {
    _undo.memory();
    mode1SetTBButtonFunction();
}

void TieBreaker::initializeTieBreakMode() { // 103024
    _iteration = 1;  // this is initialized to zero before, so it could be checked as another flag
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _pointLeds.updatePoints();
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    _serveLeds.serveSwitch();
    if ( _gameState->getTieLEDsOn() == 0 ) { tieLEDsOn(); }
    _gameLeds.updateGames();
    // Inputs _inputs( _player1, _player2, _pinInterface, _gameState );
    // WatchTimer _watchTimer;
    // for ( int currentPulseCount = 0; currentPulseCount < TIE_PULSE_COUNT; currentPulseCount++ ) {
    //     _gameState->setCurrentAction( DRAW_BLANK_SETS ); // set flag before update
    //     tieLEDsOff(); // set tie led flag to 0 and update score board
    //     if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }
    //     tieLEDsOn(); // draw tie breaker bar and update score board
    //     if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }
    // }

    
    
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    tieLEDsOn();
}

void TieBreaker::blinkSetScores() {
    for( int flash_count = 0; flash_count < ALL_SETS_FLASH_COUNT; flash_count++ ) {
        GameTimer::gameDelay( ALL_SETS_FLASH_DELAY );
        _gameState->setCurrentAction( DRAW_BLANK_SETS ); // set flag before update
        _gameLeds.getScoreBoard()->update();
        _gameLeds.getScoreBoard()->blankTieBreakerBar();
        GameTimer::gameDelay( ALL_SETS_FLASH_DELAY );
        _gameState->setCurrentAction( NORMAL_GAME_STATE );
        _gameLeds.getScoreBoard()->update();
        _gameLeds.getScoreBoard()->drawTieBreakerBar();
    }
}

void TieBreaker::setTieBreakEnable() {
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _pointLeds.updatePoints();
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    _serveLeds.serveSwitch();
    Inputs _inputs( _player1, _player2, _pinInterface, _gameState );
    WatchTimer _watchTimer;
    if ( !_scoreBoard->onRaspberryPi()) {
        for ( int currentPulseCount = 0; currentPulseCount < TIE_PULSE_COUNT; currentPulseCount++ ) {
            tieLEDsOff();
            if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }
            tieLEDsOn();
            if ( _watchTimer.watchInputDelay( TIE_BREAK_BLINK_DELAY, &_inputs, TIE_BREAK_WATCH_INTERVAL ) > 0 ) { return; }
        }
    } // end not on pi
    if ( _gameState->getTieLEDsOn() == 0 ) { tieLEDsOn(); }
    // _player1->setGames( 0 );
    // _player2->setGames( 0 );  // not here!  we are using points for the scoring.
    // _gameLeds.updateGames();  // no need for this.
    // incrementSet(); // 081224 removed this for match win.  THIS MAY BREAK SOMETHING ELSE
    // a better way would be to set a match win tie break flag and check it here.
    // GameTimer::gameDelay( UPDATE_DISPLAY_DELAY ); // wtf..  *** stack smashing detected ***: terminated
}

void TieBreaker::endTieBreak() {
    tieLEDsOff();
    _iteration = 0;
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _player1->setGames( 0 );  // this is because we where using game LEDs for the scoring
    _player2->setGames( 0 );
    _pointLeds.updatePoints();
    _gameLeds.updateGames();
    _gameState->setTieBreak( 0 );
    _gameState->setMatchTieBreak( 0 );
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    if ( _scoreBoard == nullptr ) {
        std::cerr << "*** ERROR: ScoreBoard is null in TieBreaker::run() before update. ***" << std::endl;
        exit( 1 );
    }
    try {
        _scoreBoard->update();
    }
    catch ( const std::exception& e ) {
        std::cerr << "*** EXCEPTION: " << e.what() << " ***" << std::endl;
        exit( 1 );
    }
    catch ( ... ) {
        std::cerr << "*** UNKNOWN EXCEPTION in ScoreBoard update ***" << std::endl;
        exit( 1 );
    }
}

void TieBreaker::mode1TBP1Games() {
    _gameLeds.updateGames();  // UpdateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );

    if ( _player1->getGames() == 15 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );;

        if ( _player2->getSets() == _player1->getSets() ) {
            endTieBreak();
            _mode1WinSequences.enterMatchTieBreak();
            _gameState->setMatchTieBreak( 1 );
            _gameState->setTieBreak( 1 );
            setTieBreakEnable();
        }
        else {
            _mode1WinSequences.p1SetWinSequence();
            endTieBreak();
        }
    }
    if ( _player1->getGames() >= 10 && ( _player1->getGames() - _player2->getGames() ) > 1 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            endTieBreak();
            _mode1WinSequences.enterMatchTieBreak(); // p1TBSetWinSequence();
            // _gameState->setMatchTieBreak( 1 );   
            // _gameState->setTieBreak( 1 );        
            // setTieBreakEnable();                    
        }
        else {
            _mode1WinSequences.p1SetWinSequence();
            endTieBreak();
        }
    }
}

void TieBreaker::mode1TBP2Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );

    if ( _player2->getGames() == 15 ) {
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames() );
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames() );
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            endTieBreak();
            _mode1WinSequences.enterMatchTieBreak(); //  p2TBSetWinSequence();
            _gameState->setMatchTieBreak( 1 );
            _gameState->setTieBreak( 1 );
            setTieBreakEnable();
        }
        else {
            _mode1WinSequences.p2SetWinSequence();
            endTieBreak();
        }
    }
    if ( _player2->getGames() >= 10 && ( _player2->getGames() - _player1->getGames() ) > 1 ) {
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames() );
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames() );
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            std::cout << "calling end tie break on august 3.." << std::endl;
            endTieBreak();
            std::cout << "calling p2TBSetWinSequence() on august 3.." << std::endl;
            _mode1WinSequences.enterMatchTieBreak(); // p2TBSetWinSequence();
            // _gameState->setMatchTieBreak( 1 );
            // _gameState->setTieBreak( 1 );
            // setTieBreakEnable();
            std::cout << "end mode1TBP2Games on august 3.." << std::endl;
        }
        else {
            _mode1WinSequences.p2SetWinSequence();
            endTieBreak();
        }
    }
}

void TieBreaker::mode1SetTBP2Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 7 ) {
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames() );
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames() );
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        _scoreBoard->update();
        _mode1WinSequences.p2SetTBWinSequence();
        tieLEDsOff();
        _mode1WinSequences.playerTwoMatchWin();
        // _gameState->stopGameRunning();
    }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}

void TieBreaker::mode1SetTBP1Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 7 ) {
        _player1->setSetHistory( _gameState->getCurrentSet(), _player1->getGames() );
        _player2->setSetHistory( _gameState->getCurrentSet(), _player2->getGames() );
        _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        _scoreBoard->update();
        _mode1WinSequences.p1SetTBWinSequence();
        tieLEDsOff();
        _mode1WinSequences.playerOneMatchWin();
        // _gameState->stopGameRunning();
    }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}

// currentPlayer->setGames( currentPlayer->getGames() + 1 ); // increment games
// set the current player's "6" to a "7" wherever it is in the set history
// for ( int i = 0; i < currentPlayer->getSetHistory().size(); i++ ) {
//     if ( currentPlayer->getSetHistory()[i] == GAMES_TO_WIN_SET ) {
//         currentPlayer->setSetHistory( i, GAMES_TO_WIN_SET + 1 );
//     }
// }