I am creating a tennis scoreboard in C++ and I want the system to ignore scores while the machine is in sleep mode.  Here is the code that I have so far:
```cpp
void GameObject::playerScore( int playerNumber ) {

    if ( _gameState->getCurrentAction() == SLEEP_MODE ) {
        std::cout << "*** WARNING: player score during sleep mode, returning... ***" << std::endl;
        return;
    }

    // update player score normally...
}
```
