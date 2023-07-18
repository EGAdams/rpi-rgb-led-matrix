#include "GameWinSequence.h"

GameWinSequence::GameWinSequence()  {}
GameWinSequence::~GameWinSequence() {}

void GameWinSequence::run( Player* player, GameState* gameState, 
                           GameLeds* gameLeds, ScoreBoard* scoreBoard, int games_in_memory ) {
    
    if ( TESTING == 1 ) { std::cout << "GameWinSequence::run() TESTING == 1" << std::endl; }
    if ( scoreBoard->hasCanvas()) {
        std::cout << "GameWinSequence::run() hasCanvas() == true" << std::endl;
        for ( int blink_sequence_count = 0; blink_sequence_count < LOOP_GAME_LAMP_WIN; blink_sequence_count++ ) {
            std::cout << "cloaking ... " << std::endl;
            gameState->setCurrentAction( "blink" );
            scoreBoard->update();
            std::cout << "uncloaking ... " << std::endl;
            GameTimer::gameDelay( gameState->getGameFlashDelay());  /*** wait ***/
            gameState->setCurrentAction( "normal operation" );
            scoreBoard->update(); 
            GameTimer::gameDelay( gameState->getGameFlashDelay());  /*** wait ***/ }}
    } else {
        std::cout << "GameWinSequence::run() hasCanvas() == false" << std::endl;
        for ( int blink_sequence_count = 0; blink_sequence_count < LOOP_GAME_LAMP_WIN; blink_sequence_count++ ) {  
            player->setGames( 5 );                                  // set game count so game LEDs will turn off
            gameLeds->updateGames();                                // actually turn game LEDs off
            GameTimer::gameDelay( gameState->getGameFlashDelay());  /*** wait ***/
            player->setGames( games_in_memory );                    // restore game count in player object  
            gameLeds->updateGames();                                // turn players game point on
            GameTimer::gameDelay( gameState->getGameFlashDelay());  /*** wait ****/ }}}