#include "MatchWinSequence.h"
#include <iostream>
#define MATCH_WIN_OFFSET 12

MatchWinSequence::MatchWinSequence()  {}
MatchWinSequence::~MatchWinSequence() {}

void MatchWinSequence::run( Player* player, GameState* gameState, GameLeds* gameLeds, SetLeds* setLeds ) {
    std::cout << "//////////////////////// MatchWinSequence::run() ////////////////////////" << std::endl;
    for( int flash_count = 0; flash_count < ALL_SETS_FLASH_COUNT; flash_count++ ) {
        print( "All sets flash delay in MatchWinSequence run()... " << MATCH_WIN_FLASH_DELAY );
        GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
        gameState->setCurrentAction( DRAW_BLANK_SETS ); // set flag before update
        gameLeds->getScoreBoard()->update();
        print( "All sets flash delay in MatchWinSequence run()... " << MATCH_WIN_FLASH_DELAY );
        GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
        gameState->setCurrentAction( NORMAL_GAME_STATE );
        gameLeds->getScoreBoard()->update(); }
    gameState->setCurrentAction( RUNNING_MATCH_WIN_SEQUENCE ); // so scoreboard knows...
    if ( gameLeds->getScoreBoard()->hasCanvas()) {
        gameLeds->getScoreBoard()->clearScreen();
        gameLeds->getScoreBoard()->setLittleDrawerFont( "fonts/8x13B.bdf" );
        print( "flash MATCH WIN... " );
        for ( int blink_count = 0; blink_count < MATCH_WIN_BLINK_COUNT; blink_count++ ) {
            gameLeds->getScoreBoard()->setLittleDrawerFont( "the_sets_numbers.bdf" );
            gameLeds->getScoreBoard()->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
            gameLeds->getScoreBoard()->drawNewText( "Win"  , 20, 75 - MATCH_WIN_OFFSET );
            gameLeds->getScoreBoard()->drawSets();
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
            print( "done match win flash delay of " << MATCH_WIN_FLASH_DELAY );
            gameLeds->getScoreBoard()->drawNewText( "     ", 13, 60 - MATCH_WIN_OFFSET );
            gameLeds->getScoreBoard()->drawNewText( "   "  , 20, 75 - MATCH_WIN_OFFSET );
            if ( blink_count == MATCH_WIN_BLINK_COUNT - 1 ) { gameLeds->getScoreBoard()->clearScreen(); }
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
            print( "done match win flash delay of " << MATCH_WIN_FLASH_DELAY );
        }
        gameLeds->getScoreBoard()->setLittleDrawerFont( "fonts/8x13B.bdf" );
    } else {
        ScoreBoard* scoreBoard = gameLeds->getScoreBoard();
        for ( int blink_count = 0; blink_count < MATCH_WIN_BLINK_COUNT; blink_count++ ) {
            gameState->setMatchBlinkOn();
            scoreBoard->update();
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
            gameState->setMatchBlinkOff(); 
            scoreBoard->update();
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
        }
    }
    gameLeds->getScoreBoard()->clearScreen();
    std::cout << "match win sequence is done.  switching to after match win state..." << std::endl;
    gameLeds->getScoreBoard()->setLittleDrawerFont( "the_sets_numbers.bdf" );
    gameLeds->getScoreBoard()->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
    gameLeds->getScoreBoard()->drawNewText( "Win"  , 20, 75 - MATCH_WIN_OFFSET );
    gameLeds->getScoreBoard()->drawSets();
    gameState->setState( AFTER_MATCH_WIN_STATE );
}    
