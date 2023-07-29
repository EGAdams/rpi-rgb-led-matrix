The error message indicates that there is a problem with the function declarations in the `GameState` class. It seems that the functions `setPlayer1SetHistory`, `getPlayer1SetHistory`, `setPlayer2SetHistory`, and `getPlayer2SetHistory` are being overloaded incorrectly.

To fix this issue, you need to ensure that the function signatures are unique. In C++, function overloading is based on the number and types of the function parameters. In this case, the functions have the same parameter types, which is causing the error.

To resolve the issue, you can modify the function names slightly to make them unique. For example, you can add a suffix to the function names:

In GameState.h:
```cpp
void setPlayer1SetHistory(std::map<int, int> player1_set_history) override;
std::map<int, int> getPlayer1SetHistory() override;
void setPlayer2SetHistory(std::map<int, int> player2_set_history) override;
std::map<int, int> getPlayer2SetHistory() override;
```

Change the function names to:
```cpp
void setPlayer1SetHistoryOverride(std::map<int, int> player1_set_history) override;
std::map<int, int> getPlayer1SetHistoryOverride() override;
void setPlayer2SetHistoryOverride(std::map<int, int> player2_set_history) override;
std::map<int, int> getPlayer2SetHistoryOverride() override;
```

Then, in GameState.cpp, update the function definitions accordingly:

```cpp
void GameState::setPlayer1SetHistoryOverride(std::map<int, int> player1_set_history) {
    _player1_set_history = player1_set_history;
}

std::map<int, int> GameState::getPlayer1SetHistoryOverride() {
    return _player1_set_history;
}

void GameState::setPlayer2SetHistoryOverride(std::map<int, int> player2_set_history) {
    _player2_set_history = player2_set_history;
}

std::map<int, int> GameState::getPlayer2SetHistoryOverride() {
    return _player2_set_history;
}
```