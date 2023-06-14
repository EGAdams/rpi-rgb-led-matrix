#include "GameLeds.h"

GameLeds::~GameLeds() {}
GameLeds::GameLeds( Player* player1, Player* player2, PinInterface* pinInterface )
    : _player1( player1), _player2( player2 ), _pinInterface( pinInterface ) {
    _gameLedTranslator = new GameLedTranslator( player1, player2, pinInterface ); }

void GameLeds::setScoreBoard(ScoreBoard* scoreBoard) { _scoreBoard = scoreBoard; }

void GameLeds::updateGames() {
    if ( _scoreBoard->hasCanvas()) {
        _scoreBoard->clearScreen();
        _scoreBoard->drawGames();
    } else { _gameLedTranslator->drawGameLeds(); }}