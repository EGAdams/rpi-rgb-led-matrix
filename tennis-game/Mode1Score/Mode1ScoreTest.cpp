#include <gtest/gtest.h>
#include "Mode1Score.h"
#include "../ConsoleDisplay/ConsoleDisplay.h"

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
    print("constructing GameState...");
    _gameState = new GameState();
    print("creating Player 1...");
    _player1 = new Player(_gameState, PLAYER_1_INITIALIZED);
    print("creating Player 2...");
    _player2 = new Player(_gameState, PLAYER_2_INITIALIZED);
    print("setting opponents for players...");
    _player1->setOpponent(_player2); _player2->setOpponent(_player1);
    print("creating pin map...");
    std::map<std::string, int> pin_map;
    print("creating PinState...");
    PinState* pin_state = new PinState(pin_map);
    print("creating PinInterface...");
    _pinInterface = new PinInterface(pin_state);
    print("creating History...");
    _history = new History();
    print("creating Mode1Score...");
    _mode1Score = new Mode1Score(_player1, _player2, _pinInterface, _gameState, _history);
    print("initializing TieBreaker iteration...");
    _mode1Score->getTieBreaker()->setIteration(1);  // Initialize iteration to a valid value
    print("creating ScoreBoard...");
    // Assuming ScoreBoard is a class that needs to be instantiated
    FontManager* fontManager = new FontManager();
    ColorManager* colorManager = new ColorManager();
    IDisplay* display = new ConsoleDisplay( colorManager );
    ScoreBoard* scoreBoard = new ScoreBoard( _player1, _player2, _gameState, display, fontManager, colorManager);
    print("setting ScoreBoard for TieBreaker...");
    _mode1Score->getTieBreaker()->setScoreBoards(scoreBoard);
    print( "finished Mode1ScoreTest::SetUp()" );
}

// Tear down the test fixture
void Mode1ScoreTest::TearDown() {
    print("deleting _mode1Score...");
    delete _mode1Score;
    print("deleting _history...");
    delete _history;
    print("deleting _gameState...");
    delete _gameState;
    print("deleting _pinInterface...");
    delete _pinInterface;
    print("deleting _player2...");
    delete _player2;
    print("deleting _player1...");
    delete _player1;
    print( "done with Mode1ScoreTest::TearDown()" );
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
    _player1->setPoints( 3 );
    _mode1Score->updateScore( _player1 );
    _player2->setPoints( 3 );
    _mode1Score->updateScore( _player2 );
    _player1->setPoints( 4 );
    _mode1Score->updateScore( _player1 );
    _player1->setPoints( 5 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 0 );
    EXPECT_EQ( _player2->getPoints(), 0 );
    EXPECT_EQ( _player1->getGames(),  1 );
    print( "finished TestMode1P1Score_MoreThan3Points_DifferenceMoreThan1" );
}

// Test case: TestMode1P1Score_4Points
TEST_F( Mode1ScoreTest, TestMode1P1Score_4Points ) {
    print ( "*** starting TestMode1P1Score_4Points ***" );
    print( "setting player one points to 3... " );
    _player1->setPoints( 3 );
    print( "updating score for player one... " );
    _mode1Score->updateScore( _player1  );
    print( "setting player two points to 3... " );
    _player2->setPoints( 3 );
    print( "updating score for player two... " );
    _mode1Score->updateScore( _player2 );
    print( "setting player one points to 4... " );
    _player1->setPoints( 4 );
    print( "updating score for player one... " );
    _mode1Score->updateScore( _player1 );
    print( "checking if player one has 4 points... " );
    EXPECT_EQ( _player1->getPoints(), 4 );
    print( "checking if player two has 3 points... " );
    EXPECT_EQ( _player2->getPoints(), 3 );
    print( "checking if point flash is 1... " );
    EXPECT_EQ( _gameState->getPointFlash(), 1 ); // Assuming getPointFlash returns the current pointFlash
    // Check other changes made by the method
    print( "finished TestMode1P1Score_4Points tests." );
}

TEST_F( Mode1ScoreTest, TestMode1P1Score_Deuce ) {
    _player1->setPoints( 3 );
    _player2->setPoints( 3 );
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 3 );
    EXPECT_EQ( _player2->getPoints(), 3 );
    // Check other changes made by the method if any
}

// Test case: TestMode1P1Score_Advantage
TEST_F( Mode1ScoreTest, TestMode1P1Score_Advantage ) {
    _player1->setPoints(4);
    _player2->setPoints(3);
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 4 );
    EXPECT_EQ( _player2->getPoints(), 3 );
    // Check other changes made by the method if any
}

// Test case: TestMode1P1Score_WinAfterAdvantage
TEST_F( Mode1ScoreTest, TestMode1P1Score_WinAfterAdvantage ) {
    _player1->setPoints(4);
    _player2->setPoints(3);
    _mode1Score->updateScore( _player1 );  // Player 1 at advantage
    _player1->setPoints(5);
    _mode1Score->updateScore( _player1 );  // Player 1 wins the game
    EXPECT_EQ( _player1->getPoints(), 0 );
    EXPECT_EQ( _player2->getPoints(), 0 );
    EXPECT_EQ( _player1->getGames(), 1 );
    // Check other changes made by the method if any
}

// Test case: TestMode1P1Score_ScoreFromZeroPoints
TEST_F( Mode1ScoreTest, TestMode1P1Score_ScoreFromZeroPoints ) {
    _player1->setPoints(0);
    _player2->setPoints(2);
    _player1->setPoints(_player1->getPoints() + 1); // Simulate player 1 scores a point
    _mode1Score->updateScore( _player1 );  // Update the score based on new points
    EXPECT_EQ( _player1->getPoints(), 1 ); // Expected points after scoring
    EXPECT_EQ( _player2->getPoints(), 2 );
    // Check other changes made by the method if any
}

// Test case: TestMode1P1Score_WinAfterDeuce
TEST_F( Mode1ScoreTest, TestMode1P1Score_WinAfterDeuce ) {
    _player1->setPoints(3);
    _player2->setPoints(3);
    _mode1Score->updateScore( _player1 );  // Both players at deuce
    _player1->setPoints(4);  // Player 1 scores and gains advantage
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 4 );
    EXPECT_EQ( _player2->getPoints(), 3 );

    _player1->setPoints(5);  // Player 1 scores and wins the game
    _mode1Score->updateScore( _player1 );
    EXPECT_EQ( _player1->getPoints(), 0 );
    EXPECT_EQ( _player2->getPoints(), 0 );
    EXPECT_EQ( _player1->getGames(), 1 );
    // Check other changes made by the method if any
}

// Test case: TestMode1TieBreak_P1Wins
TEST_F(Mode1ScoreTest, TestMode1TieBreak_P1Wins) {
    // Simulate a tie-break scenario
    _gameState->setTieBreak(1);
    _player1->setPoints(6);
    _player2->setPoints(6);
    _mode1Score->updateScore(_player1);  // Player 1 scores, now 7-6
    EXPECT_EQ(_player1->getPoints(), 6);
    EXPECT_EQ(_player2->getPoints(), 6);

    // Player 1 must win by at least two points
    _mode1Score->updateScore(_player1);  // Player 1 scores again, now 8-6, wins the tie-break and set
    EXPECT_EQ(_player1->getPoints(), 6);
    EXPECT_EQ(_player2->getPoints(), 6);
    EXPECT_EQ(_player1->getGames(), 0);
    EXPECT_EQ(_player2->getGames(), 0);
    EXPECT_EQ(_player1->getSets(), 0);
}
