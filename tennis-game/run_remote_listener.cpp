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

/*==========================================================
 *  Global signal variable to mimic the code's usage
 *==========================================================*/
static volatile std::sig_atomic_t gSignalStatus = 0;
#define SCORE_DELAY    0

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
        while ( remotePairingScreen->inPairingMode() && pairingBlinker->awake() ) {
            print( "inside remote pairing screen from run manual game.  before starting input timer..." );
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

        if ( !pairingBlinker->awake() ) {
            print( "pairing blinker is not awake, stopping it... " );
            pairingBlinker->stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            sleepingInputWithTimer->getInput();                 // blocks here for so many seconds
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "time slept: " << sleepingInputWithTimer->getTimeSlept() );

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
    run_remote_listener( gameObject, gameState, reset ); return 0;
}
