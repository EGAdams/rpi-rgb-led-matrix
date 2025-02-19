#include "Mode1Functions.h"

Mode1Functions::Mode1Functions( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState,
    History* history )
    : _player1( player1 ),
    _player2( player2 ),
    _gameState( gameState ),
    _history( history ),
    _undo( player1, player2, pinInterface, gameState ),
    _pointLeds( player1, player2, pinInterface ),
    _mode1Score( player1, player2, pinInterface, gameState, history ),
    _serveLeds( pinInterface, gameState ) {
        _logger         = new Logger( "Mode1Functions" );
        _remoteLocker   = new RemoteLocker( gameState );
    }

Mode1Functions::~Mode1Functions() {}

void Mode1Functions::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard(   scoreBoard  );
    _mode1Score.setScoreBoard(  scoreBoard  ); }

void Mode1Functions::mode1ButtonFunction() {
    int button = _gameState->getPlayerButton();
    _flashRemote( button );
    switch ( button ) {
    
    case 0:
        break;

    case 1: // Player 1 Score
        // if ( _remoteLocker->playerNotServing( button )) { 
        //     print( "*** WARNING: player " << button << " not serving. *** " );
        //     return;
        // }
        _undo.snapshot( _history );
        if ( _gameState->getPointFlash() == 1 ) {
            _gameState->setPointFlash( 0 );
            _player1->setPoints( _gameState->getP1PointsMem());
            _player2->setPoints( _gameState->getP2PointsMem());
            _gameState->setPlayer1Points( _player1->getPoints());
            _gameState->setPlayer2Points( _player2->getPoints());
        }
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _player1->setPoints( _player1->getPoints() + 1 );
        _gameState->setPlayer1Points( _player1->getPoints());
        _undo.memory(); //
        _mode1Score.playerOneScore();
        break;

    case 3: // UNDO button pressed
        // if ( _remoteLocker->playerNotServing( button )) { 
        //     print( "*** WARNING: player " << button << " not serving. *** " );
        //     return;
        // }
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );  // Mode1Undo();
        break;

    case 2: // Player 2 Score
        // if ( _remoteLocker->playerNotServing( button )) { 
        //     print( "*** WARNING: player " << button << " not serving. *** " );
        //     return;
        // }
        _undo.snapshot( _history );
        if ( _gameState->getPointFlash() == 1 ) {
            _gameState->setPointFlash( 0 );
            _player1->setPoints( _gameState->getP1PointsMem());
            _player2->setPoints( _gameState->getP2PointsMem());
            _gameState->setPlayer1Points( _player1->getPoints());
            _gameState->setPlayer2Points( _player2->getPoints()); }

        GameTimer::gameDelay( _gameState->getButtonDelay());
        _player2->setPoints( _player2->getPoints() + 1 );
        _gameState->setPlayer2Points( _player2->getPoints());
        _undo.memory();
        _mode1Score.playerTwoScore();
        break;

    case 4:
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );
        break; 
    } // end switch ( _gameState->getPlayerButton())
    _gameState->setPlayerButton( 0 );   // reset player button here!
}

void Mode1Functions::mode1ServeFunction() {
    _undo.snapshot( _history );
    _serveLeds.serveSwitch(); }

void Mode1Functions::pointFlash() {
    if ( _gameState->getPointFlash() == 1 ) {
        if ( _player1->getPoints() > 3 ) {
            if ( _gameState->getNow() - _gameState->getPreviousTime() > _gameState->getFlashDelay()) {
                if ( _gameState->getToggle() == 0 ) {
                    _player1->setPoints( SCORE_CASE_4 );
                    _pointLeds.updatePoints();
                    _gameState->setToggle( 1 );
                } else {
                    _player1->setPoints(
                    _gameState->getP1PointsMem());
                    _pointLeds.updatePoints();
                    _gameState->setToggle( 0 ); }
                _gameState->setPreviousTime( _gameState->getNow()); }}

        if ( _player2->getPoints() > 3 ) {
                if ( _gameState->getNow() - _gameState->getPreviousTime() > _gameState->getFlashDelay()) {
                    if ( _gameState->getToggle() == 0 ) {
                        _player2->setPoints( SCORE_CASE_4 );
                        _pointLeds.updatePoints();
                        _gameState->setToggle( 1 );
                    } else {
                        _player2->setPoints(
                        _gameState->getP2PointsMem());
                        _pointLeds.updatePoints();
                        _gameState->setToggle( 0 ); }
                    _gameState->setPreviousTime( _gameState->getNow()); }}}
}

void Mode1Functions::_flashRemote( int current_remote_button ) {
    if ( current_remote_button == 1 ) {
        _scoreBoard->drawGreenPeriod();
        GameTimer::gameDelay( BUTTON_LED_FLASH_DELAY );
        _scoreBoard->drawBlankPeriod();
    } else if ( current_remote_button == 2 ) {
        _scoreBoard->drawRedPeriod();
        GameTimer::gameDelay( BUTTON_LED_FLASH_DELAY );
        _scoreBoard->drawBlankPeriod();
    } else {
        print ( "*** Warning: unhandled button press ***" );
    }
}
