#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "Player.h"
#include "led-matrix.h"
#include "graphics.h"

using namespace rgb_matrix;

class ScoreBoard {
 public:
  ScoreBoard( Player* player1, Player* player2 );
  void update( GameState* gameState, RGBMatrix::Options matrix_options, rgb_matrix::RuntimeOptions runtime_opt );
  bool FullSaturation( const Color &c );

 private:
  Player*     _player1;
  Player*     _player2;
  ScoreBoard* _scoreBoard;
  bool FullSaturation( const Color &c );  // Add this line
};

#endif
