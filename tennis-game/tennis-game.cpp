// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// The main tennis game entry point.
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "../include/led-matrix.h"
#include "../include/graphics.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <csignal>
#include <map>
#include "FontLoader/FontLoader.h"
#include "TextDrawer/TextDrawer.h"
#include "NumberDrawer/NumberDrawer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "GameObject/GameObject.h"

using namespace rgb_matrix;
#define SCORE_DELAY    .15

void score( GameObject* gameObject, GameState* gameState, int player, int* loop_count ) {
    std::cout << "\n\n\n\n\n\n\n*** Player " << player << " scored ***\n" << std::endl;
    gameObject->playerScore( player );  // flip the player score flag
    gameObject->loopGame();  // handle the player score flag
    loop_count++;
    std::cout << "player 1 points: " << gameState->getPlayer1Points();
    std::cout << "player 2 points: " << gameState->getPlayer2Points() << std::endl;
    std::cout << "player 1 games:  " << gameState->getPlayer1Games();
    std::cout << "player 2 games:  " << gameState->getPlayer2Games() << std::endl;
    std::cout << "player 1 sets:   " << gameState->getPlayer1Sets();
    std::cout << "player 2 sets:   " << gameState->getPlayer2Sets();
    std::cout << "current set:     " << gameState->getCurrentSet() << std::endl;
    std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
    std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    std::cout << "end of game loop.  loop_count: " << loop_count << std::endl;
}


void test_01( GameObject* gameObject, GameState* gameState, int player, int* loop_count ) {
    std::signal( SIGINT, GameObject::_signalHandler );
    gameObject->getScoreBoard()->clearScreen();
    std::cout << "done sleeping.  calling gameObject->getScoreBoard()->writeMessage(). calling loopGame()..." << std::endl;
    std::cout << "calling game object start()... " << std::endl;
    gameObject->start();
    std::cout << "done calling start(). \n\n\n\n\n" << std::endl;
    gameObject->loopGame();
    std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
    sleep( 1 );
    score( gameObject, gameState, 1, loop_count );
    score( gameObject, gameState, 2, loop_count );
    score( gameObject, gameState, 2, loop_count );
    score( gameObject, gameState, 1, loop_count );
    score( gameObject, gameState, 1, loop_count );
    score( gameObject, gameState, 2, loop_count );
    score( gameObject, gameState, 2, loop_count );
    score( gameObject, gameState, 1, loop_count );
    score( gameObject, gameState, 1, loop_count );
    sleep( 1 );
    std::cout << "next player 1 score wins..." << std::endl;
    score( gameObject, gameState, 1, loop_count );
    std::cout << "player 1 won!" << std::endl;
    std::cout << "presumably done with test 1." << std::endl;
    sleep( 3 );

    // gameObject->getScoreBoard()->clearScreen();
    // std::cout << "done sleeping.  calling gameObject->getScoreBoard()->writeMessage(). calling loopGame()..." << std::endl;
    // std::cout << "calling game object start()... " << std::endl;
    // gameObject->start();
    // std::cout << "done calling start(). \n\n\n\n\n" << std::endl;
    // gameObject->loopGame();
    // std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
    // sleep( 1 );
    // score( gameObject, gameState, 1, loop_count );
    // score( gameObject, gameState, 2, loop_count );
    // score( gameObject, gameState, 2, loop_count );
    // score( gameObject, gameState, 1, loop_count );
    // score( gameObject, gameState, 1, loop_count );
    // score( gameObject, gameState, 2, loop_count );
    // score( gameObject, gameState, 2, loop_count );
    // score( gameObject, gameState, 1, loop_count );
    // score( gameObject, gameState, 1, loop_count );
    // score( gameObject, gameState, 2, loop_count );
    // score( gameObject, gameState, 2, loop_count );
    // score( gameObject, gameState, 2, loop_count );
    // score( gameObject, gameState, 1, loop_count );
    // score( gameObject, gameState, 1, loop_count );

    // sleep( 1 );
    // std::cout << "next player 1 score wins..." << std::endl;
    // score( gameObject, gameState, 1, loop_count );
    // std::cout << "player 1 won!" << std::endl;
    // std::cout << "presumably done with test 1." << std::endl;
    // sleep( 3 );
}

