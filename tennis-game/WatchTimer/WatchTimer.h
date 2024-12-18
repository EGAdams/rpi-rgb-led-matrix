#ifndef WatchTimer_H
#define WatchTimer_H
#include "../TennisConstants/TennisConstants.h"
#include "../GameTimer/GameTimer.h"
// #include "../Inputs/Inputs.h"

class Inputs;

class WatchTimer {
 public:
  WatchTimer();
  ~WatchTimer();

  int watchInputDelay(        int delay, Inputs* input, int watchInterval );
  int watchPLayerButtonDelay( int delay,                int watchInterval );
};

#endif
