#ifndef MATCH_WIN_BLINK_STATE_H
#define MATCH_WIN_BLINK_STATE_H

#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../MatchWinBlinker/MatchWinBlinker.h"

class MatchWinBlinkState : public IRemoteListenerState {
public:
    explicit MatchWinBlinkState();
    void handleInput(RemoteListenerContext& context) override;

private:
    void startBlinking(RemoteListenerContext& context);
};

#endif // MATCH_WIN_BLINK_STATE_H
