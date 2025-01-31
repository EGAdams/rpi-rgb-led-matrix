#ifndef SLEEP_MODE_STATE_H
#define SLEEP_MODE_STATE_H

#include <memory>
#include <mutex>
#include "IRemoteListenerState.h"
#include "RemoteListenerContext.h"
#include <iostream>
#include <thread>
#include <chrono>

class SleepModeState : public IRemoteListenerState {
public:
    void handleInput(RemoteListenerContext &context) override;
};

#endif // SLEEP_MODE_STATE_H
