#ifndef REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
#define REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H

#include "IRemoteListenerState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include <memory>
#include <mutex>

class RegularGamePlayAfterScoreState : public IRemoteListenerState {
public:
    void handleInput(RemoteListenerContext &context) override;

private:
    void showHelp();
};

#endif // REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
