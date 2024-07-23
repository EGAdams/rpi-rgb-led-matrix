The following C++ code is supposed to be setting up a tie-break scenario
where when player 2 scores we need to determine what to do next.
There is no framework in the following code, it is just a vanilla test.
Here is ths C++ Source code that I need you to analyze:
```cpp
void test_05( GameObject* gameObject, GameState* gameState, int* loop_count ) {
    gameObject->getScoreBoard()->clearScreen();
    gameObject->getPlayer1()->setSetHistory( 1, 6 );
    gameObject->getPlayer2()->setSetHistory( 1, 4 );
    gameObject->getPlayer1()->setSetHistory( 2, 4 );
    gameObject->getPlayer2()->setSetHistory( 2, 5 );
    gameState->setCurrentSet( 2 );
    gameObject->getPlayer1()->setGames( 4 );
    gameObject->getPlayer2()->setGames( 5 );
    gameObject->getPlayer1()->setPoints( 2 );
    gameObject->getPlayer2()->setPoints( 3 );
    std::cout << "updating scoreboard... " << std::endl;
    gameObject->getScoreBoard()->update();
    std::cout << "done updating scoreboard." << std::endl;
}
```

According to standard tennis rules, what should happen if player 2
scores in the scenario set up in the cpp source code?

