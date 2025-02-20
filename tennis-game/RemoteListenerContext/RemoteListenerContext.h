#ifndef REMOTE_LISTENER_CONTEXT_H
#define REMOTE_LISTENER_CONTEXT_H

#include <mutex>
#include <memory>
#include "../GameState/GameState.h"
#include "../GameObject/GameObject.h"
#include "../Reset/Reset.h"
#include "../RemoteLocker/RemoteLocker.h"
#include "../IInputWithTimer/IInputWithTimer.h"
#include "../IGameInput/IGameInput.h"
#include "../RemotePairingScreen/RemotePairingScreen.h"
#include "../PairingBlinker/PairingBlinker.h"
#include "../BlankBlinker/BlankBlinker.h"
#include "../ScoreboardBlinker/ScoreboardBlinker.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../MatchWinBlinker/MatchWinBlinker.h"

/************************************************************
 * RemoteListenerContext
 *
 * This class provides a **shared context** for all game states
 * within the **State Machine**. It manages shared resources
 * like game objects, input handlers, and scoreboard elements.
 *
 * **Key Features**
 * - **Thread-Safe**: Uses explicit `lock()` and `unlock()` methods.
 * - **Encapsulated Access**: Provides getters for all shared resources.
 * - **Centralized State Management**: Holds references to the game state
 *   and relevant inputs required by different states.
 ************************************************************/
class RemoteListenerContext {
public:
    // Constructor initializes shared resources
    RemoteListenerContext(ScoreBoard* scoreboard,
                          GameObject* gameObj,
                          GameState* gameSt,
                          Reset* resetPtr,
                          RemoteLocker* rLocker,
                          IInputWithTimer* pairingTimer,
                          IInputWithTimer* noBlinkTimer,
                          IInputWithTimer* sleepingTimer,
                          IGameInput* gameIn,
                          RemotePairingScreen* pairingScreen,
                          std::shared_ptr<PairingBlinker> pairingBlinker,
                          std::shared_ptr<BlankBlinker> blankBlinker,
                          std::shared_ptr<ScoreboardBlinker> sleepingBlinker,
                          std::shared_ptr<MatchWinBlinker> matchWinBlinker,
                          bool& noScoreFlag);

    // **Accessors for Shared Resources**
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
    bool& getNoScoreFlag() const;

    std::shared_ptr<PairingBlinker> getPairingBlinker() const;
    std::shared_ptr<MatchWinBlinker> getMatchWinBlinker() const;

    // **Thread Safety Controls**
    void lock();
    void unlock();

private:
    ScoreBoard* scoreboard;
    GameObject* gameObject;
    GameState* gameState;
    Reset* reset;
    RemoteLocker* remoteLocker;
    IInputWithTimer* pairingInputWithTimer;
    IInputWithTimer* noBlinkInputWithTimer;
    IInputWithTimer* sleepingInputWithTimer;
    IGameInput* gameInput;
    RemotePairingScreen* remotePairingScreen;
    std::shared_ptr<PairingBlinker> pairingBlinker;
    std::shared_ptr<BlankBlinker> blankBlinker;
    std::shared_ptr<ScoreboardBlinker> sleepingBlinker;
    std::shared_ptr<MatchWinBlinker> matchWinBlinker;
    bool& no_score;
    // Mutex for thread safety
    std::mutex mtx;
};

#endif // REMOTELISTENERCONTEXT_H
