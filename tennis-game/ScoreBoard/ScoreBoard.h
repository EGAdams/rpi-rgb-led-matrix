#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../Player/Player.h"
#include "../../include/led-matrix.h"
#include "../../include/graphics.h"
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
      ScoreBoard*                _scoreBoard;
      RGBMatrix::Options         _matrix_options;
      rgb_matrix::RuntimeOptions _runtime_opt;
      bool FullSaturation( const Color &c );
};

#endif
