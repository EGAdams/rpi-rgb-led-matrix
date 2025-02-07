#include "Reset.h"

Reset::Reset( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState )
    : _player1( player1 ),
    _player2( player2 ),
    _pinInterface( pinInterface ),
    _gameState( gameState ) {
    _logger = new Logger( "Reset" );
}
Reset::~Reset() {
    std::cout << "*** Reset destructor called. ***" << std::endl;
    delete _logger;
}

void Reset::resetScoreboard() {
    _logger->setName( "resetScoreboard" );
    _logger->logUpdate( "resetting scoreboard..." );
    _pinInterface->pinDigitalWrite( P1_POINTS_LED1, LOW );
    _pinInterface->pinDigitalWrite( P1_POINTS_LED2, LOW );
    _pinInterface->pinDigitalWrite( P1_POINTS_LED3, LOW );
    _pinInterface->pinDigitalWrite( P1_POINTS_LED4, LOW );

    _pinInterface->pinDigitalWrite( P2_POINTS_LED1, LOW );
    _pinInterface->pinDigitalWrite( P2_POINTS_LED2, LOW );
    _pinInterface->pinDigitalWrite( P2_POINTS_LED3, LOW );
    _pinInterface->pinDigitalWrite( P2_POINTS_LED4, LOW );

    _pinInterface->pinDigitalWrite( P1_GAMES_LED0, LOW );
    _pinInterface->pinDigitalWrite( P1_GAMES_LED1, LOW );
    _pinInterface->pinDigitalWrite( P1_GAMES_LED2, LOW );
    _pinInterface->pinDigitalWrite( P1_GAMES_LED3, LOW );
    _pinInterface->pinDigitalWrite( P1_GAMES_LED4, LOW );
    _pinInterface->pinDigitalWrite( P1_GAMES_LED5, LOW );
    _pinInterface->pinDigitalWrite( P1_GAMES_LED6, LOW );
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );

    _pinInterface->pinDigitalWrite( P2_GAMES_LED0, LOW );
    _pinInterface->pinDigitalWrite( P2_GAMES_LED1, LOW );
    _pinInterface->pinDigitalWrite( P2_GAMES_LED2, LOW );
    _pinInterface->pinDigitalWrite( P2_GAMES_LED3, LOW );
    _pinInterface->pinDigitalWrite( P2_GAMES_LED4, LOW );
    _pinInterface->pinDigitalWrite( P2_GAMES_LED5, LOW );
    _pinInterface->pinDigitalWrite( P2_GAMES_LED6, LOW );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW );

    _pinInterface->pinDigitalWrite( P1_SETS_LED1, LOW );
    _pinInterface->pinDigitalWrite( P1_SETS_LED2, LOW );

    _pinInterface->pinDigitalWrite( P2_SETS_LED1, LOW );
    _pinInterface->pinDigitalWrite( P2_SETS_LED2, LOW );

    _pinInterface->pinDigitalWrite( P1_SERVE, LOW );
    _pinInterface->pinDigitalWrite( P2_SERVE, LOW );

    _logger->logUpdate( "turning tie leds off... " );
    tieLEDsOff();

    _gameState->setTieBreak( 0 );
    _gameState->setMatchTieBreak( 0 );
    // std::cout << "setting serve switch and serve inside RESET.cpp..." << std::endl;
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    _gameState->setPlayerButton( 0 );
    _gameState->setStarted(
        /*1*/ 0 );  // gameStart = true; TODO: the placing of this is questionable
    GameTimer::gameDelay( 200 );  // delay( 200 );
    _logger->logUpdate( "done resetting game." );
}

void Reset::refresh() {
    if ( _gameState->getRotaryChange() == 1 ) {
        _gameState->setRotaryChange( 0 );
        resetScoreboard();
        zeroPlayerValues();
        _gameState->setTieBreakOnly( 0 );
    }
}

void Reset::tieLEDsOn() {
    _gameState->setTieLEDsOn( 1 );
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH );
}

void Reset::tieLEDsOff() {
    _gameState->setTieLEDsOn( 0 );
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW );
}

void Reset::zeroPlayerValues() {
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _gameState->setPlayer1Points( _player1->getPoints());
    _gameState->setPlayer2Points( _player2->getPoints());
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _player1->setSets( _gameState, 0 );
    _player2->setSets( _gameState, 0 );
    _player1->setMatches( 0 );
    _player2->setMatches( 0 );
}
