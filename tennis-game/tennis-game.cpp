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
#include "ITextDrawer/ITextDrawer.h"
#include "Drawer/Drawer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "GameObject/GameObject.h"
#include "LoggerFactory/LoggerFactory.h"
#include "InputWithTimer/InputWithTimer.h"
#include "TennisConstants/TennisConstants.h"
#include "RemotePairingScreen/RemotePairingScreen.h"
#include "PairingBlinker/PairingBlinker.h"
#include "ScoreboardBlinker/ScoreboardBlinker.h"
#include "ConsoleDisplay/ConsoleDisplay.h"


// for the expander
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

// to set the bits
#include <bitset>

using namespace rgb_matrix;
#define SCORE_DELAY    0
#define MAX_LOOP_COUNT 350
#define A_SPACE        13
#define FOUR_SPACE     14
#define THREE_SPACE    15
#define DEMO_DELAY 1
#define BLINK_UPDATE_DELAY 1000000 // 250000 //
#define FASTER_BLINK_UPDATE_DELAY 100000 // 750000 // 100000
#define X__POS 6
#define Y__POS 40
#define X__POSITION 5
#define Y__POSITION 80
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

void tieBreakerSevenSixTest( GameObject* gameObject, GameState* gameState ) {
    for ( int x = 0; x < 6; x++ ) {
        playerWin( gameObject, gameState, 1 );
    }
    for ( int x = 0; x < 5; x++ ) {
        playerWin( gameObject, gameState, 2 );
    }
    for ( int x = 0; x < 6; x++ ) {
        playerWin( gameObject, gameState, 1 );
    }

    playerWin( gameObject, gameState, 2 );
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
    while ( 1 ) {
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

        // sleep for a while...
        print( "Sleeping for 2 seconds...\n" );
        GameTimer::gameDelay( 2000 );
        print( "resetting match...\n" );
        GameTimer::gameDelay( 2000 );
        gameObject->resetMatch();
        print( "restarting loop...\n" );
    }
}

void seven_six_tb_test( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 1 );

    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
    playerWin( gameObject, gameState, 2 );
    playerWin( gameObject, gameState, 1 );
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

void get_and_set_font( GameObject* gameObject ) {
    std::string font_path = gameObject->getScoreBoard()->displayAndLoadFontMenu( "fonts" );
    print( "calling set little drawer font... " );
    gameObject->getScoreBoard()->setLittleDrawerFont( font_path );
    print( "done calling set little drawer font." );
    std::cout << "Enter the message to write: ";
    print( "setting message.... " );
    std::string message;
    message = "test";
    print( "clearing screen..." );
    gameObject->getScoreBoard()->clearScreen();
    print( "cleared screen." );
    gameObject->getScoreBoard()->drawNewText( message, 5, 20 );
    print( "done drawing new text." );
    GameTimer::gameDelay( 1000 );
    print( "continuing..." );
}


#include <bitset>

#define MCP23017_ADDRESS 0x20  // Default I2C address when A0, A1, A2 are tied to GND
#define I2C_DEVICE "/dev/i2c-1"
#define GPIO_REGISTER 0x12  // GPIOA register for reading pins

// Function to read bits 0 to 4 from the expander
void readBits( int file ) {
    uint8_t reg = GPIO_REGISTER;
    uint8_t value;

    if ( write( file, &reg, 1 ) != 1 ) {
        std::cerr << "Error: Unable to write to I2C device.\n";
        return;
    }

    if ( read( file, &value, 1 ) != 1 ) {
        std::cerr << "Error: Unable to read from I2C device.\n";
        return;
    }

    uint8_t bits = value & 0x1F;  // Mask bits 0-4

    std::cout << "Binary value: " << std::bitset<5>( bits ) << "\n";
    std::cout << "Decimal value: " << static_cast< int >( bits ) << "\n";
}

///// Detect the expander /////
#define MCP23017_ADDRESS 0x20  // Default I2C address when A0, A1, A2 are tied to GND

