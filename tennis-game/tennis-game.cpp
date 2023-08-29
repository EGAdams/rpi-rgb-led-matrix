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
    std::cout << "  player 2 points: " << gameState->getPlayer2Points() << std::endl;
    std::cout << "player 1 games:  "  << gameState->getPlayer1Games();
    std::cout << "  player 2 games:  "  << gameState->getPlayer2Games()  << std::endl;
    std::cout << "player 1 sets:   "   << gameState->getPlayer1Sets();
    std::cout << "  player 2 sets:   "   << gameState->getPlayer2Sets();
    std::cout << "     current set: "     << gameState->getCurrentSet()      << std::endl;
    std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
    std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    std::cout << "end of game loop.  loop_count: " << loop_count << std::endl;
}

int main( int argc, char *argv[]) {
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

    std::cout << "creating GameObject..." << std::endl;
    GameState*  gameState  = new GameState();  // make this 1st!!! cost me 3 days of debugging
    GameObject* gameObject = new GameObject( gameState );
    std::cout << "done creating game object.  sleeping...\n\n\n\n\n" << std::endl;
    sleep( 1 );
    std::cout << "done sleeping.  calling gameObject->loopGame()..." << std::endl;
    gameObject->loopGame();

    std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
    sleep( 1 );

    int test_count = 0;
    std::signal( SIGINT, GameObject::_signalHandler );
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getScoreBoard()->writeMessage( "t " + std::to_string( ++test_count ));
    gameObject->getScoreBoard()->clearScreen();
    std::cout << "done sleeping.  calling gameObject->getScoreBoard()->writeMessage(). calling loopGame()..." << std::endl;
    gameObject->loopGame();
    std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
    sleep( 1 );
    gameObject->loopGame();
    sleep( 1 );
    score( gameObject, gameState, 1, &loop_count );
    score( gameObject, gameState, 2, &loop_count );
    score( gameObject, gameState, 2, &loop_count );
    score( gameObject, gameState, 1, &loop_count );
    score( gameObject, gameState, 1, &loop_count );
    score( gameObject, gameState, 2, &loop_count );
    score( gameObject, gameState, 2, &loop_count );
    score( gameObject, gameState, 1, &loop_count );
    score( gameObject, gameState, 1, &loop_count );
    sleep( 1 );
    score( gameObject, gameState, 1, &loop_count );
    std::cout << "next player 1 score wins..." << std::endl;
    std::cout << "player 1 won!" << std::endl;
    std::cout << "presumably done with test 1." << std::endl;
    sleep( 3 );
    // exit( 0 );
    return 0;
}
