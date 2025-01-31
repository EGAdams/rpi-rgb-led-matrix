#ifndef SLEEP_MODE_STATE_H
#define SLEEP_MODE_STATE_H

#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include "../GameTimer/GameTimer.h"

class SleepModeState : public IRemoteListenerState {
public:
    void handleInput(RemoteListenerContext &context) override;
};

#endif // SLEEP_MODE_STATE_H
