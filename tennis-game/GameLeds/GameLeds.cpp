#include "GameLeds.h"

GameLeds::~GameLeds() { delete _gameLedTranslator; }
GameLeds::GameLeds( Player* player1, Player* player2, PinInterface* pinInterface )
    : _player1( player1), _player2( player2 ), _pinInterface( pinInterface ) {
    _gameLedTranslator = new GameLedTranslator( player1, player2, pinInterface ); }

void GameLeds::setScoreBoard( ScoreBoard* scoreBoard ) { _scoreBoard = scoreBoard; }
ScoreBoard* GameLeds::getScoreBoard() { return _scoreBoard; }

void GameLeds::updateGames() {
    if ( _scoreBoard   == NULL ) {
        // std::cout << "*** ERROR: scoreBoard is null. ***  returning to sender... " << std::endl;
        return; }

    if ( _gameLedTranslator == NULL ) {
        std::cout << "*** ERROR: _gameLedTranslator is null. ***  exiting... " << std::endl; exit( 1 ); }

    if ( _scoreBoard->hasCanvas()) { _scoreBoard->update();
    } else { _gameLedTranslator->drawGameLeds(); }}
