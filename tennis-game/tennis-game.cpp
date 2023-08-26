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
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "FontLoader/FontLoader.h"
#include "TextDrawer/TextDrawer.h"
#include "NumberDrawer/NumberDrawer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "GameObject/GameObject.h"
#include "Inputs/Inputs.h"

using namespace rgb_matrix;


int main() {
    int loop_count = 0;
    #define MAX_LOOP_COUNT 350
    #define A_SPACE        13
    #define FOUR_SPACE     14
    #define THREE_SPACE    15
    #define SCORE_DELAY    1

    std::ifstream configFile("config.md" );
    if (!configFile.is_open()) {
        std::cerr << "Unable to open config.md" << std::endl;
        return 1;
    }

    std::cout << "creating GameState..." << std::endl;
    GameState*  gameState  = new GameState();
    std::cout << "creating GameObject..." << std::endl;
    std::cout << "creating players..." << std::endl;
    Player* player1 = new Player( gameState, PLAYER_1_INITIALIZED );
    Player* player2 = new Player( gameState, PLAYER_2_INITIALIZED );
    std::cout << "setting opponents..." << std::endl;
    player1->setOpponent( player2 ); player2->setOpponent( player1 );
    std::map<std::string, int> pin_map;
    PinState* pin_state = new PinState( pin_map );
    PinInterface* pinInterface = new PinInterface( pin_state );
    History* history = new History();
    Mode1Score* mode1Score = new Mode1Score( player1, player2, pinInterface, gameState, history );
    GameTimer* gameTimer = new GameTimer();
    Inputs* gameInputs = new Inputs( player1, player2, pinInterface, gameState );
    GameModes* gameModes = new GameModes( player1, player2, pinInterface, gameState, history );
    ScoreBoard* scoreBoard = new ScoreBoard( player1, player2, gameState );
    WebLiquidCrystal* lcd = new WebLiquidCrystal();
    GameObject* gameObject = new GameObject( player1, player2, pin_state, pinInterface, gameState, gameTimer, gameInputs, gameModes, scoreBoard, lcd );
    mode1Score->setScoreBoard( scoreBoard );
    gameState->setCurrentAction( "testing" );

    std::string line;
    int test_count = 0;
    while ( std::getline( configFile, line )) {
        // Check if the line starts a new test
        if ( line.find( "## Test " ) != std::string::npos ) {
            std::cout << "Test " << test_count << std::endl;
            test_count++;
            scoreBoard->clearScreen();
            scoreBoard->writeMessage( "t " + std::to_string( test_count ));
            // print the line
            // replace "## Test " with "Test "
            line = line.replace( 0, 8, "" );
            std::cout << "\n\n///////////////////////////////////////////////////////////////////////" << std::endl;
            std::cout << "Running test: " << line << std::endl;
            std::cout << "///////////////////////////////////////////////////////////////////////\n" << std::endl;
            int player1_score = 0, player1_sets = 0, player1_games = 0;
            int player2_score = 0, player2_sets = 0, player2_games = 0;

            // Read configuration for this test
            while ( std::getline( configFile, line ) && line.find( "// run game" ) == std::string::npos ) {
                std::istringstream iss(line );
                std::string arg, val;
                iss >> arg >> val;
                if ( arg == "--player1_score" ) {
                    player1_score = std::stoi( val );
                } else if ( arg == "--player2_score" ) {
                    player2_score = std::stoi( val );
                } else if ( arg == "--player1_sets" ) {
                    player1_sets = std::stoi( val );
                } else if ( arg == "--player2_sets" ) {
                    player2_sets = std::stoi( val );
                } else if ( arg == "--player1_games" ) {
                    player1_games = std::stoi( val );
                } else if ( arg == "--player2_games" ) {
                    player2_games = std::stoi( val ); }}

            // Now, set up the game state and run the test
            player1->setGames( player1_games );
            player2->setGames( player2_games );
            std::cout << "player 1 games: " << player1->getGames() << std::endl;
            std::cout << "player 2 games: " << player2->getGames() << std::endl;
            player1->setSets( gameState, player1_sets );
            player2->setSets( gameState, player2_sets );
            std::cout << "setting points in gameState..." << std::endl;
            gameState->setPlayer1Points( player1_score );
            gameState->setPlayer2Points( player2_score );
            std::cout << "setting sets in gameState..." << std::endl;
            gameState->setPlayer1Sets( player1_sets );
            gameState->setPlayer2Sets( player2_sets );
            std::cout << "setting games in gamestate..." << std::endl;
            gameState->setPlayer1Games( player1_games );
            gameState->setPlayer2Games( player2_games );
            std::cout << "updating scoreboard..." << std::endl;
            scoreBoard->update();
            std::cout << "sleeping for 2 seconds..." << std::endl;
            sleep( SCORE_DELAY );

            std::cout << "setting player points; player1: " << player1_score << ", player2: " << player2_score << std::endl;
            player1->setPoints( player1_score );
            player2->setPoints( player2_score );
            
            
            scoreBoard->update();
            sleep( SCORE_DELAY );
            // gameObject->playerScore( PLAYER_1_INITIALIZED );
            std::cout << "clearing scoreboard before updating..." << std::endl;
            mode1Score->getScoreBoard()->clearScreen();
            std::cout << "updating score for player 1.  player 1 score is: " << player1->getPoints() << std::endl;
            std::cout << "simulating player 1 score..." << std::endl;
            player1->setPoints( player1->getPoints() + 1 ); // simulate player 1 scoring!!
            mode1Score->updateScore( player1 );
            sleep( SCORE_DELAY );
            player1->setSets( gameState, 0 );
            player2->setSets( gameState, 0 );
            player1->setGames( 0 );
            player2->setGames( 0 );
            scoreBoard->update();
            sleep( SCORE_DELAY );
            sleep( SCORE_DELAY );
            sleep( SCORE_DELAY );
        }
    }

    delete gameState;  // delete all of the "newed" objects
    delete gameObject;
    delete player1;
    delete player2;
    configFile.close();
    return 0;
}