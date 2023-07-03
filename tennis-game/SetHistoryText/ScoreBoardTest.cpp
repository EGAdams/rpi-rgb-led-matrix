#include "../googletest/googletest/include/gtest/gtest.h"
#include "./SetHistoryText.h"

class SetHistoryTextTest : public ::testing::Test {
 protected:
    SetHistoryTextTest() {}           // You can do set-up work for each test here.
    ~SetHistoryTextTest() override {} // You can do clean-up work that doesn't throw exceptions here.
       
    void SetUp() override { // Code here will be called immediately after the constructor (right before each test).
        gameState = new GameState();
        player_1 = new Player( PLAYER_1_INITIALIZED ); 
        player_2 = new Player( PLAYER_2_INITIALIZED ); // got players defined, now set echother as opponents...
        player_2->setOpponent( player_1 ); player_1->setOpponent( player_2 );
        setHistoryText = new SetHistoryText( gameState ); 
        scoreboard     = new ScoreBoard( player_1, player_2, gameState ); }

    void TearDown() override { // Code here will be called immediately after each test (right before the destructor).
        delete setHistoryText;
        delete player_2;
        delete player_1;
        delete gameState; 
        delete scoreboard; }

    Player*         player_1;
    Player*         player_2;
    GameState*      gameState;
    SetHistoryText* setHistoryText; 
    ScoreBoard*     scoreboard; };

TEST_F( SetHistoryTextTest, TestGetSetHistoryText ) {
    // std::map< int, int > set_history = {{ 0, 3 }, { 1, 2 }, { 2, 4 }};
    player_1->setSetHistory( SET_HISTORY_COLUMN_1, 5 );
    player_1->setSetHistory( SET_HISTORY_COLUMN_2, 4 );
    player_1->setSetHistory( SET_HISTORY_COLUMN_3, 2 );
    gameState->setPlayer1SetHistory( player_1->getSetHistory());
    // set_history = {{ 0, 4 }, { 1, 5 }, { 2, 30 }};
    player_2->setSetHistory( SET_HISTORY_COLUMN_1, 4 );
    player_2->setSetHistory( SET_HISTORY_COLUMN_2, 5 );
    player_2->setSetHistory( SET_HISTORY_COLUMN_3, 30 );

    gameState->setPlayer2SetHistory( player_2->getSetHistory());
    scoreboard->update();

    std::cout << "sets should be showing.  delaying 5 seconds..."
    GameTimer gameTimer = new GameTimer();
    gameTimer.gameDelay( 5000 );
    std::cout << "done delaying.  end of test."

    // std::string result = setHistoryText->getSetHistoryText( 1 );
    // ASSERT_EQ(result, "15 10 40 ");
    // result = setHistoryText->getSetHistoryText( 2 );
    // ASSERT_EQ(result, "4 5 30 "); 
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); }
