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
#define SCORE_DELAY    1
#define MAX_LOOP_COUNT 350
#define A_SPACE        13
#define FOUR_SPACE     14
#define THREE_SPACE    15
#define DEMO_DELAY 1
#define BLINK_UPDATE_DELAY 1000000 // 250000 // 
#define FASTER_BLINK_UPDATE_DELAY 100000 // 750000 // 100000
# define X__POS 6
# define Y__POS 40
# define X__POSITION 5
# define Y__POSITION 80
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



void scoreDelay( GameObject* gameObject, GameState* gameState, int player, int delay ) {
    gameObject->getScoreBoard()->blink_player_score( player );
    usleep( BLINK_UPDATE_DELAY );
    gameObject->playerScore( player );  // flip the player score flag
    gameObject->loopGame();             // handle the player score flag
    usleep( BLINK_UPDATE_DELAY / 2 );
    std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
    std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
}

void score( GameObject* gameObject, GameState* gameState, int player ) {
    gameObject->getScoreBoard()->blink_player_score( player );
    usleep( FASTER_BLINK_UPDATE_DELAY );
    gameObject->playerScore( player );  // flip the player score flag
    gameObject->loopGame();             // handle the player score flag
    std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
    std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
}

void playerWinDelay( GameObject* gameObject, GameState* gameState, int player, int delay ) {    
    scoreDelay( gameObject, gameState, player, delay );
    while( gameState->getPlayer1Points() != 0 || gameState->getPlayer2Points() != 0 ) {
        scoreDelay( gameObject, gameState, player, delay );
    }      
}

void checkeredPlayerOneWinDelay( GameObject* gameObject, GameState* gameState, int delay ) {
    scoreDelay( gameObject, gameState, 0, delay );
    scoreDelay( gameObject, gameState, 1, delay );
    scoreDelay( gameObject, gameState, 1, delay );
    scoreDelay( gameObject, gameState, 0, delay );
    scoreDelay( gameObject, gameState, 1, delay );
    scoreDelay( gameObject, gameState, 1, delay );
    scoreDelay( gameObject, gameState, 0, delay );
    scoreDelay( gameObject, gameState, 1, delay );
    scoreDelay( gameObject, gameState, 1, delay );
    scoreDelay( gameObject, gameState, 0, delay ); 
    while( gameState->getPlayer1Points() != 0 || gameState->getPlayer2Points() != 0 ) {
        scoreDelay( gameObject, gameState, 0, delay );
    }      
}

void playerWin( GameObject* gameObject, GameState* gameState, int player ) {       
    score( gameObject, gameState, player );
    while( gameState->getPlayer1Points() != 0 || gameState->getPlayer2Points() != 0 ) {
        score( gameObject, gameState, player );
    }
}

void normal_win_one( GameObject* gameObject, GameState* gameState ) {
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 1, DEMO_DELAY );
}

void normal_win_one_no_delay( GameObject* gameObject, GameState* gameState ) {
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
}

void different_win_one_no_delay( GameObject* gameObject, GameState* gameState ) {
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
}

void normal_win_two( GameObject* gameObject, GameState* gameState ) {
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 2, DEMO_DELAY );
}

void two_win_scramble( GameObject* gameObject, GameState* gameState ) {
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 2, DEMO_DELAY );
}

void one_win_scramble( GameObject* gameObject, GameState* gameState ) {
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
}

void ad_win_one_comeback( GameObject* gameObject, GameState* gameState ) {
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 1, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    scoreDelay( gameObject, gameState, 2, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 1, DEMO_DELAY );
}

/*
 * Runs a demo with delays until the score reaches 3 - 2
 * Then kicks into high gear and finishes the game at 6 - 2 
 */
void demo_test( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->start();
    sleep( 1 );
    
    normal_win_one( gameObject, gameState );
    ad_win_one_comeback( gameObject, gameState );
    normal_win_two( gameObject, gameState );
    two_win_scramble( gameObject, gameState );
    normal_win_one( gameObject, gameState );

    // faster now...
    one_win_scramble( gameObject, gameState );
    normal_win_one_no_delay( gameObject, gameState );
    different_win_one_no_delay( gameObject, gameState );
    normal_win_one_no_delay( gameObject, gameState );
    sleep( 20 );
}

