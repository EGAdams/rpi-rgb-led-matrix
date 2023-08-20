#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Mode1Score.h"

// Assuming that Player, GameState, PinInterface, and History are interfaces,
// we can create mock classes for them
class MockPlayer : public IPlayer {
    public:
    MOCK_METHOD(void, setOpponent, (IPlayer*), (override));
    MOCK_METHOD(IPlayer*, getOpponent, (), (override));
    MOCK_METHOD(void, setPoints, (int), (override));
    MOCK_METHOD(int, getPoints, (), (override));
    MOCK_METHOD(void, setSets, ( IGameState* gameState, int game_value ), (override));
    // get sets
    MOCK_METHOD( int, getSets, (), (override));
    // set games
    MOCK_METHOD(void, setGames, (int), (override));
    // get games
    MOCK_METHOD(int, getGames, (), (override));
    // set matches
    MOCK_METHOD(void, setMatches, (int), (override));
    // get matches
    MOCK_METHOD(int, getMatches, (), (override));
    // set mode
    MOCK_METHOD(void, setMode, (int), (override));
    // get mode
    MOCK_METHOD(int, getMode, (), (override));
    // set setting
    MOCK_METHOD(void, setSetting, (int), (override));
    // get setting  
    MOCK_METHOD(int, getSetting, (), (override));
    // increment setting
    MOCK_METHOD(int, incrementSetting, (), (override));
    // number
    MOCK_METHOD(int, number, (), (override));
    // set set history
    MOCK_METHOD(void, setSetHistory, (int, int), (override));
    // get set history
    MOCK_METHOD(void, setGameHistory, (int, int), (override));
    // set serve switch
    MOCK_METHOD(void, setServeSwitch, (int), (override));
    // get serve switch
    MOCK_METHOD(int, getServeSwitch, (), (override));
    // get set history
    // MOCK_METHOD(std::map< int, int>, getSetHistory, (), (override));

    
};

class MockGameState : public GameState {
    // Similarly, add method declarations here
};

class MockPinInterface : public PinInterface {
    // Add method declarations here
};

class MockHistory : public History {
    // Add method declarations here
};

class Mode1ScoreTest : public ::testing::Test {
protected:
    Player player1;
    Player player2;
    MockPinInterface pinInterface;
    MockGameState gameState;
    MockHistory history;

    std::unique_ptr<Mode1Score> mode1Score;

    void SetUp() override {
        mode1Score = std::make_unique<Mode1Score>(&player1, &player2, &pinInterface, &gameState, &history);
    }
};

TEST_F(Mode1ScoreTest, TestUpdateScore) {
    // Arrange
    //ON_CALL(player1, getPoints()).WillByDefault(::testing::Return(3));
    //ON_CALL(player2, getPoints()).WillByDefault(::testing::Return(2));

    // Act
    mode1Score->updateScore(&player1);

    // Assert
    // Assuming that setPoints is a method in the Player interface and it changes the internal state of the player
    //EXPECT_CALL(player1, setPoints(4)).Times(1);
}

// Continue with other tests...