#include "WinSequence.h"
#include <iostream>

/* TODO: seriously consider changing the filename to match the class name.
already cost us .5 hour */

WinSequence::WinSequence()  {}
WinSequence::~WinSequence() {}

void WinSequence::run( Player* player, GameState* gameState, GameLeds* gameLeds, int games_in_memory ) {
    for ( int blink_sequence_count = 0; blink_sequence_count < LOOP_GAME_LAMP_WIN; blink_sequence_count++ ) {
        for ( int current_lamp = 0; current_lamp < NUMBER_OF_GAME_LAMPS; current_lamp++ ) {
            player->setGames( 99 );
            gameLeds->updateGames();  // UpdateGames();
            GameTimer::gameDelay( GAME_FLASH_DELAY );
            player->setGames( current_lamp );
            gameLeds->updateGames();  // UpdateGames();
            GameTimer::gameDelay( GAME_FLASH_DELAY );
        }
        player->setGames( games_in_memory );
        gameLeds->updateGames();

    } // end LOOP_GAME_LAMP_WIN loop
}
