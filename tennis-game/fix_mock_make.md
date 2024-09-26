# Your Goal
help me fix the output of a make

# make output
```bash
adamsl@DESKTOP-2OBSQMC:~/rpi-rgb-led-matrix/tennis-game$ make
g++ -std=c++17 -Wall -O3 -g -Wextra -Wno-unused-parameter -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googletest/include -I/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include -I../include -I. -c -o Player/MockPlayer.o Player/MockPlayer.cpp
In file included from /home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-actions.h:148,
                 from /home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock.h:56,
                 from Player/MockPlayer.cpp:1:
Player/MockPlayer.cpp:6:23: error: ‘testing::internal::Function<void(int)>::Result MockPlayer::setPoints(testing::internal::ElemFromList<0, int>::type)’ marked ‘overrid
’, but does not override
    6 |     MOCK_METHOD(void, setPoints, (int points), (override));
      |                       ^~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:17:31: note: in definition of macro ‘GMOCK_PP_IDENTITY’
   17 | #define GMOCK_PP_IDENTITY(_1) _1
      |                               ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:133:3: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_IMPL’
  133 |   GMOCK_INTERNAL_MOCK_METHOD_IMPL(                                         \
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:152:39: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_ARG_
’
  152 | #define GMOCK_PP_INTERNAL_CAT(_1, _2) _1##_2
      |                                       ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:115:3: note: in expansion of macro ‘GMOCK_PP_VARIADIC_CALL’
  115 |   GMOCK_PP_VARIADIC_CALL(GMOCK_INTERNAL_MOCK_METHOD_ARG_, __VA_ARGS__) \
      |   ^~~~~~~~~~~~~~~~~~~~~~
Player/MockPlayer.cpp:6:5: note: in expansion of macro ‘MOCK_METHOD’
    6 |     MOCK_METHOD(void, setPoints, (int points), (override));
      |     ^~~~~~~~~~~
Player/MockPlayer.cpp:7:22: error: ‘testing::internal::Function<int()>::Result MockPlayer::getPoints()’ marked ‘override’, but does not override
    7 |     MOCK_METHOD(int, getPoints, (), (override));
      |                      ^~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:17:31: note: in definition of macro ‘GMOCK_PP_IDENTITY’
   17 | #define GMOCK_PP_IDENTITY(_1) _1
      |                               ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:133:3: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_IMPL’
  133 |   GMOCK_INTERNAL_MOCK_METHOD_IMPL(                                         \
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:152:39: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_ARG_
’
  152 | #define GMOCK_PP_INTERNAL_CAT(_1, _2) _1##_2
      |                                       ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:115:3: note: in expansion of macro ‘GMOCK_PP_VARIADIC_CALL’
  115 |   GMOCK_PP_VARIADIC_CALL(GMOCK_INTERNAL_MOCK_METHOD_ARG_, __VA_ARGS__) \
      |   ^~~~~~~~~~~~~~~~~~~~~~
Player/MockPlayer.cpp:7:5: note: in expansion of macro ‘MOCK_METHOD’
    7 |     MOCK_METHOD(int, getPoints, (), (override));
      |     ^~~~~~~~~~~
Player/MockPlayer.cpp:8:23: error: ‘testing::internal::Function<void(int)>::Result MockPlayer::setGames(testing::internal::ElemFromList<0, int>::type)’ marked ‘override’, but does not override
    8 |     MOCK_METHOD(void, setGames, (int games), (override));
      |                       ^~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:17:31: note: in definition of macro ‘GMOCK_PP_IDENTITY’
   17 | #define GMOCK_PP_IDENTITY(_1) _1
      |                               ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:133:3: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_IMPL’
  133 |   GMOCK_INTERNAL_MOCK_METHOD_IMPL(                                         \
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:152:39: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_ARG_
’
  152 | #define GMOCK_PP_INTERNAL_CAT(_1, _2) _1##_2
      |                                       ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:115:3: note: in expansion of macro ‘GMOCK_PP_VARIADIC_CALL’
  115 |   GMOCK_PP_VARIADIC_CALL(GMOCK_INTERNAL_MOCK_METHOD_ARG_, __VA_ARGS__) \
      |   ^~~~~~~~~~~~~~~~~~~~~~
Player/MockPlayer.cpp:8:5: note: in expansion of macro ‘MOCK_METHOD’
    8 |     MOCK_METHOD(void, setGames, (int games), (override));
      |     ^~~~~~~~~~~
Player/MockPlayer.cpp:9:22: error: ‘testing::internal::Function<int()>::Result MockPlayer::getGames()’ marked ‘override’, but does not override
    9 |     MOCK_METHOD(int, getGames, (), (override));
      |                      ^~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:17:31: note: in definition of macro ‘GMOCK_PP_IDENTITY’
   17 | #define GMOCK_PP_IDENTITY(_1) _1
      |                               ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:133:3: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_IMPL’
  133 |   GMOCK_INTERNAL_MOCK_METHOD_IMPL(                                         \
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:152:39: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_ARG_
’
  152 | #define GMOCK_PP_INTERNAL_CAT(_1, _2) _1##_2
      |                                       ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:115:3: note: in expansion of macro ‘GMOCK_PP_VARIADIC_CALL’
  115 |   GMOCK_PP_VARIADIC_CALL(GMOCK_INTERNAL_MOCK_METHOD_ARG_, __VA_ARGS__) \
      |   ^~~~~~~~~~~~~~~~~~~~~~
Player/MockPlayer.cpp:9:5: note: in expansion of macro ‘MOCK_METHOD’
    9 |     MOCK_METHOD(int, getGames, (), (override));
      |     ^~~~~~~~~~~
Player/MockPlayer.cpp:10:26: error: ‘Player* MockPlayer::getOpponent()’ marked ‘override’, but does not override
   10 |     MOCK_METHOD(Player*, getOpponent, (), (override));
      |                          ^~~~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:17:31: note: in definition of macro ‘GMOCK_PP_IDENTITY’
   17 | #define GMOCK_PP_IDENTITY(_1) _1
      |                               ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:133:3: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_IMPL’
  133 |   GMOCK_INTERNAL_MOCK_METHOD_IMPL(                                         \
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/internal/gmock-pp.h:152:39: note: in expansion of macro ‘GMOCK_INTERNAL_MOCK_METHOD_ARG_
’
  152 | #define GMOCK_PP_INTERNAL_CAT(_1, _2) _1##_2
      |                                       ^~
/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/googlemock/include/gmock/gmock-function-mocker.h:115:3: note: in expansion of macro ‘GMOCK_PP_VARIADIC_CALL’
  115 |   GMOCK_PP_VARIADIC_CALL(GMOCK_INTERNAL_MOCK_METHOD_ARG_, __VA_ARGS__) \
      |   ^~~~~~~~~~~~~~~~~~~~~~
Player/MockPlayer.cpp:10:5: note: in expansion of macro ‘MOCK_METHOD’
   10 |     MOCK_METHOD(Player*, getOpponent, (), (override));
      |     ^~~~~~~~~~~
make: *** [Makefile:48: Player/MockPlayer.o] Error 1
adamsl@DESKTOP-2OBSQMC:~/rpi-rgb-led-matrix/tennis-game$
```

