#include "StateMachine.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>  // Required for handling Ctrl+C (SIGINT)

// Global variable for tracking SIGINT (Ctrl+C) interrupts
volatile std::sig_atomic_t gSignalStatus = 0;

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 * @param signal The received signal.
 */
void signalHandler( int signal ) {
    if ( signal == SIGINT ) {
        gSignalStatus = 1;  // Set flag to indicate exit request
        std::cout << "\n[StateMachine] SIGINT received! Shutting down gracefully...\n";
    }
}

/**
 * @brief Constructs the StateMachine and initializes the starting state.
 * @param context Reference to the RemoteListenerContext.
 */
StateMachine::StateMachine( RemoteListenerContext& context )
    : _context( context ), _currentStateId( PAIRING_MODE_STATE ) {

    // Register signal handler for SIGINT
    std::signal( SIGINT, signalHandler );

    setState( PAIRING_MODE_STATE ); // Start in Pairing Mode
}

/**
 * @brief Runs the state machine loop, processing input and transitioning states.
 */
void StateMachine::run() {
    while ( _context.getGameState()->gameRunning() && gSignalStatus != 1 ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( _state_machine_delay ) ); // sleep for 500 milliseconds
        _currentState->handleInput( _context );                 // Execute the logic for the current state
        int nextState = _context.getGameState()->getState();    // Transition to a new state if necessary
        if ( nextState != _currentStateId ) {
            setState( nextState );
        }
    }
    std::cout << "[StateMachine] Exiting state machine loop.\n";
}

/**
 * @brief Transitions the state machine to a new state.
 * @param newState The new state identifier.
 */
void StateMachine::setState( int newState ) {
    _currentStateId = newState;
    _currentState = createState( newState );

    std::cout << "[StateMachine] Transitioned to state: " << newState << std::endl;
}

/**
 * @brief Creates the appropriate state object based on the game action.
 * @param currentAction The action representing the current state.
 * @return A unique pointer to the corresponding state object.
 */
std::unique_ptr<IRemoteListenerState> StateMachine::createState( int currentAction ) {
    switch ( currentAction ) {
    case PAIRING_MODE_STATE:
        return std::make_unique<PairingModeState>();
    case SLEEP_MODE_STATE:
        return std::make_unique<SleepModeState>();
    case REGULAR_PLAY_NO_SCORE_STATE:
        return std::make_unique<RegularGamePlayBeforeScoreState>();
    case REGULAR_PLAY_AFTER_SCORE_STATE:
    case AFTER_SLEEP_MODE_STATE: // Treat "After Sleep" as "After Score"
        return std::make_unique<RegularGamePlayAfterScoreState>();
    default:
        std::cout << "[StateMachine] WARNING: Unknown state (" << currentAction 
        << "), defaulting to REGULAR_PLAY_AFTER_SCORE_STATE." << std::endl;
        return std::make_unique<RegularGamePlayAfterScoreState>();
    }
}