// Function to detect the MCP23017
bool detectExpander() {
    int file;
    const char* i2c_device = "/dev/i2c-1";

    if ( ( file = open( i2c_device, O_RDWR ) ) < 0 ) {
        std::cerr << "Error: Unable to open I2C device.\n";
        return false;
    }

    if ( ioctl( file, I2C_SLAVE, MCP23017_ADDRESS ) < 0 ) {
        std::cerr << "Error: Unable to set I2C address.\n";
        close( file );
        return false;
    }

    // Try reading a single byte from the MCP23017
    if ( write( file, nullptr, 0 ) < 0 ) {
        std::cerr << "MCP23017 not detected at address 0x20.\n";
        close( file );
        return false;
    }

    std::cout << "MCP23017 detected at address 0x20!\n";
    close( file );
    return true;
}

void run_manual_game( GameObject* gameObject, GameState* gameState, Reset* reset, int player ) {
    int loop_count = 0;
    gameObject->loopGame();
    sleep( 1 );
    int menu_selection = 1;
    Inputs* inputs = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    gameObject->getScoreBoard()->setLittleDrawerFont( "fonts/8x13B.bdf" );
    std::signal( SIGINT, GameObject::_signalHandler );
    RemotePairingScreen remotePairingScreen( gameObject->getScoreBoard() );
    print( "constructing pairing blinker from run manual game" );
    PairingBlinker pairingBlinker( gameObject->getScoreBoard() );  // Use PairingBlinker
    print( "constructing input with timer from run manual game" );
    InputWithTimer inputWithTimer( &pairingBlinker, inputs );  // Pass PairingBlinker
    print( "finished constructing input with timer from run manual game" );
    bool is_on_pi = gameObject->getScoreBoard()->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ) );
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        print( "entered while loop from run manual game" );
        sleep( SCORE_DELAY );
        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen.inPairingMode() && is_on_pi && pairingBlinker.awake() ) { // 090724
            print( "inside remote pairing screen from run manual game.  before starting input timer..." );
            int menu_selection = inputWithTimer.getInput();
            if ( menu_selection == 1 ) {
                remotePairingScreen.greenPlayerPressed();
                pairingBlinker.setGreenPlayerPaired( true );  // Notify blinker that Green player is paired
            }
            else if ( menu_selection == 2 ) {
                remotePairingScreen.redPlayerPressed();
                pairingBlinker.setRedPlayerPaired( true );  // Notify blinker that Red player is paired
            }
            else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay( 1000 );
            }
        }

        print( "put in sleep mode if the pairing blinker is not awake. " );
        if ( !pairingBlinker.awake() ) {
            print( "pairing blinker is not awake, stopping it... " );
                pairingBlinker.stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        // pairingBlinker.stop();  // Stop blinking once both players are paired
        std::cout << "1.) green score             76. seven six Tie Breaker test" << std::endl;
        std::cout << "2.) red score                     " << std::endl;
        std::cout << "3.) Demo           " << std::endl;
        std::cout << "4.) Match Win Test " << std::endl;
        std::cout << "5.) Test 05        " << std::endl;
        std::cout << "6.) Match Win Tie Break Test" << std::endl;
        std::cout << "7.) Sleep Mode Test" << std::endl;
        std::cout << "8.) Font File" << std::endl;
        std::cout << "9.) Undo           " << std::endl;
        std::cout << "10.) Test Font     " << std::endl;
        std::cout << "20.) Write Text    " << std::endl;
        std::cout << "104.) Detect Expndr" << std::endl;
        std::cout << "105.) read bits.   " << std::endl;

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            ScoreboardBlinker blinker( gameObject->getScoreBoard() );
            InputWithTimer inputWithTimer( &blinker, inputs );
            menu_selection = inputWithTimer.getInput();
            gameState->setCurrentAction( AFTER_SLEEP_MODE ); // stop sleep mode
            std::cout << "time slept: " << inputWithTimer.getTimeSlept() << std::endl;
            if ( menu_selection == 1 ||
                  menu_selection == 2 ||
                 ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 ) ) { // and sleep time expired...
                // if the pairing caused the sleep mode, just go back to pairing mode
                // if ( !pairingBlinker.awake() ) {
                //     print( "pairing blinker is sleeping.  going back to pairing mode..." );
                //     gameState->setCurrentAction( NORMAL_GAME_STATE );
                //     pairingBlinker.sleepModeOff();  // wake up the pairing blinker
                //     continue;
                // }
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " << inputWithTimer.getTimeSlept() / 1000 << " seconds." << std::endl );
                    print( "clearing History because max sleep time has been reached or exceeded." );
                    gameObject->getHistory()->clearHistory();
                    print( "done clearing history because max sleep time has been reached or exceeded." );
                }
                continue;
            }
            print( "setting game state current action to after sleep mode" );
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "*** Going into last Match! ***" );
            print( "clearing screen..." );
            gameObject->getScoreBoard()->clearScreen();
            print( "cleared screen." );
            gameObject->getScoreBoard()->update();
            print( "updated scoreboard." );
        }
        else {
            std::cin >> menu_selection;
        }

        if ( menu_selection == 20 ) {
            std::cout << "Enter the message to write: ";
            std::string message;
            // std::getline(std::cin, message);  // get input from the user // never works
            std::cin >> message;
            gameObject->getScoreBoard()->clearScreen();
            gameObject->getScoreBoard()->drawNewText( message, 5, 20 );
            GameTimer::gameDelay( 1000 );
            continue;
        }

        if ( menu_selection == 10 ) {
            get_and_set_font( gameObject );
            // pairingBlinker.enable();
            // remotePairingScreen.enablePairingMode();
            continue;
        }

        if ( menu_selection == 1 || menu_selection == 2 ) {
            gameObject->playerScore( menu_selection );  // flip the player score flag
            sleep( SCORE_DELAY );
        }
        else if ( menu_selection == 0 ) {
            std::cout << "*** Exiting... ***\n" << std::endl;
            exit( 0 );
        }
        else if ( menu_selection == 8 ) {
            // get font file from user
            std::string font_file;
            std::cout << "Enter the path to the font file: ";
            std::getline( std::cin, font_file );  // get input from the user

            // Check if file exists
            std::ifstream file_check( font_file );
            if ( !file_check ) {
                std::cerr << "Warning: The specified font file does not exist.\n";
                return;  // Continue with the program flow without setting the font file
            }

            // If the file exists, set the font file
            gameObject->getScoreBoard()->setFontFile( font_file.c_str() );
        }
        else if ( menu_selection == 9 ) {
            std::cout << "\n\n\n\n\n\n\n*** Undo ***\n" << std::endl;
            gameObject->undo(); // day before election day 110424
            gameState->setCurrentAction( NORMAL_GAME_STATE );
            sleep( SCORE_DELAY );
        }
        else if ( menu_selection == 11 ) {
            std::cout << "\n\n\n\n\n\n\n*** Player 1 win ***\n" << std::endl;
            playerWin( gameObject, gameState, 1 );
            sleep( SCORE_DELAY );
        }
        else if ( menu_selection == 22 ) {
            std::cout << "\n\n\n\n\n\n\n*** Player 2 win ***\n" << std::endl;
            playerWin( gameObject, gameState, 2 );
            sleep( SCORE_DELAY );
        }
        else if ( menu_selection == 101 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 01 ***\n" << std::endl;
            gameObject->getScoreBoard()->clearScreen();
            gameObject->getScoreBoard()->drawText( "Test", X__POS, Y__POS );
            gameObject->getScoreBoard()->drawText( " 01 ", X__POSITION, Y__POSITION );
            GameTimer::gameDelay( 4000 );
            test_01( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }
        else if ( menu_selection == 3 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Demo ***\n" << std::endl;
            demo_test( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }
        else if ( menu_selection == 103 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 03 ***\n" << std::endl;
            test_03( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;

        }
        else if ( menu_selection == 104 ) {
            std::cout << "\n*** Running MCP23017 Expander Detection Test ***\n";
            detectExpander();
            sleep( SCORE_DELAY );
            continue;

        }
        else if ( menu_selection == 105 ) {
            std::cout << "reading MCP23017 bits..." << std::endl;
            int file = open( I2C_DEVICE, O_RDWR );

            if ( file < 0 ) {
                std::cerr << "Error: Unable to open I2C device.\n";
                continue;
            }

            if ( ioctl( file, I2C_SLAVE, MCP23017_ADDRESS ) < 0 ) {
                std::cerr << "Error: Unable to set I2C address.\n";
                close( file );
                continue;
            }
            int is_one;
            while ( 1 ) {
                std::cin >> is_one;
                if ( is_one == 1 ) {
                    break;
                }
                else {
                    readBits( file );
                }
            }
            close( file );
            continue;
        }
        else if ( menu_selection == 5 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Test 05 ***\n" << std::endl;
            test_05( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }

        else if ( menu_selection == 6 ) {
            resetAll( reset );
            std::cout << "\n\n\n\n\n\n\n*** Match Win Tie Break Test ***\n" << std::endl;
            matchWinTieBreakerTest( gameObject, gameState );
            sleep( SCORE_DELAY );
            continue;
        }

        else if ( menu_selection == 4 ) {
            matchWinTest( gameObject, gameState );
            sleep( SCORE_DELAY );
            continue;

        }
        else if ( menu_selection == 7 ) {
            gameState->setCurrentAction( SLEEP_MODE );
            sleep( SCORE_DELAY );
            continue;
        }
        else if ( menu_selection == 76 ) {
            tieBreakerSevenSixTest( gameObject, gameState );
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

bool is_on_raspberry_pi() {
    std::ifstream file( "/proc/device-tree/model" );
    std::string line;
    if ( file.is_open() ) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}

//// method to sit and listen to remote commands.
void run_remote_listener( GameObject* gameObject, GameState* gameState, Reset* reset, Inputs* inputs ) {
    print( "entered run remote listener method..." );
    int loop_count = 0;
    int selection = 0;
    print( "calling game object loop game..." );
    gameObject->loopGame();
    sleep( 1 );
    gameObject->getScoreBoard()->setLittleDrawerFont( "fonts/8x13B.bdf" );
    std::signal( SIGINT, GameObject::_signalHandler );
    print (" constructing remote pairing screen from inside run remote listener method... " );
    RemotePairingScreen remotePairingScreen( gameObject->getScoreBoard() );
    print( "constructing pairing blinker from run manual game" );
    PairingBlinker pairingBlinker( gameObject->getScoreBoard() );  // Use PairingBlinker
    print( "constructing input with timer from run manual game" );
    InputWithTimer inputWithTimer( &pairingBlinker, inputs );  // Pass PairingBlinker
    print( "finished constructing input with timer from run manual game" );
    bool is_on_pi = gameObject->getScoreBoard()->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ) );
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        print( "entered while loop from run manual game" );
        sleep( SCORE_DELAY );
        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen.inPairingMode() && is_on_pi && pairingBlinker.awake() ) { // 090724
            print( "inside remote pairing screen from run manual game.  before starting input timer..." );
            selection = inputWithTimer.getInput();
            print( "selection: " << selection );
            if ( selection == 7 ) {
                remotePairingScreen.greenPlayerPressed();
                pairingBlinker.setGreenPlayerPaired( true );  // Notify blinker that Green player is paired
            }
            else if ( selection == 11 ) {
                remotePairingScreen.redPlayerPressed();
                pairingBlinker.setRedPlayerPaired( true );  // Notify blinker that Red player is paired
            }
            else {
                std::cout << "*** Invalid selection during remote pairing. ***\n";
                GameTimer::gameDelay( 1000 );
            }
        }

        print( "put in sleep mode if the pairing blinker is not awake. " );
        if ( !pairingBlinker.awake() ) {
            print( "pairing blinker is not awake, stopping it... " );
                pairingBlinker.stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            ScoreboardBlinker blinker( gameObject->getScoreBoard() );
            InputWithTimer inputWithTimer( &blinker, inputs );
            int selection = inputWithTimer.getInput();
            gameState->setCurrentAction( AFTER_SLEEP_MODE ); // stop sleep mode
            std::cout << "time slept: " << inputWithTimer.getTimeSlept() << std::endl;
            if ( selection == 7 ||
                  selection == 11 ||
                 ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 ) ) { // and sleep time expired...
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( inputWithTimer.getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " << inputWithTimer.getTimeSlept() / 1000 << " seconds." << std::endl );
                    print( "clearing History because max sleep time has been reached or exceeded." );
                    gameObject->getHistory()->clearHistory();
                    print( "done clearing history because max sleep time has been reached or exceeded." );
                }
                continue;
            }
            print( "setting game state current action to after sleep mode" );
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "*** Going into last Match! ***" );
            print( "clearing screen..." );
            gameObject->getScoreBoard()->clearScreen();
            print( "cleared screen." );
            gameObject->getScoreBoard()->update();
            print( "updated scoreboard." );
        } else {
            std::cin >> selection;
        }

        if ( selection == 7 || selection == 11 ) {
            if ( selection == 7 ) {
                selection = 1;
            } else if ( selection == 11 ) {
                selection = 2;
            }
            gameObject->playerScore( selection );  // flip the player score flag
            sleep( SCORE_DELAY );
        } else {
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

int main( int argc, char* argv[] ) {
    std::unique_ptr<MonitoredObject> logger = LoggerFactory::createLogger( "TestLogger" );
    int mode = 2;           // set menu or read remote mode
    if ( argc > 1 ) {
        std::string arg1 = argv[1];
        if ( arg1 == "--manual" ) {
            std::cout << "running menu mode..." << std::endl;
            mode = 1;
        }
        else {
            std::cout << "running remote listening mode..." << std::endl;
        }
    }
    int loop_count = 0;
    std::cout << "creating game state object..." << std::endl;
    GameState* gameState = new GameState();  // make this 1st!!! cost me 3 days
    std::cout << "creating game object..." << std::endl;
    // FontManager* fontManager = new FontManager();
    ColorManager* colorManager = new ColorManager();
    bool isOnPi = is_on_raspberry_pi();
    print( "isOnPi: " << isOnPi );
    IDisplay* display = new ConsoleDisplay( colorManager );
    if ( isOnPi ) {
        std::cout << "creating display object with matrix display..." << std::endl;
    }
    else {
        std::cout << "creating display object with console display..." << std::endl;
        display = new ConsoleDisplay( colorManager );
    }
    GameObject* gameObject = new GameObject( gameState, display );
    std::cout << "creating reset object..." << std::endl;
    Reset* reset = new Reset( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    Inputs* inputs = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    if ( mode == 1 ) {
        std::cout << "running manual game..." << std::endl;
        run_manual_game( gameObject, gameState, reset, 1 ); return 0;
    }
    else if ( mode == 2 ) {
        std::cout << "running game from remote inputs..." << std::endl;
        run_remote_listener( gameObject, gameState, reset, inputs ); return 0;
    }
    else {
        std::cout << "*** ERROR: unknown mode ***" << std::endl;
    }
    int test_count = 1; ///// run tests /////
    test_count++;
    test_count++;
    test_count++;
    test_count++;
    gameObject->getScoreBoard()->clearScreen(); // test 05
    gameObject->getScoreBoard()->drawText( "Test", X__POS, Y__POS );
    gameObject->getScoreBoard()->drawText( " 05 ", X__POSITION, Y__POSITION );
    GameTimer::gameDelay( 4000 );
    std::cout << "calling test_05()..." << std::endl;
    test_05( gameObject, gameState, &loop_count );
    test_count++;
    GameTimer::gameDelay( 2000 );
    test_count++;
}
