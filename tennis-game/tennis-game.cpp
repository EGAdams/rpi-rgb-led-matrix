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
#include "IInputWithTimer/IInputWithTimer.h"
#include "TennisConstants/TennisConstants.h"
#include "RemotePairingScreen/RemotePairingScreen.h"
#include "PairingBlinker/PairingBlinker.h"
#include "ScoreboardBlinker/ScoreboardBlinker.h"
#include "ConsoleDisplay/ConsoleDisplay.h"
#include "RemoteLocker/RemoteLocker.h"
#include "PinState/PinState.h"
#include "KeyboardInputWithTimer/KeyboardInputWithTimer.h"
#include "IInputWithTimer/IInputWithTimer.h"
#include "RemoteInputWithTimer/RemoteInputWithTimer.h"
#include "BlankBlinker/BlankBlinker.h"
#include "IGameInput/IGameInput.h"
#include "RemoteGameInput/RemoteGameInput.h"
#include "KeyboardGameInput/KeyboardGameInput.h"


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
    print( "done with match win test." );
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
    if ( !gameState->gameRunning()) { // Check match win condition and display the result
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
    print( "Enter the message to write: " );
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

    print( "Binary value: " << std::bitset<5>( bits ) << "\n" );
    print( "Decimal value: " << static_cast< int >( bits ) << "\n" );
}

///// Detect the expander /////
#define MCP23017_ADDRESS 0x20  // Default I2C address when A0, A1, A2 are tied to GND

