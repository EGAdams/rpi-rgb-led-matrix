#ifndef Inputs_h
#define Inputs_h

#include "../Arduino/Arduino.h"
#include "../GameObject/GameObject.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../PinInterface/PinInterface.h"
#include "../Inputs/Inputs.h"
#include "../Logger/Logger.h"
#include "../Reset/Reset.h"
#include "../RemoteCodeTranslator/RemoteCodeTranslator.h"
#include "../ThreadSafeQueue/ThreadSafeQueue.h"
#include "../RemotePairingScreen/RemotePairingScreen.h"
#include "../PairingBlinker/PairingBlinker.h"

class Inputs {
public:
    Inputs(Player* player1, Player* player2, PinInterface* pinInterface, GameState* gameState);
    ~Inputs();

    void readReset();
    void readUndoButton();
    int readRotary();
    int readRemotePinArray(remoteDataStructure* remoteData);
    int readPlayerButtons();
    int read_mcp23017_value();

    // Getters and setters for new members
    void setInputQueue(ThreadSafeQueue<int>* inputQueue);
    ThreadSafeQueue<int>* getInputQueue() const;

    void setRemotePairingScreen(RemotePairingScreen* remotePairingScreen);
    RemotePairingScreen* getRemotePairingScreen() const;

    void setPairingBlinker(PairingBlinker* pairingBlinker);
    PairingBlinker* getPairingBlinker() const;

    void set_is_on_pi(bool* isOnPi);
    bool* get_is_on_pi() const;

    void setGameObject(GameObject* gameObject);
    GameObject* getGameObject() const;

private:
    Player* _player1;
    Player* _player2;
    PinInterface* _pinInterface;
    GameState* _gameState;
    Reset _reset;
    Logger* _logger;
    RemoteCodeTranslator* _remoteCodeTranslator;

    // New members for input handling
    ThreadSafeQueue<int>* _inputQueue;
    RemotePairingScreen* _remotePairingScreen;
    PairingBlinker* _pairingBlinker;
    GameObject* _gameObject;
    bool* _is_on_pi;
};

#endif
