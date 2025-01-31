#include "PairingModeState.h"
#include <thread>
#include <chrono>
#include <iostream>

/***************************************************************
 * PairingModeState::handleInput()
 * 
 * - Listens for player remote inputs to pair players.
 * - If a valid input is received, it marks the corresponding 
 *   player as paired.
 * - Once both players are paired or pairing times out, 
 *   the state transitions to sleep mode.
 ***************************************************************/
void PairingModeState::handleInput(RemoteListenerContext &context) {
    // Lock shared resources if necessary
    std::lock_guard<std::mutex> lock(context.mtx);

    print("=== [STATE: PairingMode] ===");

    // Ensure we are in pairing mode and the blinker is active
    while (context.remotePairingScreen->inPairingMode() && 
           context.pairingBlinkerObj->awake()) 
    {
        print("Waiting for remote pairing input...");
        
        int selection = context.pairingInputWithTimer->getInput();

        if (selection == GREEN_REMOTE_GREEN_SCORE) {
            context.remotePairingScreen->greenPlayerPressed();
            context.pairingBlinkerObj->setGreenPlayerPaired(true);
            print("Green player paired.");
        } 
        else if (selection == RED_REMOTE_RED_SCORE) {
            context.remotePairingScreen->redPlayerPressed();
            context.pairingBlinkerObj->setRedPlayerPaired(true);
            print("Red player paired.");
        } 
        else {
            print("*** Invalid selection during pairing. ***");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        // If both players are paired, exit the loop
        if (context.pairingBlinkerObj->isBothPlayersPaired()) {
            print("Both players paired, exiting pairing mode.");
            break;
        }
    }

    // If pairing is complete or timeout occurs, transition to sleep mode
    if (!context.pairingBlinkerObj->awake()) {
        print("Pairing blinker is not awake, stopping pairing mode...");
        context.pairingBlinkerObj->stop();
        print("Transitioning to sleep mode...");
        context.gameState->setCurrentAction(SLEEP_MODE);
    }
}
