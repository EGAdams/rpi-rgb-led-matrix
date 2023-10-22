#include "MatchWinSequence.h"
#include <iostream>

MatchWinSequence::MatchWinSequence()  {}
MatchWinSequence::~MatchWinSequence() {}

void MatchWinSequence::_showText( ScoreBoard* scoreboard, std::string message, int first_offset, int baseline ) {
    
}

void MatchWinSequence::run( Player* player, GameState* gameState, GameLeds* gameLeds, SetLeds* setLeds ) {
    std::cout << "//////////////////////// MatchWinSequence::run() ////////////////////////" << std::endl;
    GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
    if ( gameLeds->getScoreBoard()->hasCanvas()) {
        std::cout << "scoreboard has canvas.  updating..." << std::endl;
        gameLeds->getScoreBoard()->drawText( "MATCH", YELLOW, 10, 20 );
        gameLeds->getScoreBoard()->drawText( "WIN",   YELLOW, 30, 40 );
        GameTimer::gameDelay( 5000 );
        std::cout << "game delay done." << std::endl;
    } else {
        std::cout << "scoreboard does not have canvas.  not updating..." << std::endl;
        for ( int blink_sequence_count = 0; blink_sequence_count < LOOP_MATCH_LAMP_WIN; blink_sequence_count++ ) {
            for ( int current_lamp = 0; current_lamp < NUMBER_OF_GAME_LAMPS; current_lamp++ ) {
                player->setGames( 6 );
                gameLeds->updateGames();
                GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
                player->setGames( current_lamp );
                gameLeds->updateGames();
                GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY ); }
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY ); }} // end LOOP_MATCH_LAMP_WIN loop  // end run().
}