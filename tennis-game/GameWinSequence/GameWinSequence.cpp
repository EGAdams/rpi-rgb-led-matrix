#include "GameWinSequence.h"
#if defined _WIN32 || defined _WIN64
    #include <iostream>
#endif

GameWinSequence::GameWinSequence()  {}
GameWinSequence::~GameWinSequence() {}

void GameWinSequence::run( Player* player, GameState* gameState, GameLeds* gameLeds, int games_in_memory ) {
	#if defined _WIN32 || defined _WIN64
    	std::cout << "games_in_memory: " << games_in_memory << std::endl;
	#endif
    for ( int blink_sequence_count = 0; blink_sequence_count < LOOP_GAME_LAMP_WIN; blink_sequence_count++ ) {  
        player->setGames( 99 );                                 // set game count so game LEDs will turn off
        gameLeds->updateGames();                                // actually turn game LEDs off
        GameTimer::gameDelay( gameState->getGameFlashDelay());  // wait
        player->setGames( games_in_memory );                    // restore game count in player object  
        gameLeds->updateGames();                                // turn players game point on
        GameTimer::gameDelay( gameState->getGameFlashDelay());  // wait
    }} // end LOOP_GAME_LAMP_WIN loop    