// Function to detect the MCP23017
bool detectExpander() {
    int file;
    const char* i2c_device = "/dev/i2c-1";

    if ( ( file = open( i2c_device, O_RDWR )) < 0 ) {
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

    print( "MCP23017 detected at address 0x20!\n" );
    close( file );
    return true;
}

void run_manual_game( GameObject* gameObject, GameState* gameState, Reset* reset, int player ) {
    int KEYBOARD_TIMEOUT = 10000;
    int loop_count = 0;
    IInputWithTimer* inputWithTimer;
    gameObject->loopGame();
    sleep( 1 );
    int menu_selection = 1;
    Inputs* inputs = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    gameObject->getScoreBoard()->setLittleDrawerFont( "fonts/8x13B.bdf" );
    std::signal( SIGINT, GameObject::_signalHandler );
    RemotePairingScreen remotePairingScreen( gameObject->getScoreBoard());
    print( "constructing pairing blinker from run manual game" );
    PairingBlinker pairingBlinker( gameObject->getScoreBoard());  // Use PairingBlinker
    print( "finished constructing input with timer from run manual game" );
    bool is_on_pi = gameObject->getScoreBoard()->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ));
    if ( is_on_pi ) {
        inputWithTimer = new RemoteInputWithTimer( &pairingBlinker, inputs );
    } else {
        inputWithTimer = new KeyboardInputWithTimer( &pairingBlinker, KEYBOARD_TIMEOUT );
    }
    while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        // print( "entered while loop from run manual game" );
        sleep( SCORE_DELAY );
        // if remote pairing, write the words.  if not, snap out of the loop
        while ( remotePairingScreen.inPairingMode() && is_on_pi && pairingBlinker.awake()) {  // 090724
            print( "inside remote pairing screen from run manual game.  before starting input timer..." ); // 122224
            
            print ( "SYSTEM IS RUNNING IN MANUAL MODE.  Please enter 1 for green and 2 for red." );
            std::cin >> menu_selection;
            // int menu_selection = inputWithTimer->getInput(); // 12224
            
            if ( menu_selection == 1 ) {
                remotePairingScreen.greenPlayerPressed();
                pairingBlinker.setGreenPlayerPaired( true );  // Notify blinker that Green player is paired
            }
            else if ( menu_selection == 2 ) {
                remotePairingScreen.redPlayerPressed();
                pairingBlinker.setRedPlayerPaired( true );  // Notify blinker that Red player is paired
            }
            else {
                print( "*** Invalid selection during remote pairing. ***\n" );
                GameTimer::gameDelay( 1000 );
            }
        }

        // print( "put in sleep mode if the pairing blinker is not awake. " );
        if ( !pairingBlinker.awake()) {
            print( "pairing blinker is not awake, stopping it... " );
                pairingBlinker.stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        // pairingBlinker.stop();  // Stop blinking once both players are paired
        print( "1.) green score             76. seven six Tie Breaker test" );
        print( "2.) red score                     " );
        print( "3.) Demo           " );
        print( "4.) Match Win Test " );
        // print( "5.) Test 05        " );
        print( "6.) Match Win Tie Break Test" );
        // print( "7.) Sleep Mode Test" );
        // print( "8.) Font File" );
        print( "9.) Undo           " );
        // print( "10.) Test Font     " );
        // print( "20.) Write Text    " );
        // print( "104.) Detect Expndr" );
        // print( "105.) read bits.   " );

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            ScoreboardBlinker sleepingBlinker( gameObject->getScoreBoard());
            if ( is_on_pi ) {
                inputWithTimer = new RemoteInputWithTimer( &sleepingBlinker, inputs );
            } else {
                inputWithTimer = new KeyboardInputWithTimer( &sleepingBlinker, KEYBOARD_TIMEOUT );
            }
            menu_selection = inputWithTimer->getInput();
            gameState->setCurrentAction( AFTER_SLEEP_MODE ); // stop sleep mode
            print( "time slept: " << inputWithTimer->getTimeSlept());
            if ( menu_selection == 1 || menu_selection == 2 ) { // or sleep time expired...
                // if the pairing caused the sleep mode, just go back to pairing mode
                // if ( !pairingBlinker.awake()) {
                //     print( "pairing blinker is sleeping.  going back to pairing mode..." );
                //     gameState->setCurrentAction( NORMAL_GAME_STATE );
                //     pairingBlinker.sleepModeOff();  // wake up the pairing blinker
                //     continue;
                // }
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( inputWithTimer->getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " << inputWithTimer->getTimeSlept() / 1000 << " seconds." );
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
            print( "Enter the message to write: " );
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
            print( "*** Exiting... ***\n" );
            exit( 0 );
        }
        else if ( menu_selection == 8 ) {
            // get font file from user
            std::string font_file;
            print( "Enter the path to the font file: " );
            std::getline( std::cin, font_file );  // get input from the user

            // Check if file exists
            std::ifstream file_check( font_file );
            if ( !file_check ) {
                std::cerr << "Warning: The specified font file does not exist.\n";
                return;  // Continue with the program flow without setting the font file
            }

            // If the file exists, set the font file
            gameObject->getScoreBoard()->setFontFile( font_file.c_str());
        }
        else if ( menu_selection == 9 ) {
            print( "\n\n\n\n\n\n\n*** Undo ***\n" );
            gameObject->undo(); // day before election day 110424
            gameState->setCurrentAction( NORMAL_GAME_STATE );
            sleep( SCORE_DELAY );
        }
        else if ( menu_selection == 11 ) {
            print( "\n\n\n\n\n\n\n*** Player 1 win ***\n" );
            playerWin( gameObject, gameState, 1 );
            sleep( SCORE_DELAY );
        }
        else if ( menu_selection == 22 ) {
            print( "\n\n\n\n\n\n\n*** Player 2 win ***\n" );
            playerWin( gameObject, gameState, 2 );
            sleep( SCORE_DELAY );
        }
        else if ( menu_selection == 101 ) {
            resetAll( reset );
            print( "\n\n\n\n\n\n\n*** Test 01 ***\n" );
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
            print( "\n\n\n\n\n\n\n*** Demo ***\n" );
            demo_test( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }
        else if ( menu_selection == 103 ) {
            resetAll( reset );
            print( "\n\n\n\n\n\n\n*** Test 03 ***\n" );
            test_03( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;

        }
        else if ( menu_selection == 104 ) {
            print( "\n*** Running MCP23017 Expander Detection Test ***\n" );
            detectExpander();
            sleep( SCORE_DELAY );
            continue;

        }
        else if ( menu_selection == 105 ) {
            print( "NOT reading MCP23017 bits..." );
        }
        else if ( menu_selection == 5 ) {
            resetAll( reset );
            print( "\n\n\n\n\n\n\n*** Test 05 ***\n" );
            test_05( gameObject, gameState, &loop_count );
            sleep( SCORE_DELAY );
            continue;
        }

        else if ( menu_selection == 6 ) {
            resetAll( reset );
            print( "\n\n\n\n\n\n\n*** Match Win Tie Break Test ***\n" );
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
            print( "\n\n\n\n\n\n\n*** Invalid selection ***\n" );
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
    if ( file.is_open()) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}

// link to Refactor Plan
// https://chatgpt.com/share/678b45b2-1cf8-8006-b036-fb14c0da8dd9

//// method to sit and listen to remote commands.
/************************************************************
 *  Example Refactoring to "Program to Interfaces" in C++
 *
 *  NOTE:
 *  - This single code block shows how you might refactor
 *    everything so that usage of concrete classes is hidden
 *    behind interfaces. In real practice, you would split
 *    these out into separate header/implementation files.
 *  - The run_remote_listener() function at the bottom
 *    retains the same structure and spacing as the original
 *    code. It just uses interface pointers instead of
 *    concrete class instances directly.
 *  - Where functionality was unclear or not shown, minimal
 *    "stub" implementations are given so that you can see
 *    how each interface might be implemented in principle.
 *    You can expand these stubs as needed.
 ************************************************************/

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <csignal>
#include <thread>
#include <chrono>

/*==========================================================
 *  Constants & Utilities (stubs to keep code self-contained)
 *==========================================================*/
// static const int GREEN_REMOTE_GREEN_SCORE = 1;
// static const int GREEN_REMOTE_RED_SCORE   = 2;
// static const int RED_REMOTE_GREEN_SCORE   = 3;
// static const int RED_REMOTE_RED_SCORE     = 4;
// static const int GREEN_REMOTE_UNDO        = 5;
// static const int RED_REMOTE_UNDO          = 6;

// static const int REMOTE_INPUT = 0;  // 0 => local input, 1 => remote input

// static const int SLEEP_MODE      = 100; 
// static const int AFTER_SLEEP_MODE= 101;
// static const int MAX_SLEEP       = 60;   // in seconds
// static const int SCORE_DELAY     = 1;    // in seconds

// Simple print-like function to keep the style of the original
// void print( const std::string &msg ) {
//     print( msg );
// }

/*==========================================================
 *  Interface Declarations
 *==========================================================*/
class IGameState {
public:
    virtual ~IGameState() = default;
    virtual bool gameRunning() const = 0;
    virtual int  getCurrentAction() const = 0;
    virtual void setCurrentAction( int action ) = 0;
    virtual std::map<int, int> getPlayer1SetHistory() const = 0;
    virtual std::map<int, int> getPlayer2SetHistory() const = 0;
};


/*==========================================================
 *  Global signal variable to mimic the code's usage
 *==========================================================*/
static volatile std::sig_atomic_t gSignalStatus = 0;

/*==========================================================
 *  The Refactored run_remote_listener Function
 *
 *  Now uses only interface pointers and does not directly
 *  construct or call concrete implementations in-line.
 *==========================================================*/
void run_remote_listener( GameObject* gameObject, GameState* gameStatearg, Reset* reset ) {
    int KEYBOARD_TIMEOUT = 120000;
    GameState* gameState = gameStatearg;
    RemoteLocker*       remoteLocker = new RemoteLocker( gameState );
    IInputWithTimer*    pairingInputWithTimer;
    IInputWithTimer*    noBlinkInputWithTimer;
    IInputWithTimer*    sleepingInputWithTimer;
    IGameInput*         gameInput;

    Inputs*             inputs = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    bool no_score = true;
    print( "entered run remote listener method..." );
    int loop_count = 0;
    int selection = 0;
    print( "calling game object loop game..." );
    gameObject->loopGame();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ));
    

    // Program to the IScoreBoard interface
    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont( "fonts/8x13B.bdf" );

    std::signal( SIGINT, GameObject::_signalHandler );

    print ( " constructing blinkers... " );
    RemotePairingScreen*    remotePairingScreen = new RemotePairingScreen( scoreboard );
    PairingBlinker*         pairingBlinker      = new PairingBlinker( scoreboard );
    BlankBlinker*           blankBlinker        = new BlankBlinker();
    ScoreboardBlinker*      sleepingBlinker     = new ScoreboardBlinker( scoreboard );

    unsigned long pairing_timer   = 4000;
    unsigned long no_blink_timer  = 4000;
    unsigned long sleeping_timer  = 4000;

    bool is_on_pi = scoreboard->onRaspberryPi();
    // checking pairing blinker for null state
    if ( pairingBlinker == nullptr ) {
        print( "*** ERROR: pairingBlinker is null! ***" );
    } else {
        print( "*** pairingBlinker is not null, continuing... ***" );
    }
    print( "is_on_pi: " + std::to_string( is_on_pi ));
    if ( is_on_pi ) {
        pairingInputWithTimer       = new RemoteInputWithTimer( pairingBlinker, inputs, pairing_timer   );
        noBlinkInputWithTimer       = new RemoteInputWithTimer( blankBlinker, inputs,   no_blink_timer  );
        sleepingInputWithTimer      = new RemoteInputWithTimer( sleepingBlinker, inputs, sleeping_timer );
        gameInput                   = new RemoteGameInput( inputs );
    } else {
        pairingInputWithTimer       = new KeyboardInputWithTimer( pairingBlinker, KEYBOARD_TIMEOUT      );
        noBlinkInputWithTimer       = new KeyboardInputWithTimer( blankBlinker, KEYBOARD_TIMEOUT        );
        sleepingInputWithTimer      = new KeyboardInputWithTimer( sleepingBlinker, KEYBOARD_TIMEOUT     );
        gameInput                   = new KeyboardGameInput(                         );     
    }
    while ( gameState->gameRunning() && gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));

        // if remote pairing, write the words.  if not, snap out of the loop
        print( "in pairing mode? " + std::to_string( remotePairingScreen->inPairingMode()));
        print( "pairingBlinker->awake(): " + std::to_string( pairingBlinker->awake()));
        while ( remotePairingScreen->inPairingMode() && pairingBlinker->awake()) {
            print( "inside remote pairing screen from run remote listener.  before starting input timer..." );
            selection = pairingInputWithTimer->getInput();
            if ( selection == GREEN_REMOTE_GREEN_SCORE ) {
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired( true );
            } else if ( selection == RED_REMOTE_RED_SCORE ) {
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired( true );
            } else {
                print( "*** Invalid selection during pairing. *** 012525\n\n\n" );
                GameTimer::gameDelay( 1000 );
            }
        }

        if ( !pairingBlinker->awake()) {
            print( "pairing blinker is not awake, stopping it... " );
            pairingBlinker->stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            sleepingInputWithTimer->getInput();                 // blocks here for so many seconds
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "time slept: " << sleepingInputWithTimer->getTimeSlept());

            if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == GREEN_REMOTE_RED_SCORE ) {
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( sleepingInputWithTimer->getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " + std::to_string( sleepingInputWithTimer->getTimeSlept() / 1000 ) + " seconds." );
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
            scoreboard->clearScreen();
            print( "cleared screen." );
            scoreboard->update();
            print( "updated scoreboard." );
        }                       // else NOT in SLEEP_MODE

        // we still need to set the time for a no score mode
        if ( no_score ) {   // if there is no score, we need to time the input for the Zero score timeout.
            selection = noBlinkInputWithTimer->getInput();
            print( "selection: " + std::to_string( selection ));

            /* CHECK FOR TIMEOUT */
            // if timed out, selection is the timeout value, not a valid game input.  the timeout value is no where near a valid game input.    
            if ( selection > 1000 ) {                       // this is a Zero score timeout
                print( "*** Zero ScoreTimeout!  going to sleep mode... ***" );
                gameState->setCurrentAction( SLEEP_MODE );  // set to sleep mode and go to the beginning
                continue;
            }
        }

        if ( no_score ) { 
            print( "setting no_score to false..." ); no_score = false; // no need for the zero score timer anymore
        } else {
            selection = gameInput->getInput();  // this is either cin << or a remote call, both block until input is made
        }                                       // there are no timers in regular game play at the time of this writing
                                                // January 25, 2025  mass deportation began.  it was very cold in FL last night.
        if ( selection == 0 ) {  // not sure if we need this here, but it won't hurt for now - 012525
            print( "\n\nselection: " + std::to_string( selection ) + " ***\n\n" );
            print( "*** Invalid selection! ***  continuing..." );
            continue;
        }

        int serve_flag = remoteLocker->playerNotServing( selection );
        print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
        if ( serve_flag ) {
            print( "*** Warning: player not serving! ***" );
            continue;  // do not want to kill the no score flag quite yet
        }

        print( "setting player button to selection: " + std::to_string( selection ) + " before calling loopGame()..." );
        if ( selection == GREEN_REMOTE_GREEN_SCORE ||
             selection == GREEN_REMOTE_RED_SCORE   ||
             selection == RED_REMOTE_GREEN_SCORE   ||
             selection == RED_REMOTE_RED_SCORE ) {
            if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE ) {
                print( "*** \n\n\nGreen player scored ***\n\n\n" ); 
                selection = 1; // represent GREEN
            } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE ) {
                print( "\n\n\n*** Red player scored ***\n\n\n" );
                selection = 2; // represent RED
            }
            gameObject->playerScore( selection );
        }
        else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
            print( "\n\n\n*** Undo ***\n\n\n" );
            gameObject->undo();
        }
        else {
            print( "\n\n\n*** Invalid selection ***\n\n\n" );
            print( "------------" );
            print( "GREEN REMOTE: " );
            print( "   green remote green score: " <<  GREEN_REMOTE_GREEN_SCORE );
            print( "or green remote, red score: " << GREEN_REMOTE_RED_SCORE );
            print( " ------------ \n");
            print( "RED REMOTE: " );
            print( "or red remote, green score: " << RED_REMOTE_GREEN_SCORE );
            print( "or red remote, red score: " << RED_REMOTE_RED_SCORE );
            print( " ------------ \n");
            std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
            continue;
        }

        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
        gameObject->loopGame();  // handle the player score flag
        loop_count++;

        // Just to mimic the original code's retrieval of set history
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////
}

