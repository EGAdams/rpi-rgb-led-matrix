
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Undo.h"
#include "../History/History.h"
#include "../Player/Player.h"
#include "../Mode1Score/Mode1Score.h"

class UndoTest : public ::testing::Test {
protected:
    ScoreBoard* scoreBoard;
    Mode1Score* mode1Score;
    History* history;
    Undo* undoObj;
    Player* player1;
    Player* player2;
    GameState* gameState;
    PinInterface* pinInterface;
    PinState* pinState;
    std::map< std::string, int > pin_map;

    void SetUp() override {
        gameState = new GameState();
        player1 = new Player( gameState, PLAYER_1_INITIALIZED );
        player2 = new Player( gameState, PLAYER_2_INITIALIZED ); 
        player1->setOpponent( player2 ); 
        player2->setOpponent( player1 );
        player1->setServe( 1 ); 
        player2->setServe( 0 );
        gameState->setServe( 1 );
        gameState->setTieBreak( 0 );
        scoreBoard = new ScoreBoard( player1, player2, gameState );
        history = new History();
        pin_map = {{ "pin", 0 }};
        pinState = new PinState( pin_map );
        pinInterface = new PinInterface( pinState );
        undoObj = new Undo( player1, player2, pinInterface, gameState );
        undoObj->setScoreBoard( scoreBoard );
        mode1Score = new Mode1Score( player1, player2, pinInterface, gameState, history );
        mode1Score->setScoreBoard( scoreBoard );
    }

    void TearDown() override {
        delete mode1Score;
        delete undoObj;
        delete player1;
        delete player2;
        delete gameState;
        delete history; 
        delete pinInterface;
        delete pinState; 
        std::cout << "\n\n" << std::endl;
    }
};

TEST_F( UndoTest, TestSetMode1Undo ) {
    // Setup initial game state
    player1->setPoints( 3 );
    player2->setPoints( 2 );
    
    // Save the game state to history
    undoObj->setMode1Undo( history );

    // Change the game state
    player1->setPoints( 5 );
    player2->setPoints( 4 );

    // Verify the changed game state
    ASSERT_EQ( 5, player1->getPoints());
    ASSERT_EQ( 4, player2->getPoints());

    // Restore game state from history
    undoObj->mode1Undo( history );

    // Verify restored game state
    ASSERT_EQ( 3, player1->getPoints());
    ASSERT_EQ( 2, player2->getPoints());
}


TEST_F(UndoTest, TestMemoryFunction) {
    // Initialize the player and game state to some random values
    player1->setPoints( 2 );
    player1->setGames( 3 );
    player1->setSets( gameState, 1 );
    player2->setPoints( 3 );
    player2->setGames( 2 );
    player2->setSets( gameState, 0 );
    gameState->setTieBreak( 1 );
    gameState->setSetTieBreak( 0 );

    // Call the memory function
    undoObj->memory();

    // Check if the memory function has stored the correct values without modifying them
    ASSERT_EQ( 2, gameState->getP1PointsMem());
    ASSERT_EQ( 3, gameState->getP2PointsMem());
    ASSERT_EQ( 3, gameState->getP1GamesMem());
    ASSERT_EQ( 2, gameState->getP2GamesMem());
    ASSERT_EQ( 1, gameState->getP1SetsMem());
    ASSERT_EQ( 0, gameState->getP2SetsMem());
    ASSERT_EQ( 1, gameState->getTieBreakMem());
    ASSERT_EQ( 0, gameState->getSetTieBreakMem());
}

TEST_F( UndoTest, TestUndoFunctionality ) {
    // TODO: Add checks before the undo operation to verify initial state
    player1->setPoints( 2 );
    player2->setPoints( 2 );
    undoObj->setMode1Undo( history );
    player1->setPoints( 3 );
    mode1Score->updateScore( player1 );  // player 1 scores
    ASSERT_EQ( 3, player1->getPoints());
    ASSERT_EQ( 2, player2->getPoints());
    undoObj->mode1Undo( history );  
    std::cout << "Checking game state after undo..." << std::endl;
    ASSERT_EQ( 2, player1->getPoints()); // back to initial state before player 1 scores
    ASSERT_EQ( 2, player2->getPoints());
    std::cout << "Done checking game state after undo.  Done with TestUndoFunctionality" << std::endl;
}

TEST_F( UndoTest, TestPlayer2Scores ) {
    player2->setPoints( 2 ); // Incrementing player 2's score by 4
    mode1Score->updateScore( player2 );
    ASSERT_EQ( 2 , player2->getPoints());
    // TODO: Additional checks for game state after player 2 scores
}

TEST_F( UndoTest, TestPlayer1WinsSet ) {
    player1->setPoints( 2 );
    mode1Score->updateScore( player1 );
    player2->setPoints( 1 );
    mode1Score->updateScore( player2 );
    player1->setGames( 5 );
    player2->setGames( 4 );
    std::cout << "player 1 scores... << std::endl";
    player1->setPoints( 3 );
    mode1Score->updateScore( player1 );
    std::cout << "did player 1 win the set?" << std::endl;
    ASSERT_EQ( 1, player1->getSets());
    std::cout << "scores reset to zero?" << std::endl;
    ASSERT_EQ( 0, player1->getPoints());
    ASSERT_EQ( 0, player2->getPoints());
}

