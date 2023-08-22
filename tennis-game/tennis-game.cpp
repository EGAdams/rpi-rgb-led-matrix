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

using namespace rgb_matrix;


int main() {
    int loop_count = 0;
    #define MAX_LOOP_COUNT 350
    #define A_SPACE        13
    #define FOUR_SPACE     14
    #define THREE_SPACE    15
    #define SCORE_DELAY    .15

    std::ifstream configFile("config.txt");
    if (!configFile.is_open()) {
        std::cerr << "Unable to open config.txt" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        // Check if the line starts a new test
        if (line.find("test") != std::string::npos) {
            int player1_score = 0, player1_sets = 0, player1_games = 0;
            int player2_score = 0, player2_sets = 0, player2_games = 0;

            // Read configuration for this test
            while (std::getline(configFile, line) && line.find("// run game") == std::string::npos) {
                std::istringstream iss(line);
                std::string arg, val;
                iss >> arg >> val;
                if (arg == "--player1_score") {
                    player1_score = std::stoi(val);
                } else if (arg == "--player2_score") {
                    player2_score = std::stoi(val);
                } else if (arg == "--player1_sets") {
                    player1_sets = std::stoi(val);
                } else if (arg == "--player2_sets") {
                    player2_sets = std::stoi(val);
                } else if (arg == "--player1_games") {
                    player1_games = std::stoi(val);
                } else if (arg == "--player2_games") {
                    player2_games = std::stoi(val);
                }
            }

            // Now, set up the game state and run the test
            std::cout << "creating GameObject..." << std::endl;
            GameState*  gameState  = new GameState();  
            gameState->setPlayer1Points(player1_score);
            gameState->setPlayer2Points(player2_score);
            gameState->setPlayer1Sets(player1_sets);
            gameState->setPlayer2Sets(player2_sets);
            gameState->setPlayer1Games(player1_games);
            gameState->setPlayer2Games(player2_games);
            GameObject* gameObject = new GameObject(gameState);

            // ... Rest of the game logic ...

            gameObject->playerScore( PLAYER_1_INITIALIZED ); 
            gameObject->playerScore( PLAYER_2_INITIALIZED );

            // Once the game ends, reset it for the next test
            delete gameState;
            delete gameObject;
        }
    }

    configFile.close();
    return 0; 
}
