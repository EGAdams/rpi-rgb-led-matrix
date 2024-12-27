#ifndef RemoteLocker_h
#define RemoteLocker_h

#include "../GameState/GameState.h"

class RemoteLocker {
 public:
  RemoteLocker( GameState* gameState );
  ~RemoteLocker();
  bool playerNotServing( int button );

 private:
  GameState* _gameState;
};

#endif
