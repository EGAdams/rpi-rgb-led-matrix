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
#include "Drawer/Drawer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "GameObject/GameObject.h"
#include "LoggerFactory/LoggerFactory.h"

using namespace rgb_matrix;
#define SCORE_DELAY    .15
#define MAX_LOOP_COUNT 350
#define A_SPACE        13
#define FOUR_SPACE     14
#define THREE_SPACE    15

void writeMessage( GameObject* gameObject, std::string message ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getScoreBoard()->writeMessage( message );
    sleep( 3 );
    gameObject->getScoreBoard()->clearScreen();
}


void score( GameObject* gameObject, GameState* gameState, int player ) {
    gameObject->playerScore( player );  // flip the player score flag
    gameObject->loopGame();             // handle the player score flag
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
    gameObject->start();
    // gameObject->loopGame();
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
    score( gameObject, gameState, 1 );
    sleep( 3 );
}

void test_02( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
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
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 ); // player 2 needs to bump the serve 1st
    playerWin( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 ); // this score wins player 1 set.
    sleep( 2 );
}

void test_03( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    sleep( 4 ); }

void test_04( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getPlayer1()->setPoints( 0 );
    gameState->setPlayer1Points(         0 );
    gameObject->getPlayer2()->setPoints( 2 );
    gameState->setPlayer2Points(         2 );
    gameObject->getPlayer1()->setGames(  6 );
    gameObject->getPlayer2()->setGames(  5 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    GameTimer::gameDelay( 4000 );
    score( gameObject, gameState, 2 );    // now to trigger the tie break...
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    sleep( 6 ); }

void test_05( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getPlayer1()->setPoints( 0 );
    gameState->setPlayer1Points(         0 );
    gameObject->getPlayer2()->setPoints( 2 );
    gameState->setPlayer2Points(         2 );
    gameObject->getPlayer1()->setGames(  6 );
    gameObject->getPlayer2()->setGames(  5 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    GameTimer::gameDelay( 4000 );
    score( gameObject, gameState, 2 );    // now to trigger the tie break...
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    sleep( 6 ); }

void test_06( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen(); // Initialize the game state
    gameObject->getPlayer1()->setPoints( 0 );
    gameState->setPlayer1Points( 0 );
    gameObject->getPlayer2()->setPoints( 0 );
    gameState->setPlayer2Points( 0 );
    gameObject->getPlayer1()->setGames( 5 );
    gameObject->getPlayer2()->setGames( 4 );
    playerWin(gameObject, gameState, 1); // Player 1 wins the first set
    gameObject->getPlayer1()->setPoints( 0 ); // Reset points for the next set
    gameState->setPlayer1Points( 0 );
    gameObject->getPlayer2()->setPoints( 0 );
    gameState->setPlayer2Points( 0 );
    gameObject->getPlayer1()->setGames( 5 );
    gameObject->getPlayer2()->setGames( 4 );
    playerWin( gameObject, gameState, 1 ); // Player 1 wins the second set and the match
    if ( !gameState->gameRunning()) { // Check match win condition and display the result
    }
}

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
        sleep( 750 );
        gameObject->loopGame();  // handle the player score flag
        loop_count++;
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////
    std::cout << "game loop exited.  loop_count: " << loop_count << std::endl;
    if ( loop_count > MAX_LOOP_COUNT ) {
        // sleep for 5 seconds
        std::cout << "sleeping for 5 seconds..." << std::endl;
        sleep( 120 );
        std::cout << "MAX_LOOP_COUNT reached.  Exiting...\n\n\n\n\n" << std::endl; }
    return; }

int main( int argc, char *argv[]) {
    std::unique_ptr<MonitoredObject> logger = LoggerFactory::createLogger( "TestLogger" );
    // if --argument is manual then run manual tests
    int manual = 0;
    if( argc > 1 ) {
        std::string arg1 = argv[1];
        if( arg1 == "--manual" ) {
            std::cout << "running manual tests..." << std::endl;
            manual = 1;
        }
    }
    # define TEST_TEXT__X__POSITION   6
    # define TEST_TEXT__Y__POSITION   40
    # define TEST_NUMBER__X__POSITION 5
    # define TEST_NUMBER__Y__POSITION 80



    int loop_count = 0;
    GameState*  gameState  = new GameState();  // make this 1st!!! cost me 3 days of debugging
    GameObject* gameObject = new GameObject( gameState );
    sleep( .5 );
    gameObject->loopGame();
    sleep( .5 );
    if ( manual == 1 ) { run_manual_game( gameObject, gameState, 1 ); return 0; }

    ///// run tests /////
    int test_count = 1;
    // gameLeds->getScoreBoard()->drawText( "Win",  YELLOW, 18, 80  );
    gameObject->getScoreBoard()->clearScreen();
    // gameObject->getScoreBoard()->drawText( "Test",  
    // YELLOW, TEST_TEXT__X__POSITION, TEST_TEXT__Y__POSITION );
    // gameObject->getScoreBoard()->drawText( " 01 ",  
    // YELLOW, TEST_NUMBER__X__POSITION, TEST_NUMBER__Y__POSITION );
    // GameTimer::gameDelay( 4000 );
    test_01( gameObject, gameState, &loop_count );
    test_count++;
    // end test_01

    gameObject->getScoreBoard()->clearScreen();
    gameObject->getScoreBoard()->drawText( "Test",  
    YELLOW, TEST_TEXT__X__POSITION, TEST_TEXT__Y__POSITION );
    gameObject->getScoreBoard()->drawText( " 02 ",  
    YELLOW, TEST_NUMBER__X__POSITION, TEST_NUMBER__Y__POSITION );
    GameTimer::gameDelay( 4000 );
    std::cout << "calling test_02()..." << std::endl;
    test_02( gameObject, gameState, &loop_count );
    test_count++;
    // end test_02
    // writeMessage( gameObject, "t " + std::to_string( test_count ));
    // test_03( gameObject, gameState, &loop_count );
    test_count++;
    // end test_03
    // test_04( gameObject, gameState, &loop_count );
    // end test_04
    test_count++;
    // test 05
    // test_05( gameObject, gameState, &loop_count );
    test_count++;

   //  writeMessage( gameObject, "t " + std::to_string( test_count ));
    // writeMessage( gameObject, "Match" );
    // sleep( 6 );
    // test_06( gameObject, gameState, &loop_count );
    // gameObject->getScoreBoard()->clearScreen();
    // gameObject->getScoreBoard()->drawText( "10,20",  YELLOW, 10, 20  );
    // gameObject->getScoreBoard()->drawText( "10,80",  YELLOW, 10, 80  );
    GameTimer::gameDelay( 2000 );
    test_count++;
}
