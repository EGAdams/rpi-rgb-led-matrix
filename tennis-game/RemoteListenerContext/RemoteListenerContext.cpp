#include "RemoteListenerContext.h"
#include <iostream>

// Constructor
RemoteListenerContext::RemoteListenerContext(GameObject* gameObj,
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
      scoreboard(nullptr), // Initialize this if necessary
      no_score(noScoreFlag) {
    // Constructor implementation
    std::cout << "RemoteListenerContext constructor called" << std::endl;
}

// Accessors (Getters)
GameObject* RemoteListenerContext::getGameObject() const {
    return gameObject;
}

GameState* RemoteListenerContext::getGameState() const {
    return gameState;
}

Reset* RemoteListenerContext::getReset() const {
    return reset;
}

RemoteLocker* RemoteListenerContext::getRemoteLocker() const {
    return remoteLocker;
}

IInputWithTimer* RemoteListenerContext::getPairingInputWithTimer() const {
    return pairingInputWithTimer;
}

IInputWithTimer* RemoteListenerContext::getNoBlinkInputWithTimer() const {
    return noBlinkInputWithTimer;
}

IInputWithTimer* RemoteListenerContext::getSleepingInputWithTimer() const {
    return sleepingInputWithTimer;
}

IGameInput* RemoteListenerContext::getGameInput() const {
    return gameInput;
}

RemotePairingScreen* RemoteListenerContext::getRemotePairingScreen() const {
    return remotePairingScreen;
}

ScoreBoard* RemoteListenerContext::getScoreboard() const {
    return scoreboard;
}

bool& RemoteListenerContext::getNoScoreFlag() const {
    return no_score;
}

// Thread Safety Controls
void RemoteListenerContext::lock() {
    mtx.lock();
}

void RemoteListenerContext::unlock() {
    mtx.unlock();
}
