#ifndef GameTimer_h
#define GameTimer_h

class GameTimer {
 public:
  GameTimer();
  void sleep_until(int milliseconds);
  static void gameDelay(int milliseconds);
  static unsigned long gameMillis();
  ~GameTimer();
};

#endif