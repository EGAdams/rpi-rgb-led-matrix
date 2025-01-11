#include "GameWinSequence.h"

GameWinSequence::GameWinSequence()  {}
GameWinSequence::~GameWinSequence() {}

void GameWinSequence::run( Player* player, GameState* gameState, 
                           GameLeds* gameLeds, ScoreBoard* scoreBoard, int games_in_memory ) {
    // if ( scoreBoard->onRaspberryPi() == false ) { return; }
    std::string current_action = gameState->getCurrentAction();
    if ( !scoreBoard->onRaspberryPi() && current_action != RUNNING_MATCH_TIE_BREAK ) {
    print( "not returning out of GameWinSequence::run() because on Raspberry Pi" );
    if ( scoreBoard->hasCanvas()) {
        print ( "scoreboard has canvas, entering blink sequence loop..." );
        for ( int blink_sequence_count = 0; blink_sequence_count < GAME_WIN_SEQUENCE_LOOP_COUNT; blink_sequence_count++ ) {
            player->number() == PLAYER_1_INITIALIZED ? gameState->setCurrentAction( "player1 blink" ) 
                                : gameState->setCurrentAction( "player2 blink" );
            print( "updating scoreboard..." );
            scoreBoard->update();
            print( "sleeping " << GAME_FLASH_DELAY << " milliseconds..." );
            GameTimer::gameDelay( GAME_FLASH_DELAY );  /*** wait ***/
            gameState->setCurrentAction( NORMAL_GAME_STATE );
            scoreBoard->update(); 
            print( "sleeping " << GAME_FLASH_DELAY << " milliseconds..." );
            GameTimer::gameDelay( GAME_FLASH_DELAY );  /*** wait ***/ }
    } else {
        print ( "scoreboard does not have canvas, how did we ever get here?" );
        for ( int blink_sequence_count = 0; blink_sequence_count < GAME_WIN_SEQUENCE_LOOP_COUNT; blink_sequence_count++ ) {  
            player->setGames( 5 );                    // set game count so game LEDs will turn off
            gameLeds->updateGames();                                // actually turn game LEDs off
            print( "sleeping " << GAME_FLASH_DELAY << " milliseconds..." );
            GameTimer::gameDelay( GAME_FLASH_DELAY );  /*** wait ***/
            player->setGames( games_in_memory );                    // restore game count in player object  
            gameLeds->updateGames(); 
            print( "sleeping " << GAME_FLASH_DELAY << " milliseconds..." );                              // turn players game point on
            GameTimer::gameDelay( GAME_FLASH_DELAY );  /*** wait ****/ }
    }
    print ( "exiting GameWinSequence::run()" );
}