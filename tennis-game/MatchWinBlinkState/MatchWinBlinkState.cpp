#include "MatchWinBlinkState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include "../GameState/GameState.h"
#include "../GameObject/GameObject.h"
#include "../Inputs/Inputs.h"
#include "../History/History.h"
#include "../TennisConstants/TennisConstants.h"
#define MATCH_WIN_OFFSET 12

MatchWinBlinkState::MatchWinBlinkState() {}

void MatchWinBlinkState::handleInput(RemoteListenerContext& context) {
    context.lock();
    print("===================================");
    print("=== [STATE: MatchWinBlinkState] ===");
    print("===================================\n\n\n");

    // Start blinking asynchronously
    startBlinking(context);

    while (context.getMatchWinBlinker()->isRunning()) {
        int selection = context.getNoBlinkInputWithTimer()->getInput(); // Listen for input while blinking
        print("Selection from noBlinkInputWithTimer: " + std::to_string(selection));

        if (selection == GREEN_REMOTE_GREEN_SCORE ||
            selection == GREEN_REMOTE_RED_SCORE ||
            selection == RED_REMOTE_GREEN_SCORE ||
            selection == RED_REMOTE_RED_SCORE) {
            print("*** Player button pressed, transitioning to Regular Play After Score State ***");
            context.getGameState()->setState(REGULAR_PLAY_AFTER_SCORE_STATE);
            context.getScoreboard()->update();
            context.getMatchWinBlinker()->stop();  // Stop blinking early
            context.unlock();
            return;
        } else if (selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO) {
            print("*** Undo ***");
            context.getGameObject()->undo();
            context.getGameState()->setState(REGULAR_PLAY_AFTER_SCORE_STATE);
            context.getScoreboard()->update();
            context.getMatchWinBlinker()->stop();  // Stop blinking early
            context.unlock();
            return;
        }
    }

    // If no input was received and blinking is complete, transition to After Match Win State
    print("*** MatchWinBlinker finished blinking, transitioning to AfterMatchWinState ***");
    context.getGameState()->setState(AFTER_MATCH_WIN_STATE);
    context.getScoreboard()->clearScreen();
    context.getScoreboard()->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
    context.getScoreboard()->drawNewText( "Win",   20, 75 - MATCH_WIN_OFFSET );
    context.getScoreboard()->drawSets();
    context.unlock();
}

void MatchWinBlinkState::startBlinking(RemoteListenerContext& context) {
    if (!context.getMatchWinBlinker()->isRunning()) {
        context.getMatchWinBlinker()->start();
    }
}
