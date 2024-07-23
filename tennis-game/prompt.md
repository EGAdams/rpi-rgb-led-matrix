
# TieBreakerTest.cpp
```cpp
#include <gtest/gtest.h>
#include "TieBreaker.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../GameObject/GameObject.h" // Ensure GameObject header is included

class TieBreakerTest : public ::testing::Test {
protected:
    Player* _player1;
    Player* _player2;
    GameState* _gameState;
    PinInterface* _pinInterface;
    TieBreaker* _tieBreaker;
    GameObject* _gameObject; // Add GameObject variable

    void SetUp() override {
        _gameState = new GameState();
        _player1 = new Player(_gameState, PLAYER_1_INITIALIZED);
        _player2 = new Player(_gameState, PLAYER_2_INITIALIZED);
        _player1->setOpponent(_player2);
        _player2->setOpponent(_player1);
        std::map<std::string, int> pin_map;
        PinState* pin_state = new PinState(pin_map);
        _pinInterface = new PinInterface(pin_state);
        _tieBreaker = new TieBreaker(_player1, _player2, _pinInterface, _gameState, NULL);
        _gameObject = new GameObject(_player1, _player2, pin_state, _pinInterface, _gameState, nullptr, nullptr, nullptr, nullptr, nullptr); // Initialize GameObject
    }

    void TearDown() override {
        delete _tieBreaker;
        delete _pinInterface;
        delete _player2;
        delete _player1;
        delete _gameState;
        delete _gameObject; // Clean up GameObject
    }
};

TEST_F(TieBreakerTest, SetUpSetTieBreakScenario) {
    _gameObject->getScoreBoard()->clearScreen();
    _player1->setSetHistory(1, 6);
    _player2->setSetHistory(1, 4);
    _player1->setSetHistory(2, 5);
    _player2->setSetHistory(2, 5);
    _gameState->setCurrentSet(2);
    _player1->setGames(5);
    _player2->setGames(5);
    _player1->setPoints(2);
    _player2->setPoints(3);
    
    std::cout << "Updating scoreboard..." << std::endl;
    _gameObject->getScoreBoard()->update();
    std::cout << "Done updating scoreboard." << std::endl;

    // Simulate Player 2 winning the next point
    _player2->setPoints(4);
    std::cout << "Player 2 scores the next point..." << std::endl;
    
    // Check if Player 2 wins the set and if the tie-breaker is ready
    if (_player2->getPoints() >= 4 && (_player2->getPoints() - _player1->getPoints()) >= 2) {
        _player2->setGames(_player2->getGames() + 1);
        EXPECT_EQ(_player2->getGames(), 6);  // Player 2 should now have 6 games
    }

    // Update and check the tie-breaker initiation
    _tieBreaker->initializeTieBreakMode();
    EXPECT_EQ(_tieBreaker->getIteration(), 1);
    EXPECT_EQ(_player1->getPoints(), 0);
    EXPECT_EQ(_player2->getPoints(), 0);
    EXPECT_EQ(_gameState->getServeSwitch(), 1);
    EXPECT_EQ(_gameState->getServe(), 0);
    EXPECT_EQ(_gameState->getTieLEDsOn(), 1);
    std::cout << "Tie-breaker mode initialized." << std::endl;
}
```

# Makefile Source
```bash
# Add the GameObject.o to TEST_OBJECTS
TEST_OBJECTS=../TieLeds/TieLeds.o ../GameLedTranslator/GameLedTranslator.o ../GameState/GameState.o ../Player/Player.o ../GameTimer/GameTimer.o ../ScoreBoard/ScoreBoard.o ../SetDrawer/SetDrawer.o ../SetHistoryText/SetHistoryText.o ../Drawer/Drawer.o ../CanvasCreator/CanvasCreator.o ../FontLoader/FontLoader.o ../LogObject/LogObject.o ../LogObjectContainer/LogObjectContainer.o ../LogObjectFactory/LogObjectFactory.o ../MonitorLedClassObject/MonitorLedClassObject.o ../MonitorLed/MonitorLed.o ../JsonParser/JsonParser.o ../LoggerFactory/LoggerFactory.o ../Model/Model.o ../MonitoredObject/MonitoredObject.o ../SourceData/SourceData.o ../FetchRunner/FetchRunner.o ../PinInterface/PinInterface.o ../PinState/PinState.o ../History/History.o ../Mode1Score/Mode1Score.o TieBreakerTest.o ../TranslateConstant/TranslateConstant.o ../Logger/Logger.o TieBreaker.o ../PointLeds/PointLeds.o ../GameLeds/GameLeds.o ../SetLeds/SetLeds.o ../WinSequences/WinSequences.o ../Undo/Undo.o ../ServeLeds/ServeLeds.o ../GameWinSequence/GameWinSequence.o ../SetWin/SetWin.o ../MatchWinSequence/MatchWinSequence.o ../Inputs/Inputs.o ../WatchTimer/WatchTimer.o ../Reset/Reset.o ../GameObject/GameObject.o # Add this line

all: TieBreakerTest

TieBreakerTest: $(TEST_OBJECTS)
        $(CXX) $(CXXFLAGS) -o TieBreakerTest $(TEST_OBJECTS) $(LDFLAGS)

TieBreakerTest.o: TieBreakerTest.cpp
        $(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

TieBreaker.o: TieBreaker.cpp
        $(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
        rm -f $(TEST_OBJECTS) TieBreakerTest

FORCE:
.PHONY: FORCE

print-%:
        @echo $* = $($*)

```
 
