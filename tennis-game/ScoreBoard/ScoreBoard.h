#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../Player/Player.h"
#include "../GameState/GameState.h"

using namespace rgb_matrix;

class ScoreBoard {
  public:
      ScoreBoard( Player* player1, Player* player2, GameState* gameState );
      ~ScoreBoard();
      void update();

  private:
      Player*                    _player1;
      Player*                    _player2;
      GameState*                 _gameState;
};

#endif
