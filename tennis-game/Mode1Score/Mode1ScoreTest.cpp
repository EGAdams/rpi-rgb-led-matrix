#include <gtest/gtest.h>
#include "Mode1Score.h"
class Mode1ScoreTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    PinInterface* _pinInterface;
    GameState* _gameState;
    History* _history;
    Mode1Score* _mode1Score;

    void SetUp() override;
    void TearDown() override;
};

void Mode1ScoreTest::SetUp() {
    _gameState = new GameState();
    _player1 = new Player( _gameState, PLAYER_1_INITIALIZED );
    _player2 = new Player( _gameState, PLAYER_2_INITIALIZED );
    _player1->setOpponent( _player2 ); _player2->setOpponent( _player1 );
    std::map<std::string, int> pin_map;
    PinState* pin_state = new PinState( pin_map );
    _pinInterface = new PinInterface( pin_state );
    _history = new History();
    _mode1Score = new Mode1Score( _player1, _player2, _pinInterface, _gameState, _history );
}

// Tear down the test fixture
void Mode1ScoreTest::TearDown() {
    delete _mode1Score;
    delete _history;
    delete _gameState;
    delete _pinInterface;
    delete _player2;
    delete _player1;
}


///////////////// Test case: Test Mode 1 P1 Score Less Than 3 Points //////////
TEST_F( Mode1ScoreTest, TestMode1P1Score_LessThan3Points ) {
    _player1->setPoints( 2 );
    _player2->setPoints( 1 );
    _mode1Score->updateScore( _player1  );
    EXPECT_EQ( _player1->getPoints(), 2 );
    EXPECT_EQ( _player2->getPoints(), 1 );
    // Check other changes made by the method
}
///////////////////////////////////////////////////////////////////////////////


// Test case: Mode1P1ScoreTest
TEST_F( Mode1ScoreTest, Mode1P1ScoreTest) {
    // Test implementation here
}

// Test case: TestMode1P1Score_3Points_LessThan3PointsP2
TEST_F( Mode1ScoreTest, TestMode1P1Score_3Points_LessThan3PointsP2) {
    _player1->setPoints(3);
    _player2->setPoints( 2 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 3);
    EXPECT_EQ( _player2->getPoints(), 2);
    // Check other changes made by the method
}

// Test case: TestMode1P1Score_3Points_EqualPoints
TEST_F( Mode1ScoreTest, TestMode1P1Score_3Points_EqualPoints ) {
    _player1->setPoints(3);
    _player2->setPoints(3);
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 3);
    EXPECT_EQ( _player2->getPoints(), 3);
    // Check other changes made by the method
}

// Test case: TestMode1P1Score_MoreThan3Points_DifferenceMoreThan1
TEST_F( Mode1ScoreTest, TestMode1P1Score_MoreThan3Points_DifferenceMoreThan1) {
    _player1->setPoints(5);
    _player2->setPoints(3);
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 5);
    EXPECT_EQ( _player2->getPoints(), 3);
    EXPECT_EQ( _player1->getGames(), 1 ); // Assuming getGames starts from 0
    // Check other changes made by the method
}

// Test case: TestMode1P1Score_4Points
TEST_F( Mode1ScoreTest, TestMode1P1Score_4Points ) {
    _player1->setPoints(4);
    _player2->setPoints( 2 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 4);
    EXPECT_EQ( _player2->getPoints(), 2);
    EXPECT_EQ( _gameState->getPointFlash(), 1 ); // Assuming getPointFlash returns the current pointFlash
    // Check other changes made by the method
}


TEST(BasicTest, TestWorks) {
    EXPECT_EQ(1, 1);
}

