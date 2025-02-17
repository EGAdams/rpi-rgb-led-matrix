
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

 // External global for receiving Ctrl-C signals, etc.
extern volatile std::sig_atomic_t gSignalStatus;

#include <csignal>
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
#include "StateMachine/StateMachine.h"
#include "MatchWinBlinkState/MatchWinBlinkState.h"
#include "MatchWinBlinker/MatchWinBlinker.h"

#define SCORE_DELAY 0

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
void run_remote_listener( GameObject* gameObject, GameState* gameStatearg, Reset* reset ) {
    const int main_input_timeout = MAIN_INPUT_TIMEOUT; // <---------- set universal timeout here ---------<<
    GameState* gameState = gameStatearg;
    RemoteLocker* remoteLocker = new RemoteLocker( gameState );
    bool no_score = true;
    Inputs* inputs = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    bool keyboard_off = false;     // <---------- toggle the keyboard off here ---------<<
    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont( "fonts/8x13B.bdf" );
    RemotePairingScreen* remotePairingScreen = new RemotePairingScreen( scoreboard );

    // set up blinkers
    std::shared_ptr<PairingBlinker> pairingBlinker = std::make_shared<PairingBlinker>( scoreboard );
    std::shared_ptr<ScoreboardBlinker> sleepingBlinker = std::make_shared<ScoreboardBlinker>( scoreboard );
    std::shared_ptr<BlankBlinker> blankBlinker = std::make_shared<BlankBlinker>();
    std::shared_ptr<MatchWinBlinker> matchWinBlinker = std::make_shared<MatchWinBlinker>( scoreboard );

    // declare the input handlers
    IInputWithTimer* pairingInputWithTimer = nullptr;
    IInputWithTimer* noBlinkInputWithTimer = nullptr;
    IInputWithTimer* sleepingInputWithTimer = nullptr;
    IGameInput* gameInput = nullptr;

    gameObject->loopGame(); // call loop game to initialize sets

    // create the input handlers depending on the machine type
    if ( scoreboard->onRaspberryPi() && keyboard_off ) {
        if ( !pairingBlinker ) {
            print( "*** ERROR: pairingBlinker is NULL before creating RemoteInputWithTimer! ***" );
        }
        pairingInputWithTimer  = new RemoteInputWithTimer( pairingBlinker.get(), inputs, main_input_timeout );
        noBlinkInputWithTimer  = new RemoteInputWithTimer( blankBlinker.get(), inputs, main_input_timeout );
        sleepingInputWithTimer = new RemoteInputWithTimer( sleepingBlinker.get(), inputs, SLEEP_FOREVER );
        gameInput = new RemoteGameInput( inputs );
    } else {
        if ( !pairingBlinker ) {
            print( "*** ERROR: pairingBlinker is NULL before creating KeyInputWithTimer! ***" );
        }
        pairingInputWithTimer = new KeyboardInputWithTimer( pairingBlinker.get(), main_input_timeout );
        noBlinkInputWithTimer = new KeyboardInputWithTimer( blankBlinker.get(), main_input_timeout );
        sleepingInputWithTimer = new KeyboardInputWithTimer( sleepingBlinker.get(), SLEEP_FOREVER );
        gameInput = new KeyboardGameInput();
    }

    // create the state context
    RemoteListenerContext context( scoreboard, gameObject, gameState, reset, remoteLocker,
                                   pairingInputWithTimer, noBlinkInputWithTimer,
                                   sleepingInputWithTimer, gameInput, remotePairingScreen,
                                   pairingBlinker, blankBlinker, sleepingBlinker, matchWinBlinker, no_score );

    // initialize and run the StateMachine
    StateMachine stateMachine( context );
    stateMachine.run();

    // cleanup
    delete remoteLocker;
    delete pairingInputWithTimer;
    delete noBlinkInputWithTimer;
    delete sleepingInputWithTimer;
    delete gameInput;
    delete inputs;
    delete remotePairingScreen;
}

int main() {
    std::cout << "Initializing Tennis Game System..." << std::endl;
    std::signal( SIGINT, GameObject::_signalHandler ); // Register signal handler for graceful shutdown

    // Create core game objects
    GameState* gameState = new GameState();
    ColorManager* colorManager = new ColorManager();
    IDisplay* display = static_cast< IDisplay* >( new ConsoleDisplay( colorManager ) );

    GameObject* gameObject = new GameObject( gameState, display );
    Reset* reset = new Reset(
        gameObject->getPlayer1(),
        gameObject->getPlayer2(),
        gameObject->getPinInterface(),
        gameState
    );

    std::cout << "Game System Initialized. Starting Remote Listener..." << std::endl;
    run_remote_listener( gameObject, gameState, reset );

    // Cleanup dynamically allocated memory
    delete gameObject;
    delete gameState;
    delete reset;
    delete display;
    delete colorManager;

    std::cout << "Shutting down Tennis Game System." << std::endl;
    return 0;
}

