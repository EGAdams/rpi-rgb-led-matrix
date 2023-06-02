#ifndef Player_h
#define Player_h

class Player {
 public:
  Player(int player_number);
  ~Player();
  void setPoints(int points);   int getPoints();
  void setGames(int games);     int getGames();
  void setSets(int sets);       int getSets();
  void setMatches(int matches); int getMatches();
  void setMode(int mode);       int getMode();
  void setSetting(int setting); int getSetting();

  int incrementSetting();
  int number(); // only set during construction

 private:
  int _playerNumber;
  int _points;
  int _games;
  int _sets;
  int _matches;
  int _mode;
  int _setting;
};
#endif