# Mock Player C++ Source Code
```cpp
#include <gmock/gmock.h>
#include "Player.h"

class MockPlayer : public Player {
public:
    MOCK_METHOD(void, setPoints, (int points), (override));
    MOCK_METHOD(int, getPoints, (), (override));
    MOCK_METHOD(void, setGames, (int games), (override));
    MOCK_METHOD(int, getGames, (), (override));
    MOCK_METHOD(Player*, getOpponent, (), (override));
    // ... add other methods as needed
};
```

# Player.h
```cpp
#include "../GameState/GameState.h"
#include "../TennisConstants/TennisConstants.h"
#include <map>

class GameState;
class Player {
public:
    Player( GameState* gameState, int player_number );
    ~Player();
    
    void setOpponent(    Player* opponent           );
    Player* getOpponent();

    void setSets( GameState* gameState, int sets );
    int getSets();

    void setSets( int sets );

    void setPoints( int points  );
    int getPoints();

    void setGames( int game_value  ); // it knows the current set
    int getGames();

    void setServeSwitch( int serve_switch                 );
    int getServeSwitch();

    void setMatches(     int matches );
    int getMatches();

    void setMode(        int mode    );
    int getMode();

    void setSetting(     int setting );
    int getSetting();

    void setSetHistory(  int set, int score  );
    std::map<int, int> getSetHistory();

    void setSetHistory( std::map<int, int> );

    void setGameHistory( int game, int score );
    std::map<int, int> getGameHistory();

    void clearGameHistory();
    void clearSetHistory();

    int incrementSetting();

    int number(); // only set during construction

private:
    GameState* _gameState;
    int _playerNumber;
    Player* _opponent;
    int _points;
    int _games;
    int _sets;
    int _matches;
    int _mode;
    int _setting;
    int _serve_switch;
    std::map<int, int> _set_history;
    std::map<int, int> _game_history;
};
```
