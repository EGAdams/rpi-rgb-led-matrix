#ifndef GameWinSequence_h
#define GameWinSequence_h

#include "../Arduino/Arduino.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../GameLeds/GameLeds.h"
#include "../GameTimer/GameTimer.h"
class GameWinSequence {

 public:
  GameWinSequence();
  ~GameWinSequence();
  void run( IPlayer* player, GameState* gameState, 
            GameLeds* gameLeds, ScoreBoard* scoreBoard, int games_in_memory ); };
#endif