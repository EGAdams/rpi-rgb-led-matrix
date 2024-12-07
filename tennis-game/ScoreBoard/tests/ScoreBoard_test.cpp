#include <gtest/gtest.h>
#include "../ScoreBoard.h"
#include <memory>

class ScoreBoardTest : public ::testing::Test {
protected:
    std::unique_ptr<ScoreBoard> scoreBoard;

    void SetUp() override {
        scoreBoard = std::make_unique<ScoreBoard>();
    }
};

TEST_F(ScoreBoardTest, DrawYellowPeriod_NotOnRaspberryPi) {
    // Mock the isRaspberryPi function to return false
    EXPECT_CALL(*scoreBoard, isRaspberryPi()).WillOnce(testing::Return(false));

    // The function should return early without calling drawNumber
    EXPECT_CALL(*scoreBoard->_yellowPeriodDrawer, drawNumber(testing::_, testing::_, testing::_)).Times(0);

    scoreBoard->drawYellowPeriod();
}
TEST_F(ScoreBoardTest, DrawYellowPeriod_OnRaspberryPi) {
    // Mock the onRaspberryPi function to return true
    EXPECT_CALL(*scoreBoard, onRaspberryPi()).WillOnce(testing::Return(true));

    // Expect drawNumber to be called with correct parameters
    EXPECT_CALL(*scoreBoard->_yellowPeriodDrawer, drawNumber(".", 53, 109)).Times(1);

    scoreBoard->drawYellowPeriod();
}

TEST_F(ScoreBoardTest, DrawYellowPeriod_CorrectOffsets) {
    // Mock the onRaspberryPi function to return true
    EXPECT_CALL(*scoreBoard, onRaspberryPi()).WillOnce(testing::Return(true));

    // Capture the arguments passed to drawNumber
    EXPECT_CALL(*scoreBoard->_yellowPeriodDrawer, drawNumber(testing::_, testing::_, testing::_))
        .WillOnce(testing::Invoke([](const std::string& str, int x, int y) {
            EXPECT_EQ(str, ".");
            EXPECT_EQ(x, 53);
            EXPECT_EQ(y, 109);
        }));

    scoreBoard->drawYellowPeriod();
}