int main( int argc, char *argv[]) {
    // if --argument is manual then run manual tests
    int manual = 0;
    if( argc > 1 ) {
        std::string arg1 = argv[1];
        if( arg1 == "--manual" ) {
            std::cout << "running manual tests..." << std::endl;
            manual = 1;
        }
    }
    int loop_count = 0;
    #define MAX_LOOP_COUNT 350
    #define A_SPACE        13
    #define FOUR_SPACE     14
    #define THREE_SPACE    15
    #define SCORE_DELAY    .15

    // set games to --games argument
    // set sets to --sets argument
    // set player1 score to --player1 argument
    // set player2 score to --player2 argument
    std::cout << "manual set to: " << manual << std::endl;
    if ( manual == 1 ) {
        std::cout << "creating GameObject..." << std::endl;
        GameState*  gameState  = new GameState();  // make this 1st!!! cost me 3 days of debugging
        GameObject* gameObject = new GameObject( gameState );
        std::cout << "done creating game object.  sleeping...\n\n\n\n\n" << std::endl;
        sleep( 1 );
        std::cout << "done sleeping.  calling gameObject->loopGame()..." << std::endl;
        gameObject->loopGame();
        std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
        sleep( 1 );
        int player = 1;
        std::signal( SIGINT, GameObject::_signalHandler );
        /*/// Begin Game Loop ///*/ while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) {
            if ( loop_count >  MAX_LOOP_COUNT ) { gameState->stopGameRunning(); }
            sleep( SCORE_DELAY );
            std::cout << "\n\nenter 1 or 2 to score for player 1 or 2: ";
            std::cin >> player;
            std::cout << "\n\n\n\n\n\n\n*** Player " << player << " scored ***\n" << std::endl;
            gameObject->playerScore( player );  // flip the player score flag
            sleep( SCORE_DELAY );
            gameObject->loopGame();  // handle the player score flag
            loop_count++;
            std::cout << "player 1 points: " << gameState->getPlayer1Points();
            std::cout << "  player 2 points: " << gameState->getPlayer2Points() << std::endl;
            std::cout << "player 1 games:  "  << gameState->getPlayer1Games();
            std::cout << "  player 2 games:  "  << gameState->getPlayer2Games()  << std::endl;
            std::cout << "player 1 sets:   "   << gameState->getPlayer1Sets();
            std::cout << "  player 2 sets:   "   << gameState->getPlayer2Sets();
            std::cout << "     current set: "     << gameState->getCurrentSet()      << std::endl;
            std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
            std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
            std::cout << "end of game loop.  loop_count: " << loop_count << std::endl;
        } ///////// End Game Loop /////////
        std::cout << "game loop exited.  loop_count: " << loop_count << std::endl;
        if ( loop_count > MAX_LOOP_COUNT ) {
            // sleep for 5 seconds
            std::cout << "sleeping for 5 seconds..." << std::endl;
            sleep( 120 );
            std::cout << "MAX_LOOP_COUNT reached.  Exiting...\n\n\n\n\n" << std::endl; }
        return 0;
    } else {
        std::cout << "creating GameState..." << std::endl;
        GameState*  gameState  = new GameState();  // make this 1st!!! cost me 3 days of debugging
        std::cout << "creating GameObject..." << std::endl;
        GameObject* gameObject = new GameObject( gameState );
        std::cout << "done creating game object.  sleeping...\n\n\n\n\n" << std::endl;
        sleep( 1 );
        std::cout << "done sleeping.  calling gameObject->loopGame()..." << std::endl;
        gameObject->loopGame();
        std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
        sleep( 1 );
        int test_count = 0;
        gameObject->getScoreBoard()->writeMessage( "t " + std::to_string( ++test_count ));
        gameObject->getScoreBoard()->clearScreen();
        std::cout << "calling test_01()..." << std::endl;
        test_01( gameObject, gameState, 1, &loop_count );
    }
    return 0;
}
