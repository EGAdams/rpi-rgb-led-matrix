#ifndef UNDO_STATE_H
#define UNDO_STATE_H
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../RemoteListenerContext/RemoteListenerContext.h"
#include "../UndoState/UndoState.h"
#include "../TennisConstants/TennisConstants.h"

class UndoState : public IRemoteListenerState {
public:
    void handleInput(RemoteListenerContext &context) override;

private:
    void _showHelp();
};

#endif // UNDO_STATE_H
