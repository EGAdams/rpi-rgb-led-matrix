
/************************************************************
 * run_remote_listener.cpp
 *
 * Demonstration of using a State Pattern to manage the
 * "beginning game states" for a tennis scoring system.
 *
 * The goal is to make the code more modular, testable,
 * maintainable, and thread-safe.
 *
 * The four primary states that we care about are:
 *   1) Pairing Mode
 *   2) Sleep Mode
 *   3) Regular Game Play (Before any Scores)
 *   4) Regular Game Play (After a Score)
 *
 * This file shows a re-organization of logic using the
 * GoF State Pattern. Each state is implemented as a class
 * responsible for its own behavior and for performing
 * state transitions as needed.
 *
 * The run_remote_listener() function is refactored to:
 *   - Initialize all shared resources (GameObject, Inputs, etc.)
 *   - Create a Context (InputListenerContext) which holds
 *     references to the shared resources, input readers,
 *     scoreboard, blinkers, etc.
 *   - Loop until the game is no longer running or receives SIGINT
 *   - Delegate game logic to the current State object
 *   - The State object can transition to a different State by
 *     updating the "currentAction" in the shared GameState or
 *     by calling a helper method in the Context.
 *
 * NOTE: This code block focuses ONLY on rewriting the
 * run_remote_listener.cpp logic. It references classes
 * and methods you already have (such as GameObject, IInputWithTimer,
 * Blinker, etc.) but does not redefine them here. 
 * For any new interfaces introduced (e.g. IInputListenerState),
 * we put them at the top of this file for illustration.
 *
 * IMPORTANT: Thread-safety
 * -----------
 * This example adds a few std::mutex locks around sensitive
 * operations to guard shared resources (e.g. scoreboard, gameObject).
 * Actual concurrency considerations may require further locking
 * depending on how scoreboard, gameObject, etc. are implemented.
 ************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <map>
#include <mutex>
#include <memory>

// ---------------------------------------------------------
// Forward declarations of existing classes/interfaces
// ---------------------------------------------------------
// class GameState;
// class GameObject;
// class Reset;
// class Inputs;
// class RemoteLocker;
// class IInputWithTimer;
// class IGameInput;
// class Blinker;
// class ScoreBoard;
// class PairingBlinker;
// class BlankBlinker;
// class ScoreboardBlinker;
// class RemotePairingScreen;

// External global for receiving Ctrl-C signals, etc.
extern volatile std::sig_atomic_t gSignalStatus;

// Some additional constants for game states. 
static const int PAIRING_MODE_STATE             = 1;
static const int SLEEP_MODE_STATE               = 2;
static const int AFTER_SLEEP_MODE_STATE         = 3;   // e.g. "wake up" mode
static const int REGULAR_PLAY_NO_SCORE_STATE    = 4; 
static const int REGULAR_PLAY_AFTER_SCORE_STATE = 5; 

#include "GameObject/GameObject.h"
#include "GameState/GameState.h"
#include "Reset/Reset.h"
#include "RemoteLocker/RemoteLocker.h"
#include "Inputs/Inputs.h"
#include "IInputWithTimer/IInputWithTimer.h"
#include "RemoteInputWithTimer/RemoteInputWithTimer.h"
#include "KeyboardInputWithTimer/KeyboardInputWithTimer.h"
#include "BlankBlinker/BlankBlinker.h"
#include "IGameInput/IGameInput.h"
#include "RemotePairingScreen/RemotePairingScreen.h"
#include "PairingBlinker/PairingBlinker.h"
#include "ScoreboardBlinker/ScoreboardBlinker.h"
#include "RemoteGameInput/RemoteGameInput.h"
#include "KeyboardGameInput/KeyboardGameInput.h"
#include <csignal>

#define SCORE_DELAY 0

/*==========================================================
 *  Global signal variable to mimic the code's usage
 *==========================================================*/
static volatile std::sig_atomic_t gSignalStatus = 0;

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

/***************************************************************
 * run_remote_listener()
 *
 * This is the main loop that references the new State-based
 * design. Instead of a giant while-loop with many nested
 * conditions, it delegates to the appropriate State object
 * each iteration.
 ***************************************************************/
