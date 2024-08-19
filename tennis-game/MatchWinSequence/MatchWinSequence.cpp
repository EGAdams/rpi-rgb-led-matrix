#include "MatchWinSequence.h"
#include <iostream>

MatchWinSequence::MatchWinSequence()  {}
MatchWinSequence::~MatchWinSequence() {}

void MatchWinSequence::run( Player* player, GameState* gameState, GameLeds* gameLeds, SetLeds* setLeds ) {
    std::cout << "//////////////////////// MatchWinSequence::run() ////////////////////////" << std::endl;
    gameState->setCurrentAction( RUNNING_MATCH_WIN_SEQUENCE ); // so scoreboard knows...
    GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
    if ( gameLeds->getScoreBoard()->hasCanvas()) {
        gameLeds->getScoreBoard()->clearScreen();
        gameLeds->getScoreBoard()->drawText( "Match",  YELLOW, 10, 60  );
        gameLeds->getScoreBoard()->drawText( "Win",    YELLOW, 18, 80  );
        GameTimer::gameDelay( 3000 );
        std::cout << "game delay done." << std::endl;
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
    std::cout << "setting current action back to normal game state..." << std::endl;
    gameState->setCurrentAction( NORMAL_GAME_STATE );
    // delete scoreBoard; // this causes a segmentation fault
    player->clearSetHistory();
    player->getOpponent()->clearSetHistory();
    player->clearGameHistory();
    player->getOpponent()-> clearGameHistory();
    gameState->setPlayer1SetHistory( player->getSetHistory()); // both zero
    gameState->setPlayer2SetHistory( player->getOpponent()->getSetHistory());
    gameState->setGameHistory( player->getGameHistory() );
    gameState->setGameHistory( player->getOpponent()->getGameHistory() );
    gameState->setCurrentSet( 1 );
    gameLeds->getScoreBoard()->clearScreen();
    gameLeds->getScoreBoard()->update();
    // gameState->setStarted( 0 );  // trigger game reset, end run().
    gameState->setCurrentAction( NORMAL_GAME_STATE );
    std::cout << "match win sequence is done." << std::endl;
}