# Make Output
```bash
g++ -I  -c -o TieBreakerTest.o TieBreakerTest.cpp
/usr/bin/ld: /usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/Scrt1.o: in function `_start':
(.text+0x17): undefined reference to `main'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `TieBreakerTest_SetUpSetTieBreakScenario_Test::TestBody()':
TieBreakerTest.cpp:(.text+0x22): undefined reference to `GameObject::getScoreBoard()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x2a): undefined reference to `ScoreBoard::clearScreen()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x47): undefined reference to `Player::setSetHistory(int, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x64): undefined reference to `Player::setSetHistory(int, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x81): undefined reference to `Player::setSetHistory(int, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x9e): undefined reference to `Player::setSetHistory(int, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0xb6): undefined reference to `GameState::setCurrentSet(int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0xce): undefined reference to `Player::setGames(int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0xe6): undefined reference to `Player::setGames(int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0xfe): undefined reference to `Player::setPoints(int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x116): undefined reference to `Player::setPoints(int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x154): undefined reference to `GameObject::getScoreBoard()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x15c): undefined reference to `ScoreBoard::update()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x19f): undefined reference to `Player::setPoints(int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x1dd): undefined reference to `Player::getPoints()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x1f5): undefined reference to `Player::getPoints()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x20a): undefined reference to `Player::getPoints()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x245): undefined reference to `Player::getGames()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x252): undefined reference to `Player::setGames(int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x26f): undefined reference to `Player::getGames()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x2c8): undefined reference to `testing::Message::Message()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x2fd): undefined reference to `testing::internal::AssertHelper::AssertHelper(testing::TestPartResult::Type, char const*, int, char const*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x316): undefined reference to `testing::internal::AssertHelper::operator=(testing::Message const&) const'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x325): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x356): undefined reference to `TieBreaker::initializeTieBreakMode()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x373): undefined reference to `TieBreaker::getIteration()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x3cc): undefined reference to `testing::Message::Message()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x401): undefined reference to `testing::internal::AssertHelper::AssertHelper(testing::TestPartResult::Type, char const*, int, char const*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x41a): undefined reference to `testing::internal::AssertHelper::operator=(testing::Message const&) const'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x429): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x464): undefined reference to `Player::getPoints()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x4ba): undefined reference to `testing::Message::Message()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x4ec): undefined reference to `testing::internal::AssertHelper::AssertHelper(testing::TestPartResult::Type, char const*, int, char const*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x4ff): undefined reference to `testing::internal::AssertHelper::operator=(testing::Message const&) const'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x50b): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x540): undefined reference to `Player::getPoints()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x58d): undefined reference to `testing::Message::Message()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x5bf): undefined reference to `testing::internal::AssertHelper::AssertHelper(testing::TestPartResult::Type, char const*, int, char const*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x5d2): undefined reference to `testing::internal::AssertHelper::operator=(testing::Message const&) const'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x5de): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x613): undefined reference to `GameState::getServeSwitch()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x660): undefined reference to `testing::Message::Message()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x692): undefined reference to `testing::internal::AssertHelper::AssertHelper(testing::TestPartResult::Type, char const*, int, char const*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x6a5): undefined reference to `testing::internal::AssertHelper::operator=(testing::Message const&) const'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x6b1): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x6e6): undefined reference to `GameState::getServe()'
... truncated output...
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x7b9): undefined reference to `GameState::getTieLEDsOn()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x806): undefined reference to `testing::Message::Message()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x838): undefined reference to `testing::internal::AssertHelper::AssertHelper(testing::TestPartResult::Type, char const*, int, char const*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x84b): undefined reference to `testing::internal::AssertHelper::operator=(testing::Message const&) const'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x857): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x8b4): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x8f9): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x93b): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: TieBreakerTest.cpp:(.text+0x97a): undefined reference to `testing::internal::AssertHelper::~AssertHelper()'
/usr/bin/ld: /tmp/ccG1SlFT.o:TieBreakerTest.cpp:(.text+0x9b9): more undefined references to `testing::internal::AssertHelper::~AssertHelper()' follow
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `__static_initialization_and_destruction_0(int, int)':
TieBreakerTest.cpp:(.text+0xbcd): undefined reference to `testing::internal::MakeAndRegisterTestInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char const*, char const*, char const*, testing::internal::CodeLocation, void const*, void (*)(), void (*)(), testing::internal::TestFactoryBase*)'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `TieBreakerTest::SetUp()':
TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0x2e): undefined reference to `GameState::GameState()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0x67): undefined reference to `Player::Player(GameState*, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0xa0): undefined reference to `Player::Player(GameState*, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0xcc): undefined reference to `Player::setOpponent(Player*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0xed): undefined reference to `Player::setOpponent(Player*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0x134): undefined reference to `PinState::PinState(std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int> > >)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0x166): undefined reference to `PinInterface::PinInterface(PinState*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0x1c4): undefined reference to `TieBreaker::TieBreaker(Player*, Player*, PinInterface*, GameState*, History*)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest5SetUpEv[_ZN14TieBreakerTest5SetUpEv]+0x231): undefined reference to `GameObject::GameObject(Player*, Player*, PinState*, PinInterface*, GameState*, GameTimer*, Inputs*, GameModes*, ScoreBoard*, WebLiquidCrystal*)'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `TieBreakerTest::TearDown()':
TieBreakerTest.cpp:(.text._ZN14TieBreakerTest8TearDownEv[_ZN14TieBreakerTest8TearDownEv]+0x1e): undefined reference to `TieBreaker::~TieBreaker()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest8TearDownEv[_ZN14TieBreakerTest8TearDownEv]+0x40): undefined reference to `PinInterface::~PinInterface()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest8TearDownEv[_ZN14TieBreakerTest8TearDownEv]+0x62): undefined reference to `Player::~Player()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest8TearDownEv[_ZN14TieBreakerTest8TearDownEv]+0x84): undefined reference to `Player::~Player()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest8TearDownEv[_ZN14TieBreakerTest8TearDownEv]+0xa6): undefined reference to `GameState::~GameState()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN14TieBreakerTest8TearDownEv[_ZN14TieBreakerTest8TearDownEv]+0xc8): undefined reference to `GameObject::~GameObject()'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `testing::internal::SuiteApiResolver<TieBreakerTest>::GetSetUpCaseOrSuite(char const*, int)':
TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci]+0x6a): undefined reference to `testing::internal::IsTrue(bool)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci]+0x8f): undefined reference to `testing::internal::GTestLog::GTestLog(testing::internal::GTestLogSeverity, char const*, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci]+0x108): undefined reference to `testing::internal::GTestLog::~GTestLog()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE19GetSetUpCaseOrSuiteEPKci]+0x12a): undefined reference to `testing::internal::GTestLog::~GTestLog()'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `testing::internal::SuiteApiResolver<TieBreakerTest>::GetTearDownCaseOrSuite(char const*, int)':
TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci]+0x6a): undefined reference to `testing::internal::IsTrue(bool)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci]+0x8f): undefined reference to `testing::internal::GTestLog::GTestLog(testing::internal::GTestLogSeverity, char const*, int)'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci]+0x108): undefined reference to `testing::internal::GTestLog::~GTestLog()'
/usr/bin/ld: TieBreakerTest.cpp:(.text._ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci[_ZN7testing8internal16SuiteApiResolverI14TieBreakerTestE22GetTearDownCaseOrSuiteEPKci]+0x12a): undefined reference to `testing::internal::GTestLog::~GTestLog()'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `testing::AssertionResult testing::internal::CmpHelperEQ<int, int>(char const*, char const*, int const&, int const&)':
TieBreakerTest.cpp:(.text._ZN7testing8internal11CmpHelperEQIiiEENS_15AssertionResultEPKcS4_RKT_RKT0_[_ZN7testing8internal11CmpHelperEQIiiEENS_15AssertionResultEPKcS4_RKT_RKT0_]+0x34): undefined reference to `testing::AssertionSuccess()'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `testing::AssertionResult testing::internal::CmpHelperEQFailure<int, int>(char const*, char const*, int const&, int const&)':
TieBreakerTest.cpp:(.text._ZN7testing8internal18CmpHelperEQFailureIiiEENS_15AssertionResultEPKcS4_RKT_RKT0_[_ZN7testing8internal18CmpHelperEQFailureIiiEENS_15AssertionResultEPKcS4_RKT_RKT0_]+0x6c): undefined reference to `testing::internal::EqFailure(char const*, char const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, bool)'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `TieBreakerTest::~TieBreakerTest()':
TieBreakerTest.cpp:(.text._ZN14TieBreakerTestD2Ev[_ZN14TieBreakerTestD5Ev]+0x22): undefined reference to `testing::Test::~Test()'
/usr/bin/ld: /tmp/ccG1SlFT.o:(.data.rel.ro._ZTI14TieBreakerTest[_ZTI14TieBreakerTest]+0x10): undefined reference to `typeinfo for testing::Test'
/usr/bin/ld: /tmp/ccG1SlFT.o: in function `TieBreakerTest::TieBreakerTest()':
TieBreakerTest.cpp:(.text._ZN14TieBreakerTestC2Ev[_ZN14TieBreakerTestC5Ev]+0x14): undefined reference to `testing::Test::Test()'
collect2: error: ld returned 1 exit status
make: *** [Makefile:10: TieBreakerTest.o] Error 1
```
Please help me debug this.
