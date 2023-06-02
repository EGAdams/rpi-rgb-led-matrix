#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../NumberDrawer/NumberDrawer.h"
#include <string.h>


#define PLAYER_1_SERVE 1
#define PLAYER_2_SERVE 2

class ScoreBoard {
  public:
      ScoreBoard( Player* player1, Player* player2, GameState* gameState );
      ~ScoreBoard();
      void update();

  private:
      Player*      _player1;
      Player*      _player2;
      GameState*   _gameState;
      NumberDrawer _bigNumberDrawer;
      NumberDrawer _pipeDrawer;
      void         _showLittleNumbers();
      void         _drawPlayerScore( Player* player );
      std::string  _translate( int raw_score );
};
#endif
