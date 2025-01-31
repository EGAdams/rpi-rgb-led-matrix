#ifndef REGULAR_GAME_PLAY_BEFORE_SCORE_STATE_H
#define REGULAR_GAME_PLAY_BEFORE_SCORE_STATE_H

#include "IRemoteListenerState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

class RegularGamePlayBeforeScoreState : public IRemoteListenerState {
public:
    void handleInput(RemoteListenerContext &context) override;

private:
    void handleSelectionAndUpdate(RemoteListenerContext &context, int selection);
    void showHelp();
};

#endif // REGULAR_GAME_PLAY_BEFORE_SCORE_STATE_H
