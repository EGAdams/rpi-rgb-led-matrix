#ifndef History_h
#define History_h

#include <stack>
#include "GameState.h"
#include "Logger.h"

class History {
 public:
  History();
  ~History();
  void push(GameState gameState);
  GameState pop();
  int size();

 private:
  std::stack<GameState> _history;
  Logger* _logger;
};

#endif
