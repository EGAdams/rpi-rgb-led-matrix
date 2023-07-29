#include <gmock/gmock.h>
#include "IPlayer.h"

class MockPlayer : public IPlayer {
public:
    MOCK_METHOD(void, setPoints, (int points), (override));
    MOCK_METHOD(int, getPoints, (), (override));
    MOCK_METHOD(void, setGames, (int games), (override));
    MOCK_METHOD(int, getGames, (), (override));
    MOCK_METHOD(IPlayer*, getOpponent, (), (override));
    // ... add other methods as needed
};
