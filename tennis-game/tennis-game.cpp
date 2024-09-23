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
#include "InputWithTimer/InputWithTimer.h"
#include "TennisConstants/TennisConstants.h"
#include "RemotePairingScreen/RemotePairingScreen.h"
#include "PairingBlinker/PairingBlinker.h"
#include "ScoreboardBlinker/ScoreboardBlinker.h"
#include "../../../../usr/include/c++/13/bits/fs_fwd.h"

using namespace rgb_matrix;
#define SCORE_DELAY    0
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
    if ( gameState->getCurrentAction() == SLEEP_MODE ) {
        print( "*** WARNING: may need to return early for sleep mode. *** " );
    }    
    std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
    std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
}

void playerWinDelay( GameObject* gameObject, GameState* gameState, int player, int delay ) {
    scoreDelay( gameObject, gameState, player, delay );
    while ( gameState->getPlayer1Points() != 0 || gameState->getPlayer2Points() != 0 ) {
        scoreDelay( gameObject, gameState, player, delay );
    }
}

void playerWin( GameObject* gameObject, GameState* gameState, int player ) {
    if ( gameState->getCurrentAction() == SLEEP_MODE ) {
        print( "*** Sleep mode detected.. returning. ***" );
        return;
    }
    score( gameObject, gameState, player );
    if ( gameState->getCurrentAction() == SLEEP_MODE ) {
        print( "*** Sleep mode detected.. returning. ***" );
        return;
    }
    while ( gameState->getPlayer1Points() != 0 || gameState->getPlayer2Points() != 0 ) {
        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            print( "*** Sleep mode detected.. returning. ***" );
            return;
        }
        score( gameObject, gameState, player );
    }
}

void matchWinTieBreakerTest( GameObject* gameObject, GameState* gameState ) {
    for ( int x = 0; x < 6; x++ ) {
        playerWin( gameObject, gameState, 1 );
    }
    for ( int x = 0; x < 5; x++ ) {
        playerWin( gameObject, gameState, 2 );
    }
}

void matchWinTest( GameObject* gameObject, GameState* gameState ) {
    for ( int x = 0; x < 11; x++ ) {
        playerWin( gameObject, gameState, 1 );
    }
    playerWin( gameObject, gameState, 1 );
    std::cout << "done with match win test." << std::endl;
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
    while ( gameState->getPlayer1Points() != 0 || gameState->getPlayer2Points() != 0 ) {
        scoreDelay( gameObject, gameState, 0, delay );
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
    while ( 1 ) {
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
        sleep( 5 );
    }
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
    sleep( 4 );
}

void test_04( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getPlayer1()->setPoints( 0 );
    gameState->setPlayer1Points( 0 );
    gameObject->getPlayer2()->setPoints( 2 );
    gameState->setPlayer2Points( 2 );
    gameObject->getPlayer1()->setGames( 6 );
    gameObject->getPlayer2()->setGames( 5 );
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
    sleep( 6 );
}

void test_05( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );

    // now for player 2 to win the set...
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );
    score( gameObject, gameState, 2 );

    // now player 1 win match
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
}


void test_06( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen(); // Initialize the game state
    gameObject->getPlayer1()->setPoints( 0 );
    gameState->setPlayer1Points( 0 );
    gameObject->getPlayer2()->setPoints( 0 );
    gameState->setPlayer2Points( 0 );
    gameObject->getPlayer1()->setGames( 5 );
    gameObject->getPlayer2()->setGames( 4 );
    playerWin( gameObject, gameState, 1 ); // Player 1 wins the first set
    gameObject->getPlayer1()->setPoints( 0 ); // Reset points for the next set
    gameState->setPlayer1Points( 0 );
    gameObject->getPlayer2()->setPoints( 0 );
    gameState->setPlayer2Points( 0 );
    gameObject->getPlayer1()->setGames( 5 );
    gameObject->getPlayer2()->setGames( 4 );
    playerWin( gameObject, gameState, 1 ); // Player 1 wins the second set and the match
    if ( !gameState->gameRunning() ) { // Check match win condition and display the result
    }
}

void resetAll( Reset* reset ) {
    reset->zeroPlayerValues();
    reset->zeroSetHistory();
    reset->resetScoreboard();
}

