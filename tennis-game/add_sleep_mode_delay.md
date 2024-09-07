I am creating a tennis scoreboard in C++ and I want the system to ignore scores while the machine is in sleep mode.  Here is the code that I have so far:
```cpp
void GameObject::playerScore( int playerNumber ) {

    if ( _gameState->getCurrentAction() == SLEEP_MODE ) {
        std::cout << "*** WARNING: player score during sleep mode, returning... ***" << std::endl;
        // set timer
        if ( /* 3 seconds not up */ ) {
            // detatch this thread so that the system keeps running
            return;
        }
        // 3 seconds up, continue...
    }

    // update player score normally...
}
```
The problem here is that the machine never gets out of sleep mode because the playerScore method takes it out of sleep mode.

I need you to make it so that when it sees that the current action is sleep mode, the system should  set a timer for about 3 seconds, let all other threads run and ignore the score and return.  After the 3 seconds is up, the player should score normally.  So I only want it to block for a few seconds.  After the 3 seconds is up and the player scores, It will take it out of sleep mode in a different part of the system so don't worry about that.  You just worry about writing the code that will block for three seconds while allowing the other parts of the system to run.