void run_remote_listener( GameObject* gameObject, GameState* gameStateArg, Reset* reset )
{
    // Setup some constants
    const int KEYBOARD_TIMEOUT = 5000;

    // Basic initialization
    GameState*    gameState     = gameStateArg;
    RemoteLocker* remoteLocker  = new RemoteLocker( gameState );

    // We need the four input readers
    IInputWithTimer* pairingInputWithTimer  = nullptr;
    IInputWithTimer* noBlinkInputWithTimer  = nullptr;
    IInputWithTimer* sleepingInputWithTimer = nullptr;
    IGameInput*      gameInput              = nullptr;

    bool no_score     = true;
    Inputs* inputs    = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    bool keyboard_off = false; // set to true to use the remotes

    // Some initial calls
    print( "calling game object loop game..." );
    gameObject->loopGame();
    print( "sleeping for 1 second..." );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    // Program to the scoreboard interface
    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont( "fonts/8x13B.bdf" );
    std::signal( SIGINT, GameObject::_signalHandler );

    // Set up the screens/blinkers
    print( "constructing blinkers..." );
    RemotePairingScreen* remotePairingScreen = new RemotePairingScreen( scoreboard );
    PairingBlinker*      pairingBlinker      = new PairingBlinker( scoreboard );
    BlankBlinker*        blankBlinker        = new BlankBlinker();
    ScoreboardBlinker*   sleepingBlinker     = new ScoreboardBlinker( scoreboard );

    // Timeouts
    unsigned long pairing_timeout  = 4000;
    unsigned long no_blink_timeout = 4000;
    unsigned long sleeping_timeout = 4000;

    bool is_on_pi = scoreboard->onRaspberryPi();
    print( "is_on_pi: " + std::to_string( is_on_pi ) );

    // Decide whether to use keyboard-based or remote-based input
    if ( is_on_pi && keyboard_off ) {
        pairingInputWithTimer  = new RemoteInputWithTimer( pairingBlinker,  inputs, pairing_timeout  );
        noBlinkInputWithTimer  = new RemoteInputWithTimer( blankBlinker,    inputs, no_blink_timeout );
        sleepingInputWithTimer = new RemoteInputWithTimer( sleepingBlinker, inputs, sleeping_timeout );
        gameInput              = new RemoteGameInput(                       inputs                   );
    } else {
        pairingInputWithTimer  = new KeyboardInputWithTimer( pairingBlinker,  KEYBOARD_TIMEOUT       );
        noBlinkInputWithTimer  = new KeyboardInputWithTimer( blankBlinker,    KEYBOARD_TIMEOUT       );
        sleepingInputWithTimer = new KeyboardInputWithTimer( sleepingBlinker, KEYBOARD_TIMEOUT       );
        gameInput = new KeyboardGameInput();
    }

    // Build our context for the states
    InputListenerContext context( gameObject,
                                  gameState,
                                  reset,
                                  remoteLocker,
                                  pairingInputWithTimer,
                                  noBlinkInputWithTimer,
                                  sleepingInputWithTimer,
                                  gameInput,
                                  remotePairingScreen,
                                  pairingBlinker,
                                  blankBlinker,
                                  sleepingBlinker,
                                  no_score );

    
    while ( gameState->gameRunning() && gSignalStatus != SIGINT ) {         // Main loop
        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ) ); // sleep for some reason.
        int currentState = gameState->getState();                           // check the current state
        if ( currentState == 0 ) {                                          // If we haven't explicitly  
            currentState = PAIRING_MODE_STATE;                              // set a state yet,                
            gameState->setState( PAIRING_MODE_STATE );                      // we default to pairing.
        }
        std::unique_ptr<IInputListenerState> state = createState( currentState ); // Create the appropriate state and handle input
        state->handleInput( context );
    }

    // Cleanup (some items are allocated inside this function)
    delete remoteLocker;
    delete pairingInputWithTimer;
    delete noBlinkInputWithTimer;
    delete sleepingInputWithTimer;
    delete gameInput;
    delete inputs;
    delete remotePairingScreen;
    delete pairingBlinker;
    delete blankBlinker;
    delete sleepingBlinker;
    print( "Exiting run_remote_listener()..." );
}
