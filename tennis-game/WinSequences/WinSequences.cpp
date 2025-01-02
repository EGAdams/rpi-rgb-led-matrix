#include "WinSequences.h"

Mode1WinSequences::~Mode1WinSequences() {
    // // // std::cout << "*** Mode1WinSequences destructor called. ***" << std::endl;
    delete _setWin; }

Mode1WinSequences::Mode1WinSequences( Player* player1, Player* player2, PinInterface* pinInterface,
                                      GameState* gameState ):
    _player1( player1 ), _player2( player2 ),
    _pinInterface( pinInterface ),
    _gameState( gameState ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _reset( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {
        _setWin = new SetWin( &_undo, _gameState, &_setLeds );}

void Mode1WinSequences::setScoreBoards( ScoreBoard* scoreBoard ) {
    _pointLeds.setScoreBoard( scoreBoard );
    _gameLeds.setScoreBoard(  scoreBoard );
    _setLeds.setScoreBoard(   scoreBoard );
    _scoreBoard =             scoreBoard; } // set mine too!

void Mode1WinSequences::p1GameWinSequence() {
    // // std::cout << "//////////////////////// p1GameWinSequence() ////////////////////////" << std::endl;
    GameWinSequence gameWinSequence;
    gameWinSequence.run( _player1, _gameState, &_gameLeds, _scoreBoard, _player1->getGames());
    // // std::cout << "*** inside WinSequences class.  p1GameWinSequence() ***" << std::endl;
    _undo.memory();
    // // std::cout << "*** delaying game after p1GameWinSequence()... ***" << std::endl;
    GameTimer::gameDelay( GAME_FLASH_DELAY );
    // // std::cout << "*** done delaying game after p1GameWinSequence()  setting points to zero... ***" << std::endl;
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _gameState->setPlayer1Points( 0 );
    _gameState->setPlayer2Points( 0 );

    // Switch the server at the end of a game
    if (_gameState->getServe() == PLAYER_ONE_SERVE) {
        _gameState->setServe(PLAYER_TWO_SERVE);
    } else {
        _gameState->setServe(PLAYER_ONE_SERVE);
    }
}

void Mode1WinSequences::p1SetWinSequence() {
    // // std::cout << "*** executing _setWin->execute for player one... ***" << std::endl;
    _setWin->execute( _player1, _scoreBoard ); }

void Mode1WinSequences::playerOneMatchWin() {
    // // std::cout << "//////////////////////// playerOneMatchWin() ////////////////////////" << std::endl;
    _undo.memory();
    _pointLeds.updateTBPoints();
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player1, _gameState, &_gameLeds, &_setLeds );
    _player1->clearGameHistory();
    _player2->clearGameHistory();
    _player1->clearSetHistory();
    _player2->clearSetHistory();
    _gameState->setPlayer1SetHistory( _player1->getSetHistory());
    _gameState->setPlayer2SetHistory( _player2->getSetHistory());
    _gameState->setCurrentSet( 1 );
    _reset.resetScoreboard();
}

void Mode1WinSequences::playerTwoMatchWin() {
    _undo.memory();
    _pointLeds.updateTBPoints();
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player2, _gameState, &_gameLeds, &_setLeds );
    _player1->clearGameHistory();
    _player2->clearGameHistory();
    _player1->clearSetHistory();
    _player2->clearSetHistory();
    _gameState->setPlayer1SetHistory( _player1->getSetHistory());
    _gameState->setPlayer2SetHistory( _player2->getSetHistory());
    _gameState->setCurrentSet( 1 );   
    _reset.resetScoreboard(); 
}


void Mode1WinSequences::p2GameWinSequence() {
    GameWinSequence gameWinSequence;
    // std::cout << "calling game win sequence run from p2GameWinSequence... " << std::endl;
    gameWinSequence.run( _player2, _gameState, &_gameLeds, _scoreBoard, _player2->getGames());
    // std::cout << "calling undo.memory() from inside p2GameWinSequence... " << std::endl;
    _undo.memory();
    // std::cout << "calling gameDelay from inside p2GameWinSequence... " << std::endl;
    GameTimer::gameDelay( GAME_FLASH_DELAY );
    // std::cout << "calling setPoints to zero for player1 from inside p2GameWinSequence... " << std::endl;
    _player1->setPoints( 0 );
    // std::cout << "calling setPoints to zero for player2 from inside p2GameWinSequence... " << std::endl;
    _player2->setPoints( 0 );
    // std::cout << "calling setPlayer1Points to zero for player1 from inside p2GameWinSequence... " << std::endl;
    _gameState->setPlayer1Points( 0 );
    // std::cout << "calling setPlayer2Points to zero for player2 from inside p2GameWinSequence... " << std::endl;
    _gameState->setPlayer2Points( 0 );

    // std::cout << "calling gameState->setServe from inside p2GameWinSequence... " << std::endl;
    // Switch the server at the end of a game
    if (_gameState->getServe() == PLAYER_ONE_SERVE) {
        _gameState->setServe(PLAYER_TWO_SERVE);
    } else {
        _gameState->setServe(PLAYER_ONE_SERVE);
    }
}