void run_manual_game( GameObject* gameObject, GameState* gameState, Reset* reset, int player ) {
    int loop_count = 0;
    gameObject->loopGame();
    sleep( 1 );
    int menu_selection = 1;
    int remote_pairing = 1;
    std::signal( SIGINT, GameObject::_signalHandler );
    RemotePairingScreen remotePairingScreen( gameObject->getScoreBoard());
    PairingBlinker pairingBlinker( gameObject->getScoreBoard());  // Use PairingBlinker
    InputWithTimer inputWithTimer( &pairingBlinker );  // Pass PairingBlinker
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        sleep( SCORE_DELAY );
        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen.inPairingMode()) {
            int menu_selection = inputWithTimer.getInput();
    
            if (menu_selection == 1) {
                remotePairingScreen.greenPlayerPressed();
                pairingBlinker.setGreenPlayerPaired(true);  // Notify blinker that Green player is paired
            } else if (menu_selection == 2) {
                remotePairingScreen.redPlayerPressed();
                pairingBlinker.setRedPlayerPaired(true);  // Notify blinker that Red player is paired
            } else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay(1000);
            }
        }
        pairingBlinker.stop();  // Stop blinking once both players are paired
        std::cout << "1.) green score    " << std::endl;
        std::cout << "2.) red score      " << std::endl;
        std::cout << "3.) Demo           " << std::endl;
        std::cout << "4.) Match Win Test " << std::endl;
        std::cout << "5.) Test 05        " << std::endl;
        std::cout << "6.) Match Win Tie Break Test" << std::endl;
        std::cout << "7.) Sleep Mode Test" << std::endl;
        std::cout << "8.) Font File"       << std::endl;
        std::cout << "9.) Undo           " << std::endl;


        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            ScoreboardBlinker blinker( gameObject->getScoreBoard() );
            InputWithTimer inputWithTimer( &blinker );
            menu_selection = inputWithTimer.getInput();
            gameState->setCurrentAction( AFTER_SLEEP_MODE ); // stop sleep mode
            std::cout << "time slept: " << inputWithTimer.getTimeSlept() << std::endl;
            if (  menu_selection == 1  || 
                  menu_selection == 2  || 
                 ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 )) {
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " << inputWithTimer.getTimeSlept() * 1000 << " seconds." << std::endl );
                    print( "clearing History because max sleep time has been reached or exceeded." );
                    gameObject->getHistory()->clearHistory();
                    print( "done clearing history because max sleep time has been reached or exceeded." );
                }
                continue;
            }
            print("setting game state current action to after sleep mode");
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "*** Going into last Match! ***" );
            print( "clearing screen..." );
            gameObject->getScoreBoard()->clearScreen();
            print( "cleared screen." );
            gameObject->getScoreBoard()->update();
            print( "updated scoreboard." );
        } else {
            std::cin >> menu_selection;
        }

        if (  menu_selection == 1  ||  menu_selection == 2  ) {
            gameObject->playerScore( menu_selection );  // flip the player score flag
            sleep( SCORE_DELAY );
        }
        else if (  menu_selection == 0  ) {
            std::cout << "*** Exiting... ***\n" << std::endl;
            exit( 0 );
        }
        else if (menu_selection == 8) {
            // get font file from user
            std::string font_file;
            std::cout << "Enter the path to the font file: ";
            std::getline(std::cin, font_file);  // get input from the user

            // Check if file exists
            std::ifstream file_check(font_file);
            if (!file_check) {
                std::cerr << "Warning: The specified font file does not exist.\n";
                return;  // Continue with the program flow without setting the font file
            }
            
            // If the file exists, set the font file
            gameObject->getScoreBoard()->setFontFile(font_file.c_str());
        }
        else if (  menu_selection == 9  ) {
            std::cout << "\n\n\n\n\n\n\n*** Undo ***\n" << std::endl;

            gameObject->undo();
            sleep( SCORE_DELAY );
        }
        else if (  menu_selection == 11 ) {
            std::cout << "\n\n\n\n\n\n\n*** Player 1 win ***\n" << std::endl;
            playerWin( gameObject, gameState, 1 );
            sleep( SCORE_DELAY );
        }
        else if (  menu_selection == 22 ) {
            std::cout << "\n\n\n\n\n\n\n*** Player 2 win ***\n" << std::endl;
            playerWin( gameObject, gameState, 2 );
            sleep( SCORE_DELAY );
        }
        else if (  menu_selection == 101 ) {
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
        }
        else if (  menu_selection == 3  ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Demo ***\n" << std::endl;
            demo_test( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }
        else if (  menu_selection == 103 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 03 ***\n" << std::endl;
            test_03( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }
        else if (  menu_selection == 104 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 04 ***\n" << std::endl;
            test_04( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }
        else if (  menu_selection == 5  ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 05 ***\n" << std::endl;
            test_05( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }
        else if (  menu_selection == 6  ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Match Win Tie Break Test ***\n" << std::endl;
            matchWinTieBreakerTest( gameObject, gameState );
            sleep( SCORE_DELAY );
            continue;
        }
        else if (  menu_selection == 4  ) {
            matchWinTest( gameObject, gameState );
            sleep( SCORE_DELAY );
            continue;

        }
        else if (  menu_selection == 7  ) {
            gameState->setCurrentAction( SLEEP_MODE );
            sleep( SCORE_DELAY );
            continue;
        }
        else {
            std::cout << "\n\n\n\n\n\n\n*** Invalid selection ***\n" << std::endl;
            sleep( SCORE_DELAY );
            continue;
        }
        gameObject->loopGame();  // handle the player score flag
        loop_count++;
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////


}

int main( int argc, char* argv[] ) {    std::unique_ptr<MonitoredObject> logger = LoggerFactory::createLogger( "TestLogger" );
    int manual = 0;
    if ( argc > 1 ) {
        std::string arg1 = argv[1];
        if ( arg1 == "--manual" ) {
            std::cout << "running manual tests..." << std::endl;
            manual = 1;
        }
    }
    manual = 1;  // yes hardcodeing  is bad but ina hurry!!
    int loop_count = 0;
    std::cout << "creating game state object..." << std::endl;
    GameState* gameState = new GameState();  // make this 1st!!! cost me 3 days
    std::cout << "creating game object..." << std::endl;
    GameObject* gameObject = new GameObject( gameState );
    std::cout << "creating reset object..." << std::endl;
    Reset* reset = new Reset( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    if ( manual == 1 ) {
        std::cout << "running manual game..." << std::endl;
        run_manual_game( gameObject, gameState, reset, 1 ); return 0;
    }
    int test_count = 1; ///// run tests /////
    test_count++;
    test_count++;
    test_count++;
    test_count++;
    gameObject->getScoreBoard()->clearScreen(); // test 05
    gameObject->getScoreBoard()->drawText( "Test",
    YELLOW, X__POS, Y__POS );
    gameObject->getScoreBoard()->drawText( " 05 ",
    YELLOW, X__POSITION, Y__POSITION );
    GameTimer::gameDelay( 4000 );
    std::cout << "calling test_05()..." << std::endl;
    test_05( gameObject, gameState, &loop_count );
    test_count++;
    GameTimer::gameDelay( 2000 );
    test_count++;
}




