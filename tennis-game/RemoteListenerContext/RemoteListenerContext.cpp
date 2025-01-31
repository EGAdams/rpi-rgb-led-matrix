/************************************************************
 * RemoteListenerContext.cpp
 *
 * Implements the RemoteListenerContext class, which provides
 * access to shared resources required by game states.
 ************************************************************/

#include "RemoteListenerContext.h"

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

// Getters
GameObject* RemoteListenerContext::getGameObject() { return gameObject; }
GameState* RemoteListenerContext::getGameState() { return gameState; }
Reset* RemoteListenerContext::getReset() { return reset; }
RemoteLocker* RemoteListenerContext::getRemoteLocker() { return remoteLocker; }
IInputWithTimer* RemoteListenerContext::getPairingInputWithTimer() { return pairingInputWithTimer; }
IInputWithTimer* RemoteListenerContext::getNoBlinkInputWithTimer() { return noBlinkInputWithTimer; }
IInputWithTimer* RemoteListenerContext::getSleepingInputWithTimer() { return sleepingInputWithTimer; }
IGameInput* RemoteListenerContext::getGameInput() { return gameInput; }
RemotePairingScreen* RemoteListenerContext::getRemotePairingScreen() { return remotePairingScreen; }
ScoreBoard* RemoteListenerContext::getScoreboard() { return scoreboard; }
PairingBlinker* RemoteListenerContext::getPairingBlinker() { return pairingBlinkerObj; }
BlankBlinker* RemoteListenerContext::getBlankBlinker() { return blankBlinkerObj; }
ScoreboardBlinker* RemoteListenerContext::getSleepingBlinker() { return sleepingBlinkerObj; }
bool& RemoteListenerContext::getNoScoreFlag() { return no_score; }

// Thread safety
void RemoteListenerContext::lock() { mtx.lock(); }
void RemoteListenerContext::unlock() { mtx.unlock(); }