int main( int argc, char* argv[] ) {
    std::unique_ptr<MonitoredObject> logger = LoggerFactory::createLogger( "TestLogger" );
    // int mode = 0; // used to compile the --manual option.  not used as of 122924
    if ( argc > 1 ) {
        std::string arg1 = argv[1];
        if ( arg1 == "--manual" ) {
            print( "running menu mode..." );
            // mode = 1;
        }
        else {
            print( "running remote listening mode..." );
        }
    }
    print( "creating game state object..." );
    GameState* gameState = new GameState();  // make this 1st!!! cost me 3 days
    print( "creating game object..." );
    // FontManager* fontManager = new FontManager();
    ColorManager* colorManager = new ColorManager();
    bool isOnPi = is_on_raspberry_pi();
    print( "isOnPi: " << isOnPi );
    IDisplay* display = new ConsoleDisplay( colorManager );
    if ( isOnPi ) {
        print( "creating display object with matrix display..." );
    }
    else {
        print( "creating display object with console display..." );
        display = new ConsoleDisplay( colorManager );
    }
    GameObject* gameObject = new GameObject( gameState, display );
    print( "creating reset object..." );
    Reset* reset = new Reset( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );

    if ( REMOTE_INPUT == 0 ) {
        print( "running manual game..." );
        run_manual_game( gameObject, gameState, reset, 1 ); return 0;
    }
    else if ( REMOTE_INPUT == 1 ) {
        print( "running game from remote inputs..." );
        run_remote_listener( gameObject, gameState, reset ); return 0;
    }
    else {
        print( "*** ERROR: unknown mode ***" );
    }
}

// skoolyaad - Alone  // mix this with marvin G.
