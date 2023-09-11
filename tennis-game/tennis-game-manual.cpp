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

    std::cout << "done calling loopGame().  sleeping...\n" << std::endl;
    sleep( 1 );

    int menu_selection = 1;
    std::signal( SIGINT, GameObject::_signalHandler );
    
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/  
        if ( loop_count >  MAX_LOOP_COUNT ) { gameState->stopGameRunning(); }
        sleep( SCORE_DELAY );
        std::cout << "\n  1.) Player 1 score" << std::endl;
        std::cout << "  2.) Player 2 score" << std::endl;
        std::cout << "  3.) Undo" << std::endl;
        std::cout << "  4.) Exit" << std::endl;
        std::cin >> menu_selection;
        if ( menu_selection == 1  ||  menu_selection == 2 ) {
            std::cout << "\n\n\n\n\n\n\n*** Player " << menu_selection << " scored ***\n" << std::endl;
            gameObject->playerScore( menu_selection );  // flip the player score flag
            sleep( SCORE_DELAY );
        } else if ( menu_selection == 4 ) {
            std::cout << "*** Exiting... ***\n" << std::endl;
            exit( 0 );
        } else if ( menu_selection == 3 ) { 
            std::cout << "\n\n\n\n\n\n\n*** Undo ***\n" << std::endl;
            gameObject->undo(); 
            sleep( SCORE_DELAY );
        } else {
            std::cout << "\n\n\n\n\n\n\n*** Invalid selection ***\n" << std::endl;
            sleep( SCORE_DELAY );
            continue; }
        
        gameObject->loopGame();  // handle the player score flag
        loop_count++;
        std::cout << "player 1 points: " << "\033[32m" << gameState->getPlayer1Points() << "\033[0m";
        std::cout << "  player 2 points: " << "\033[31m" << gameState->getPlayer2Points() << "\033[0m" << std::endl;
        std::cout << "player 1 games:  "  << gameState->getPlayer1Games();
        std::cout << "  player 2 games:  "  << gameState->getPlayer2Games()  << std::endl;
        std::cout << "player 1 sets:   "   << gameState->getPlayer1Sets();
        std::cout << "  player 2 sets:   "   << gameState->getPlayer2Sets();
        std::cout << "     current set: "     << gameState->getCurrentSet()      << std::endl;
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
        // std::cout << "end of game loop.  loop_count: " << loop_count << std::endl;
    } ///////// End Game Loop /////////
    std::cout << "game loop exited.  loop_count: " << loop_count << std::endl;
    if ( loop_count > MAX_LOOP_COUNT ) {
        // sleep for 5 seconds
        std::cout << "sleeping for 5 seconds..." << std::endl;
        sleep( 120 );
        std::cout << "MAX_LOOP_COUNT reached.  Exiting...\n\n\n\n\n" << std::endl; }
    return 0; }

    