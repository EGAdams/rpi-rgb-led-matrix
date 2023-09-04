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
#define MAX_LOOP_COUNT 350
#define A_SPACE        13
#define FOUR_SPACE     14
#define THREE_SPACE    15

void writeMessage( GameObject* gameObject, std::string message ) {
    std::cout << "writing message: " << message << std::endl;
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getScoreBoard()->writeMessage( message );
    gameObject->getScoreBoard()->clearScreen();
    std::cout << "done writing message." << std::endl;
}


void score( GameObject* gameObject, GameState* gameState, int player ) {
    std::cout << "\n\n\n\n\n\n\n*** Player " << player << " scored ***\n" << std::endl;
    gameObject->playerScore( player );  // flip the player score flag
    gameObject->loopGame();  // handle the player score flag
    std::cout << "player 1 points: " << gameState->getPlayer1Points();
    std::cout << "player 2 points: " << gameState->getPlayer2Points() << std::endl;
    std::cout << "player 1 games:  " << gameState->getPlayer1Games();
    std::cout << "player 2 games:  " << gameState->getPlayer2Games() << std::endl;
    std::cout << "player 1 sets:   " << gameState->getPlayer1Sets();
    std::cout << "player 2 sets:   " << gameState->getPlayer2Sets();
    std::cout << "current set:     " << gameState->getCurrentSet() << std::endl;
    std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
    std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
}

void playerWin( GameObject* gameObject, GameState* gameState, int player ) {
    score( gameObject, gameState, player );
    score( gameObject, gameState, player );
    score( gameObject, gameState, player );
    score( gameObject, gameState, player );
}

void test_01( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    std::signal( SIGINT, GameObject::_signalHandler );
    gameObject->getScoreBoard()->clearScreen();
    std::cout << "done sleeping.  calling gameObject->getScoreBoard()->writeMessage(). calling loopGame()..." << std::endl;
    std::cout << "calling game object start()... " << std::endl;
    gameObject->start();
    std::cout << "done calling start(). \n\n\n\n\n" << std::endl;
    gameObject->loopGame();
    std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
    sleep( 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    sleep( 1 );
    std::cout << "next player 1 score wins..." << std::endl;
    score( gameObject, gameState, 1 );
    std::cout << "player 1 won!" << std::endl;
    std::cout << "presumably done with test 1." << std::endl;
    sleep( 3 );
}

void test_02( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    // sleep( 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );    
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    // sleep( 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    // sleep( 2 );
    playerWin( gameObject, gameState, 1 );
    score(     gameObject, gameState, 2 ); // player 2 needs to bump the serve 1st
    playerWin( gameObject, gameState, 2 );
    score(     gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    score(     gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    // sleep( 2 );
    std::cout << " ready for player 1 set win..." << std::endl;
    playerWin( gameObject, gameState, 1 );
    std::cout << "player 1 won the set!" << std::endl;
    sleep( 2 );
}

void test_03( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    //undo
    Undo* undo = gameObject->createUndo();
    undo->setScoreBoard( gameObject->getScoreBoard());
    sleep( 1 );
    undo->mode1Undo( gameObject->getHistory());
    sleep( 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    sleep( 2 );
    std::cout << " ready for player 1 set win..." << std::endl;
    playerWin( gameObject, gameState, 1 );
    std::cout << "player 1 won the set!" << std::endl;
    sleep( 4 ); }

void run_manual_game( GameObject* gameObject, GameState* gameState, int player ) {
    int loop_count = 0;
    std::signal( SIGINT, GameObject::_signalHandler );
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) {
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
    return;
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
    std::cout << "creating GameObject..." << std::endl;
    std::cout << "creating GameState..." << std::endl;
    GameState*  gameState  = new GameState();  // make this 1st!!! cost me 3 days of debugging
    std::cout << "creating GameObject..." << std::endl;
    GameObject* gameObject = new GameObject( gameState );
    std::cout << "done creating game object.  sleeping...\n\n\n\n\n" << std::endl;
    sleep( .5 );
    std::cout << "done sleeping.  calling gameObject->loopGame()..." << std::endl;
    gameObject->loopGame();
    std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
    sleep( .5 );
    std::cout << "manual set to: " << manual << std::endl;
    if ( manual == 1 ) { run_manual_game( gameObject, gameState, 1 ); return 0; } 

    ///// run tests /////
    int test_count = 0;
    writeMessage( gameObject, "t " + std::to_string( ++test_count ));
    std::cout << "calling test_01()..." << std::endl;
    test_01( gameObject, gameState, &loop_count );

    writeMessage( gameObject, "t " + std::to_string( ++test_count ));
    std::cout << "calling test_02()..." << std::endl;
    test_02( gameObject, gameState, &loop_count );

    writeMessage( gameObject, "t " + std::to_string( ++test_count ));
    std::cout << "calling test_03()..." << std::endl;
    test_03( gameObject, gameState, &loop_count );

}