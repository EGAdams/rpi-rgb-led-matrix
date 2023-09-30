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
    _serveLeds( pinInterface, gameState ) {}

Mode1Functions::~Mode1Functions() {}

void Mode1Functions::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard( scoreBoard );
    _mode1Score.setScoreBoard( scoreBoard ); }

void Mode1Functions::mode1ButtonFunction() {
    switch ( _gameState->getPlayerButton()) {
    case 0:
        break;

    case 1: // Player 1 Score
        _undo.snapshot( _history );
        if ( _gameState->getPointFlash() == 1 ) {
            _gameState->setPointFlash( 0 );
            _player1->setPoints( _gameState->getP1PointsMem());
            _player2->setPoints( _gameState->getP2PointsMem());
            _gameState->setPlayer1Points( _player1->getPoints());
            _gameState->setPlayer2Points( _player2->getPoints());
            // _pointLeds.updatePoints(); jul20
        }
        GameTimer::gameDelay( _gameState->getButtonDelay());
        // if serving, increment score.  if not serving, set serve to 1 and don't increment score.
        if ( _gameState->getServe() == PLAYER_ONE_SERVE || 
            /*aways increment when tie break is enabled*/ _gameState->getTieBreak() == 1 )  {
            _player1->setPoints( _player1->getPoints() + 1 );
            _gameState->setPlayer1Points( _player1->getPoints());
        } else {
            _gameState->setServe( PLAYER_ONE_SERVE ); }
        _undo.memory();
        _mode1Score.playerOneScore();
        break;

    case 3: // UNDO button pressed
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );  // Mode1Undo();
        break;

    case 2: // Player 2 Score
        _undo.snapshot( _history );
        if ( _gameState->getPointFlash() == 1 ) {
            _gameState->setPointFlash( 0 );
            _player1->setPoints( _gameState->getP1PointsMem());
            _player2->setPoints( _gameState->getP2PointsMem());
            _gameState->setPlayer1Points( _player1->getPoints());
            _gameState->setPlayer2Points( _player2->getPoints());
            /*_pointLeds.updatePoints(); jul20 */ }

        GameTimer::gameDelay( _gameState->getButtonDelay());
        if ( _gameState->getServe() == PLAYER_TWO_SERVE || 
            /*aways increment when tie break is enabled*/ _gameState->getTieBreak() == 1 )  {
            _player2->setPoints( _player2->getPoints() + 1 );
            _gameState->setPlayer2Points( _player2->getPoints());
        } else {
            _gameState->setServe( PLAYER_TWO_SERVE ); }
        _undo.memory();
        _mode1Score.playerTwoScore();
        break;

    case 4:
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );
        break; }                        // end switch ( _gameState->getPlayerButton())
    _gameState->setPlayerButton( 0 ); } // reset player button here!

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
                    _gameState->setPreviousTime( _gameState->getNow()); }}}}