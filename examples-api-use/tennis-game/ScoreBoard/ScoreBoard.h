#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Player.h"

class ScoreBoard {
 public:
  ScoreBoard(Player* player1, Player* player2 );
  void update();
  ~ScoreBoard();

 private:
  Player*     _player1;
  Player*     _player2;
  ScoreBoard* _scoreBoard;
};

#endif
