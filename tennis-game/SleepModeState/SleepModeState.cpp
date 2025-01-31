#include "SleepModeState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h" 
#include "GameTimer.h"
#include <iostream>

void SleepModeState::handleInput(RemoteListenerContext &context) {
    std::lock_guard<std::mutex> lock(context.mtx);

    print("=== [STATE: SleepMode] ===");

    // Block for input from sleepingInputWithTimer
    int selection = context.getSleepingInputWithTimer()->getInput();

    // Transition to AFTER_SLEEP_MODE
    context.gameState->setCurrentAction(AFTER_SLEEP_MODE);

    print("Time slept: " + std::to_string(context.getSleepingInputWithTimer()->getTimeSlept()));

    // Check if the user pressed a valid score button during sleep
    if (selection == GREEN_REMOTE_GREEN_SCORE || selection == GREEN_REMOTE_RED_SCORE) {
        print("Resetting match...");
        context.gameObject->resetMatch();
        print("Match reset complete.");

        // If sleep time exceeded MAX_SLEEP, clear game history
        if (context.getSleepingInputWithTimer()->getTimeSlept() > MAX_SLEEP * 1000) {
            print("Max sleep time exceeded. Clearing game history...");
            context.gameObject->getHistory()->clearHistory();
            print("Game history cleared.");
        }

        // Stay in the loop, the state machine will pick up the transition
        return;
    }

    // After waking up, update the scoreboard and prepare for game continuation
    print("*** Waking up from Sleep Mode, resuming last match ***");
    print("Clearing scoreboard screen...");
    context.scoreboard->clearScreen();
    print("Screen cleared.");
    context.scoreboard->update();
    print("Scoreboard updated.");
}