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

int TieBreaker::_getServe() {
    switch ( _iteration ) {
    case 1:
        return PLAYER_2_SERVE; // doesn't matter who scores here.  doesn't concern me.
        break;
    
    case 2:
        return PLAYER_2_SERVE;
        break;

    case 3:
        return PLAYER_1_SERVE;
        break;

    case 4:
        return PLAYER_1_SERVE;
        break;
    
    case 5:
        return PLAYER_2_SERVE;
        break;
    
    case 6:
        return PLAYER_2_SERVE;
        break;
    
    case 7:
        return PLAYER_1_SERVE;
        break;

    case 8:
        return PLAYER_1_SERVE;
        break;

    case 9:
        return PLAYER_2_SERVE;
        break;

    case 10:
        return PLAYER_2_SERVE;
        break;
    
    case 11:
        return PLAYER_1_SERVE;
        break;

    case 12:
        return PLAYER_1_SERVE;
        break;
    
    case 13:
        return PLAYER_2_SERVE;
        break;

    case 14:
        return PLAYER_2_SERVE;
        break;

    case 15:
        return PLAYER_1_SERVE;
        break;
    
    case 16:
        return PLAYER_1_SERVE;
        break;

    case 17:
        return PLAYER_2_SERVE;
        break;

    case 18:
        return PLAYER_2_SERVE;
        break;

    case 19:
        return PLAYER_1_SERVE;
        break;

    case 20:
        return PLAYER_1_SERVE;
        break;

    case 21:
        return PLAYER_2_SERVE;
        break;

    case 22:
        return PLAYER_2_SERVE;
        break;

    case 23:
        return PLAYER_1_SERVE;
        break;

    case 24:
        return PLAYER_1_SERVE;
        break;

    case 25:
        return PLAYER_2_SERVE;
        break;
    
    default:
        std::cout << "*** WARNING: _getServe() in TieBreaker.cpp is returning default value. ***"
                  << std::endl;
        std::cout << "The iteration is: " << _iteration << std::endl;
        return PLAYER_1_SERVE;
        break;
    }
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
    _gameState->setServe( _getServe()); // set the serve bar depending tie-break iteration
    _scoreBoard->update();
    Player* opponent = currentPlayer->getOpponent();

    if ( currentPlayer->getPoints() == TIE_BREAK_MAX_POINTS ) {
        _undo.snapshot( _history );                                   
        currentPlayer->setGames( currentPlayer->getGames() + 1 );     // increment games
        incrementSet();
        _scoreBoard->update();
        celebrate( currentPlayer );    // this is a win no matter what.
        GameTimer::gameDelay( 3000 );
        endTieBreak(); 
    } else if ( currentPlayer->getPoints() >= TIE_BREAK_WIN_BY_TWO  && 
        ( currentPlayer->getPoints() - opponent->getPoints() >= 2)) {
        _undo.snapshot( _history );                                   
        currentPlayer->setGames( currentPlayer->getGames() + 1 );     // increment games
        incrementSet();
        _scoreBoard->update();
        celebrate( currentPlayer );
        GameTimer::gameDelay( 3000 );
        endTieBreak(); 
    } else {
                               // needed to put this here otherwise tie break would
                               // be incremented even after a win.
        incrementIteration();  // need this to determine serve bar location
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
    _scoreBoard->update(); }

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