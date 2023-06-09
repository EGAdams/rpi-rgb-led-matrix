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

#include "FontLoader/FontLoader.h"
#include "TextDrawer/TextDrawer.h"
#include "NumberDrawer/NumberDrawer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "GameObject/GameObject.h"

using namespace rgb_matrix;

int main( int argc, char *argv[]) {

    ///////// Start Game Loop /////////////
    bool game_running = true;
    int loop_count = 0;
    #define MAX_LOOP_COUNT 60
    #define SCORE_DELAY    1
    #define A_SPACE        13
    #define FOUR_SPACE     14
    #define THREE_SPACE    15
    
    std::cout << "creating GameObject..." << std::endl;
    GameObject* gameObject = new GameObject();
    std::cout << "done creating game object." << std::endl;

    int randomPlayer = 1;
    std::signal( SIGINT, GameObject::_signalHandler );
    while ( game_running && GameObject::gSignalStatus != SIGINT ) {
        if ( loop_count >  MAX_LOOP_COUNT ) { game_running = false; }
        
        sleep( SCORE_DELAY );
        randomPlayer = rand() % 2 + 1; // generate random player between 1 and 2
        std::cout << "randomPlayer: " << randomPlayer << std::endl;
        gameObject->playerScore( randomPlayer );
        sleep( SCORE_DELAY );
        gameObject->loopGame();

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen


        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen


        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // bigNumberDrawer.DrawNumber( " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen



        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        



        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "2", 16, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "2", 16, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        // pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        // pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        // bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        // showLittleNumbers( canvas );
        // sleep( SCORE_DELAY );
        // canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen

        loop_count++; 
    }
    ///////// End Game Loop /////////////
    return 0;
}
