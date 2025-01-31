#include "RemoteListenerContext.h"

/************************************************************
 * RemoteListenerContext Implementation
 *
 * This file provides the implementation of the **RemoteListenerContext**
 * class, which serves as the **shared state container** for different
 * game states within the **State Machine**.
 ************************************************************/

// **Constructor**
RemoteListenerContext::RemoteListenerContext(GameObject* gameObj,
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
                                             bool& noScoreFlag)
    : gameObject(gameObj),
      gameState(gameSt),
      reset(resetPtr),
      remoteLocker(rLocker),
      pairingInputWithTimer(pairingTimer),
      noBlinkInputWithTimer(noBlinkTimer),
      sleepingInputWithTimer(sleepingTimer),
      gameInput(gameIn),
      remotePairingScreen(pairingScreen),
      pairingBlinkerObj(pairingBlinker),
      blankBlinkerObj(blankBlinker),
      sleepingBlinkerObj(sleepingBlinker),
      no_score(noScoreFlag) {
    scoreboard = gameObject->getScoreBoard();
}

// **Accessors (Getters)**
GameObject* RemoteListenerContext::getGameObject() const { return gameObject; }
GameState* RemoteListenerContext::getGameState() const { return gameState; }
Reset* RemoteListenerContext::getReset() const { return reset; }
RemoteLocker* RemoteListenerContext::getRemoteLocker() const { return remoteLocker; }
IInputWithTimer* RemoteListenerContext::getPairingInputWithTimer() const { return pairingInputWithTimer; }
IInputWithTimer* RemoteListenerContext::getNoBlinkInputWithTimer() const { return noBlinkInputWithTimer; }
IInputWithTimer* RemoteListenerContext::getSleepingInputWithTimer() const { return sleepingInputWithTimer; }
IGameInput* RemoteListenerContext::getGameInput() const { return gameInput; }
RemotePairingScreen* RemoteListenerContext::getRemotePairingScreen() const { return remotePairingScreen; }
ScoreBoard* RemoteListenerContext::getScoreboard() const { return scoreboard; }
PairingBlinker* RemoteListenerContext::getPairingBlinker() const { return pairingBlinkerObj; }
BlankBlinker* RemoteListenerContext::getBlankBlinker() const { return blankBlinkerObj; }
ScoreboardBlinker* RemoteListenerContext::getSleepingBlinker() const { return sleepingBlinkerObj; }
bool& RemoteListenerContext::getNoScoreFlag() const { return no_score; }

// **Thread Safety Controls**
void RemoteListenerContext::lock() { mtx.lock(); }
void RemoteListenerContext::unlock() { mtx.unlock(); }