void Mode1WinSequences::p2SetWinSequence() { _setWin->execute( _player2, _scoreBoard ); }

void Mode1WinSequences::p1TBGameWinSequence() {
    _undo.memory();
    if ( _scoreBoard->hasCanvas()) {
        GameWinSequence gameWinSequence;
        gameWinSequence.run( _player1, _gameState, &_gameLeds, _scoreBoard, _player1->getGames());
    } else {
        for ( int currentPulseCount = 0; currentPulseCount < _gameState->getGameWinPulseCount();
                currentPulseCount++ ) {
                _player1->setSets( _gameState, 0 );
                _setLeds.updateSets();
                GameTimer::gameDelay( _gameState->getFlashDelay());
                _player1->setSets( _gameState, _gameState->getP1SetsMem());
                _setLeds.updateSets();
                GameTimer::gameDelay( _gameState->getFlashDelay());}}
    tieLEDsOff();
    _gameState->setTieBreak( 0 );
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();}

void Mode1WinSequences::p2TBGameWinSequence() {
    _undo.memory();
    if ( _scoreBoard->hasCanvas()) {     // the matrix is active
        GameWinSequence gameWinSequence;
        gameWinSequence.run( _player2, _gameState, &_gameLeds, _scoreBoard, _player2->getGames());
    } else {
        for ( int currentPulseCount = 0;
            currentPulseCount < _gameState->getGameWinPulseCount();
            currentPulseCount++ ) {
            _player2->setSets( _gameState, 0 );
            _setLeds.updateSets();
            GameTimer::gameDelay( _gameState->getFlashDelay());
            _player2->setSets( _gameState, _gameState->getP2SetsMem());
            _setLeds.updateSets();
            GameTimer::gameDelay( _gameState->getFlashDelay()); }}
    tieLEDsOff();
    _gameState->setTieBreak( 0 );
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames(); }


//////////////////////////// ENTER MATCH TIE BREAK  ///////////////////////////
void Mode1WinSequences::enterMatchTieBreak() {  
    _undo.memory();
    _gameLeds.getScoreBoard()->update();
    for( int flash_count = 0; flash_count < ALL_SETS_FLASH_COUNT; flash_count++ ) {
        GameTimer::gameDelay( ALL_SETS_FLASH_DELAY );
        _gameState->setCurrentAction( DRAW_BLANK_SETS ); // set flag before update
        _gameLeds.getScoreBoard()->update();
        GameTimer::gameDelay( ALL_SETS_FLASH_DELAY );
        _gameState->setCurrentAction( NORMAL_GAME_STATE );
        _gameLeds.getScoreBoard()->update();
        _gameLeds.getScoreBoard()->drawTieBreakerBar();
    }
    tieLEDsOn();
}
///////////////////////////// END MATCH TIE BREAK /////////////////////////////


////////////////////////// SET TIE BREAKER WIN SEQUENCES //////////////////////////////////////////
void Mode1WinSequences::p1SetTBWinSequence() {  // for winning set t/b
    _undo.memory();

    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player1->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player1->setSets( _gameState, 3 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}}

void Mode1WinSequences::p2SetTBWinSequence() {  // for winning set t/b
    _undo.memory();
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player2->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player2->setSets( _gameState, 3 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}}
//////////////////////// END OF SET TIE BREAKER WIN SEQUENCES /////////////////////////////////////

void Mode1WinSequences::p1TBMatchWinSequence() {
    _player2->setGames( 99 );
    _gameLeds.updateGames();
    _undo.memory();
    tieLEDsOff();
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player1, _gameState, &_gameLeds, &_setLeds );
    _reset.resetScoreboard();}

void Mode1WinSequences::p2TBMatchWinSequence() {
    _player1->setGames( 99 );
    _gameLeds.updateGames();
    _undo.memory();
    tieLEDsOff();
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player2, _gameState, &_gameLeds, &_setLeds );
    _reset.resetScoreboard();}

void Mode1WinSequences::tieLEDsOn() {
    _gameState->setTieLEDsOn( 1 );  // tieLEDsOn = true;
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH );}

void Mode1WinSequences::tieLEDsOff() {
    _gameState->setTieLEDsOn( 0 );  // tieLEDsOn = false;
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW );}
