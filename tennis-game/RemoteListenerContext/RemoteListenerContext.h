/************************************************************
 * RemoteListenerContext.h
 *
 * This class provides a shared context for all game states.
 * It contains references to the game state, game object,
 * scoreboard, blinkers, input handlers, and other resources
 * needed for processing game logic.
 *
 * This allows different game states to access and modify
 * shared resources while maintaining thread safety.
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

class RemoteListenerContext {
public:
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

    // Getters for shared resources
    GameObject* getGameObject();
    GameState* getGameState();
    Reset* getReset();
    RemoteLocker* getRemoteLocker();
    IInputWithTimer* getPairingInputWithTimer();
    IInputWithTimer* getNoBlinkInputWithTimer();
    IInputWithTimer* getSleepingInputWithTimer();
    IGameInput* getGameInput();
    RemotePairingScreen* getRemotePairingScreen();
    ScoreBoard* getScoreboard();
    PairingBlinker* getPairingBlinker();
    BlankBlinker* getBlankBlinker();
    ScoreboardBlinker* getSleepingBlinker();
    bool& getNoScoreFlag();

    // Thread safety: Lock and unlock the context
    void lock();
    void unlock();

private:
    std::mutex mtx;  // Mutex for thread safety

    // References to shared resources
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
    bool& no_score;  // Reference to the "no_score" flag
};

#endif // REMOTE_LISTENER_CONTEXT_H


