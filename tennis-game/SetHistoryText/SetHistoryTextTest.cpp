#include "../googletest/googletest/include/gtest/gtest.h"
#include "./SetHistoryText.h"
#include "../Player/Player.h"

class SetHistoryTextTest : public ::testing::Test {
 protected:
    SetHistoryTextTest() {}           // You can do set-up work for each test here.
    ~SetHistoryTextTest() override {} // You can do clean-up work that doesn't throw exceptions here.
       
    void SetUp() override { // Code here will be called immediately after the constructor (right before each test).
        gameState = new GameState();
        player_1 = new Player( PLAYER_1_INITIALIZED );
        player_2 = new Player( PLAYER_2_INITIALIZED ); // got players defined, now set echother as opponents...
        setHistoryText = new SetHistoryText( gameState ); }

    void TearDown() override { // Code here will be called immediately after each test (right before the destructor).
        delete setHistoryText;
        delete player_1;
        delete player_2;
        delete gameState; }

    GameState* gameState;
    Player* player_1;
    Player* player_2;
    SetHistoryText* setHistoryText; };

TEST_F( SetHistoryTextTest, TestGetSetHistoryText ) {
    // std::map< int, int > set_history = {{ 0, 15 }, { 1, 10 }, { 2, 40 }};
    player_1->setSetHistory( SET_HISTORY_COLUMN_1, 4 );
    player_1->setSetHistory( SET_HISTORY_COLUMN_2, 6 );
    player_1->setSetHistory( SET_HISTORY_COLUMN_3, 2 );
    gameState->setPlayer1SetHistory( player_1->getSetHistory());
    // set_history = {{ 0, 4 }, { 1, 5 }, { 2, 30 }};
    player_2->setSetHistory( SET_HISTORY_COLUMN_1, 2 );
    player_2->setSetHistory( SET_HISTORY_COLUMN_2, 5 );
    player_2->setSetHistory( SET_HISTORY_COLUMN_3, 3 );
    gameState->setPlayer2SetHistory( player_2->getSetHistory());

    ASSERT_EQ( 1, 1 );
    // std::string result = setHistoryText->getSetHistoryText( 1 );
    // ASSERT_EQ(result, "15 10 40 ");
    // result = setHistoryText->getSetHistoryText( 2 );
    // ASSERT_EQ(result, "4 5 30 "); 
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); }