void test_01( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    std::signal( SIGINT, GameObject::_signalHandler );
    gameObject->getScoreBoard()->clearScreen();
    gameObject->start();
    sleep( 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 1 );
    score( gameObject, gameState, 1 );
    sleep( 1 );
    score( gameObject, gameState, 1 );
}

/*
 * Runs a demo with delays until the score reaches 3 - 2
 * Then kicks into high gear and finishes the game at 6 - 2 
 */
void test_02( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->start();
    sleep( 1 );
    #define DEMO_DELAY 1
    playerWinDelay( gameObject, gameState, 1, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 1, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 2, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 2, DEMO_DELAY );
    playerWinDelay( gameObject, gameState, 1, DEMO_DELAY );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    sleep( 2 );
}

void test_03( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->start();
    sleep( 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
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
    gameObject->getScoreBoard()->update();
    score( gameObject, gameState, 2 );
    GameTimer::gameDelay( 2000 );
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
    sleep( 6 ); }

void test_05( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getPlayer1()->setSetHistory( 1, 6 );
    gameObject->getPlayer2()->setSetHistory( 1, 4 );
    gameObject->getPlayer1()->setGames( 4 );
    gameObject->getPlayer2()->setGames( 5 );
    gameState->setCurrentSet( 2 );
    gameObject->getPlayer1()->setPoints( 3 );
    gameObject->getPlayer2()->setPoints( 2 );
    gameObject->getScoreBoard()->update();

}

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

void resetAll( Reset* reset ) {
    reset->zeroPlayerValues();
    reset->zeroSetHistory();
    reset->resetScoreboard();
}

void run_manual_game( GameObject* gameObject, GameState* gameState, Reset* reset, int player ) {
    int loop_count = 0;
    int test_count = 0;
    
    // set games to --games argument
    // set sets to --sets argument
    // set player1 score to --player1 argument
    // set player2 score to --player2 argument

    gameObject->loopGame();

    // std::cout << "done calling loopGame().  sleeping...\n" << std::endl;
    sleep( 1 );

    int menu_selection = 1;
    std::signal( SIGINT, GameObject::_signalHandler );
    
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/  
        if ( loop_count >  MAX_LOOP_COUNT ) { gameState->stopGameRunning(); }
        sleep( SCORE_DELAY );
        std::cout << "\n";
        std::cout << "  1.) Player 1 score" << std::endl;
        std::cout << "  2.) Player 2 score" << std::endl;
        std::cout << "  11.) Player 1 win" << std::endl;
        std::cout << "  22.) Player 2 win" << std::endl;
        std::cout << "  9.) Undo" << std::endl;
        std::cout << "  101.) Test 01" << std::endl;
        std::cout << "  102.) Test 02" << std::endl;
        std::cout << "  103.) Test 03" << std::endl;
        std::cout << "  104.) Test 04" << std::endl;
        std::cout << "  105.) Test 05" << std::endl;

        std::cout << "  0.) Exit" << std::endl;
        // cout
        std::cout << "  Enter selection: ";
        std::cin >> menu_selection;
        
        if ( menu_selection == 1  ||  menu_selection == 2 ) {
            std::cout << "\n\n\n\n\n\n\n*** Player " << menu_selection << " scored ***\n" << std::endl;
            gameObject->playerScore( menu_selection );  // flip the player score flag
            sleep( SCORE_DELAY );
        } else if ( menu_selection == 0 ) {
            std::cout << "*** Exiting... ***\n" << std::endl;
            exit( 0 );
        } else if ( menu_selection == 9 ) { 
            std::cout << "\n\n\n\n\n\n\n*** Undo ***\n" << std::endl;
            gameObject->undo(); 
            sleep( SCORE_DELAY );
        } else if ( menu_selection == 11 ) { 
            std::cout << "\n\n\n\n\n\n\n*** Player 1 win ***\n" << std::endl;
            playerWin( gameObject, gameState, 1 );
            sleep( SCORE_DELAY );
        } else if ( menu_selection == 22 ) { 
            std::cout << "\n\n\n\n\n\n\n*** Player 2 win ***\n" << std::endl;
            playerWin( gameObject, gameState, 2 );
            sleep( SCORE_DELAY );
        } else if ( menu_selection == 101 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 01 ***\n" << std::endl;
            gameObject->getScoreBoard()->clearScreen();
        gameObject->getScoreBoard()->drawText( "Test",  
        YELLOW, X__POS, Y__POS );
        gameObject->getScoreBoard()->drawText( " 01 ",  
        YELLOW, X__POSITION, Y__POSITION );
        GameTimer::gameDelay( 4000 );
            test_01( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        } else if ( menu_selection == 102 ) { 
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 02 ***\n" << std::endl;
            test_02( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        } else if ( menu_selection == 103 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 03 ***\n" << std::endl;
            test_03( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        } else if ( menu_selection == 104 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 04 ***\n" << std::endl;
            test_04( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        } else if ( menu_selection == 105 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 05 ***\n" << std::endl;
            test_05( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        } else {
            std::cout << "\n\n\n\n\n\n\n*** Invalid selection ***\n" << std::endl;
            sleep( SCORE_DELAY );
            continue; }
        
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

    int loop_count = 0;
    GameState*  gameState  = new GameState();  // make this 1st!!! cost me 3 days of debugging
    GameObject* gameObject = new GameObject( gameState );
    Reset* reset = new Reset( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );

    sleep( .5 );
    gameObject->loopGame();
    sleep( .5 );
    if ( manual == 1 ) { run_manual_game( gameObject, gameState, reset, 1 ); return 0; }
    
    ////// demo tests /////
    // gameObject->getScoreBoard()->clearScreen();
    // gameObject->getScoreBoard()->drawText( "Demo", YELLOW, X__POS, Y__POS );
    // gameObject->getScoreBoard()->drawText( "Test", YELLOW, X__POSITION, Y__POSITION );
    // GameTimer::gameDelay( 4000 );
    // demo_test( gameObject, gameState, &loop_count );

    //////////////// exit after demo for now ////////////////
    // exit( 0 );



    ///// run tests /////
    int test_count = 1;
    // gameObject->getScoreBoard()->drawText( "Win",  YELLOW, 18, 80  );
    // gameObject->getScoreBoard()->clearScreen();
    // gameObject->getScoreBoard()->drawText( "Test",  
    // YELLOW, X__POS, Y__POS );
    // gameObject->getScoreBoard()->drawText( " 01 ",  
    // YELLOW, X__POSITION, Y__POSITION );
    // GameTimer::gameDelay( 3000 );
    // test_01( gameObject, gameState, &loop_count );
    test_count++;
    // end test_01

    // gameObject->getScoreBoard()->clearScreen();
    // gameObject->getScoreBoard()->drawText( "Test",  
    // YELLOW, X__POS, Y__POS );
    // gameObject->getScoreBoard()->drawText( " 02 ",  
    // YELLOW, X__POSITION, Y__POSITION );
    // GameTimer::gameDelay( 4000 );
    // std::cout << "calling test_02()..." << std::endl;
    // test_02( gameObject, gameState, &loop_count );
    test_count++;
    // end test_02
    
    // gameObject->getScoreBoard()->clearScreen();
    // gameObject->getScoreBoard()->drawText( "Test",  
    // YELLOW, X__POS, Y__POS );
    // gameObject->getScoreBoard()->drawText( " 03 ",  
    // YELLOW, X__POSITION, Y__POSITION );
    // GameTimer::gameDelay( 4000 );
    // std::cout << "calling test_03()..." << std::endl;
    // test_03( gameObject, gameState, &loop_count );
    test_count++;
    // end test_03

    // gameObject->getScoreBoard()->clearScreen();
    // gameObject->getScoreBoard()->drawText( "Test",  
    // YELLOW, X__POS, Y__POS );
    // gameObject->getScoreBoard()->drawText( " 04 ",  
    // YELLOW, X__POSITION, Y__POSITION );
    // GameTimer::gameDelay( 4000 );
    // std::cout << "calling test_04()..." << std::endl;
    // test_04( gameObject, gameState, &loop_count );
    // end test_04
    test_count++;

    // test 05
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getScoreBoard()->drawText( "Test",  
    YELLOW, X__POS, Y__POS );
    gameObject->getScoreBoard()->drawText( " 05 ",  
    YELLOW, X__POSITION, Y__POSITION );
    GameTimer::gameDelay( 4000 );
    std::cout << "calling test_05()..." << std::endl;
    test_05( gameObject, gameState, &loop_count );
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
