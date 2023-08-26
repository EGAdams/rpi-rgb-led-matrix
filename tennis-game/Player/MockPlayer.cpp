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
