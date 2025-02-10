#ifndef AFTER_MATCH_WIN_STATE_H
#define AFTER_MATCH_WIN_STATE_H

#include "../TennisConstants/TennisConstants.h"
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

class AfterMatchWinState : public IRemoteListenerState {
public:
    void handleInput(RemoteListenerContext &context) override;

private:
    void handleSelectionAndUpdate(RemoteListenerContext &context, int selection);
    void showHelp();
};

#endif // AFTER_MATCH_WIN_STATE_H