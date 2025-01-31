/************************************************************
 * RemoteListenerContext.h
 *
 * Shared context for managing game states in a thread-safe
 * manner. This context holds references to essential game
 * objects, input handlers, and visual elements required
 * for different states within the State Machine.
 *
 * The context ensures synchronized access to shared resources
 * through explicit locking methods.
 ************************************************************/

#ifndef REMOTE_LISTENER_CONTEXT_H
#define REMOTE_LISTENER_CONTEXT_H

#include <mutex>
#include "GameState.h"
#include "GameObject.h"
#include "Reset.h"
#include "RemoteLocker.h"
#include "IInputWithTimer.h"
#include "IGameInput.h"
#include "RemotePairingScreen.h"
#include "PairingBlinker.h"
#include "BlankBlinker.h"
#include "ScoreboardBlinker.h"
#include "ScoreBoard.h"  // Assuming ScoreBoard is a class not included in the original

class RemoteListenerContext {
public:
    // Constructor to initialize shared resources
    RemoteListenerContext(GameObject* gameObj,
                          GameState* gameSt,
                          Reset* resetPtr,
                          RemoteLocker* rLocker,
                          IInputWithTimer* pairingTimer,
                          IInputWithTimer* noBlinkTimer,
                          IInputWithTimer* sleepingTimer,
                          IGameInput* gameIn,
                          RemotePairingScreen* pairingScreen,
                          PairingBlinker* pairingBlinker,
                          BlankBlinker* blankBlinker,
                          ScoreboardBlinker* sleepingBlinker,
                          bool& noScoreFlag);

    // Accessor Methods for Shared Resources
    GameObject* getGameObject() const;
    GameState* getGameState() const;
    Reset* getReset() const;
    RemoteLocker* getRemoteLocker() const;
    IInputWithTimer* getPairingInputWithTimer() const;
    IInputWithTimer* getNoBlinkInputWithTimer() const;
    IInputWithTimer* getSleepingInputWithTimer() const;
    IGameInput* getGameInput() const;
    RemotePairingScreen* getRemotePairingScreen() const;
    ScoreBoard* getScoreboard() const;
    PairingBlinker* getPairingBlinker() const;
    BlankBlinker* getBlankBlinker() const;
    ScoreboardBlinker* getSleepingBlinker() const;
    bool& getNoScoreFlag() const;

    // Thread Safety Controls
    void lock();
    void unlock();

private:
    mutable std::mutex mtx;  // Mutex for thread-safe access

    // Shared Game Resources
    GameObject* gameObject;
    GameState* gameState;
    Reset* reset;
    RemoteLocker* remoteLocker;
    IInputWithTimer* pairingInputWithTimer;
    IInputWithTimer* noBlinkInputWithTimer;
    IInputWithTimer* sleepingInputWithTimer;
    IGameInput* gameInput;
    RemotePairingScreen* remotePairingScreen;
    ScoreBoard* scoreboard;
    PairingBlinker* pairingBlinkerObj;
    BlankBlinker* blankBlinkerObj;
    ScoreboardBlinker* sleepingBlinkerObj;

    // Reference to the no_score flag
    bool& no_score;
};

#endif // REMOTE_LISTENER_CONTEXT_H
