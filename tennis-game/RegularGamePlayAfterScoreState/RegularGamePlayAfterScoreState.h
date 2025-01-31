#ifndef REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
#define REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include "../RegularGamePlayAfterScoreState/RegularGamePlayAfterScoreState.h"

class RegularGamePlayAfterScoreState : public IRemoteListenerState {
public:
    void handleInput(RemoteListenerContext &context) override;

private:
    void showHelp();
};

#endif // REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
