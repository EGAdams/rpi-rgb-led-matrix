#include "MatchWinSequence.h"
#include <iostream>

MatchWinSequence::MatchWinSequence()  {}
MatchWinSequence::~MatchWinSequence() {}

void MatchWinSequence::run( Player* player, GameState* gameState, GameLeds* gameLeds, SetLeds* setLeds ) {
    std::cout << "//////////////////////// MatchWinSequence::run() ////////////////////////" << std::endl;
    for( int flash_count = 0; flash_count < ALL_SETS_FLASH_COUNT; flash_count++ ) {
        GameTimer::gameDelay( ALL_SETS_FLASH_DELAY );
        gameState->setCurrentAction( DRAW_BLANK_SETS ); // set flag before update
        gameLeds->getScoreBoard()->update();
        GameTimer::gameDelay( ALL_SETS_FLASH_DELAY );
        gameState->setCurrentAction( NORMAL_GAME_STATE );
        gameLeds->getScoreBoard()->update(); }
    gameState->setCurrentAction( RUNNING_MATCH_WIN_SEQUENCE ); // so scoreboard knows...
    GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
    if ( gameLeds->getScoreBoard()->hasCanvas()) {
        gameLeds->getScoreBoard()->clearScreen();
        gameLeds->getScoreBoard()->drawText( "Match", 10, 60  );
        gameLeds->getScoreBoard()->drawText( "Win"  , 18, 80  );
        gameLeds->getScoreBoard()->drawSets();
        GameTimer::gameDelay( SHOW_MATCH_WIN_TEXT_DELAY );
    } else {
        ScoreBoard* scoreBoard = gameLeds->getScoreBoard();
        for ( int blink_count = 0; blink_count < MATCH_WIN_BLINK_COUNT; blink_count++ ) {
            gameState->setMatchBlinkOn();
            scoreBoard->update();
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
            gameState->setMatchBlinkOff(); 
            scoreBoard->update();
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
        }}
    std::cout << "match win sequence is done." << std::endl;
}
