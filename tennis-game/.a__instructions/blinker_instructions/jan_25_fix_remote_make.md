Her is the C++ file that I need to build:
```cpp
#include "../include/led-matrix.h"
#include "../include/graphics.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <csignal>
#include <map>
#include "FontLoader/FontLoader.h"
#include "ITextDrawer/ITextDrawer.h"
#include "Drawer/Drawer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "GameObject/GameObject.h"
#include "LoggerFactory/LoggerFactory.h"
#include "IInputWithTimer/IInputWithTimer.h"
#include "TennisConstants/TennisConstants.h"
#include "RemotePairingScreen/RemotePairingScreen.h"
#include "PairingBlinker/PairingBlinker.h"
#include "ScoreboardBlinker/ScoreboardBlinker.h"
#include "ConsoleDisplay/ConsoleDisplay.h"
#include "RemoteLocker/RemoteLocker.h"
#include "PinState/PinState.h"
#include "KeyboardInputWithTimer/KeyboardInputWithTimer.h"
#include "IInputWithTimer/IInputWithTimer.h"
#include "RemoteInputWithTimer/RemoteInputWithTimer.h"
#include "BlankBlinker/BlankBlinker.h"
#include "IGameInput/IGameInput.h"
#include "RemoteGameInput/RemoteGameInput.h"
#include "KeyboardGameInput/KeyboardGameInput.h"


// for the expander
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

// to set the bits
#include <bitset>

using namespace rgb_matrix;

/*==========================================================
 *  Global signal variable to mimic the code's usage
 *==========================================================*/
static volatile std::sig_atomic_t gSignalStatus = 0;
#define SCORE_DELAY    0

bool is_on_raspberry_pi() {
    std::ifstream file( "/proc/device-tree/model" );
    std::string line;
    if ( file.is_open()) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}

void run_remote_listener( GameObject* gameObject, GameState* gameStatearg, Reset* reset ) {
    int KEYBOARD_TIMEOUT = 120000;
    GameState* gameState = gameStatearg;
    RemoteLocker*       remoteLocker = new RemoteLocker( gameState );
    IInputWithTimer*    pairingInputWithTimer;
    IInputWithTimer*    noBlinkInputWithTimer;
    IInputWithTimer*    sleepingInputWithTimer;
    IGameInput*         gameInput;

    Inputs*             inputs = new Inputs( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    bool no_score = true;
    print( "entered run remote listener method..." );
    int loop_count = 0;
    int selection = 0;
    print( "calling game object loop game..." );
    gameObject->loopGame();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ));
    

    // Program to the IScoreBoard interface
    auto scoreboard = gameObject->getScoreBoard();
    scoreboard->setLittleDrawerFont( "fonts/8x13B.bdf" );

    std::signal( SIGINT, GameObject::_signalHandler );

    print ( " constructing blinkers... " );
    RemotePairingScreen*    remotePairingScreen = new RemotePairingScreen( scoreboard );
    PairingBlinker*         pairingBlinker      = new PairingBlinker( scoreboard );
    BlankBlinker*           blankBlinker        = new BlankBlinker();
    ScoreboardBlinker*      sleepingBlinker     = new ScoreboardBlinker( scoreboard );

    unsigned long pairing_timer   = 4000;
    unsigned long no_blink_timer  = 4000;
    unsigned long sleeping_timer  = 4000;

    bool is_on_pi = scoreboard->onRaspberryPi();
    // checking pairing blinker for null state
    if ( pairingBlinker == nullptr ) {
        print( "*** ERROR: pairingBlinker is null! ***" );
    } else {
        print( "*** pairingBlinker is not null, continuing... ***" );
    }
    print( "is_on_pi: " + std::to_string( is_on_pi ));
    if ( is_on_pi ) {
        pairingInputWithTimer       = new RemoteInputWithTimer( pairingBlinker, inputs, pairing_timer   );
        noBlinkInputWithTimer       = new RemoteInputWithTimer( blankBlinker, inputs,   no_blink_timer  );
        sleepingInputWithTimer      = new RemoteInputWithTimer( sleepingBlinker, inputs, sleeping_timer );
        gameInput                   = new RemoteGameInput( inputs );
    } else {
        pairingInputWithTimer       = new KeyboardInputWithTimer( pairingBlinker, KEYBOARD_TIMEOUT      );
        noBlinkInputWithTimer       = new KeyboardInputWithTimer( blankBlinker, KEYBOARD_TIMEOUT        );
        sleepingInputWithTimer      = new KeyboardInputWithTimer( sleepingBlinker, KEYBOARD_TIMEOUT     );
        gameInput                   = new KeyboardGameInput(                         );     
    }
    while ( gameState->gameRunning() && gSignalStatus != SIGINT ) { /*/// Begin Game Loop ///*/
        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));

        // if remote pairing, write the words.  if not, snap out of the loop
        print( "in pairing mode? " + std::to_string( remotePairingScreen->inPairingMode()));
        print( "pairingBlinker->awake(): " + std::to_string( pairingBlinker->awake()));
        while ( remotePairingScreen->inPairingMode() && pairingBlinker->awake() ) {
            print( "inside remote pairing screen from run manual game.  before starting input timer..." );
            selection = pairingInputWithTimer->getInput();
            if ( selection == GREEN_REMOTE_GREEN_SCORE ) {
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired( true );
            } else if ( selection == RED_REMOTE_RED_SCORE ) {
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired( true );
            } else {
                print( "*** Invalid selection during pairing. *** 012525\n\n\n" );
                GameTimer::gameDelay( 1000 );
            }
        }

        if ( !pairingBlinker->awake() ) {
            print( "pairing blinker is not awake, stopping it... " );
            pairingBlinker->stop();
            print( "pairing blinker stopped.  now putting in sleep mode..." );
            gameState->setCurrentAction( SLEEP_MODE );
        }

        if ( gameState->getCurrentAction() == SLEEP_MODE ) {
            sleepingInputWithTimer->getInput();                 // blocks here for so many seconds
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "time slept: " << sleepingInputWithTimer->getTimeSlept() );

            if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == GREEN_REMOTE_RED_SCORE ) {
                print( "reset match." );
                gameObject->resetMatch();
                print( "done resetting match." );
                if ( sleepingInputWithTimer->getTimeSlept() > MAX_SLEEP * 1000 ) {
                    print( "time slept: " + std::to_string( sleepingInputWithTimer->getTimeSlept() / 1000 ) + " seconds." );
                    print( "clearing History because max sleep time has been reached or exceeded." );
                    gameObject->getHistory()->clearHistory();
                    print( "done clearing history because max sleep time has been reached or exceeded." );
                }
                continue;
            }
            print( "setting game state current action to after sleep mode" );
            gameState->setCurrentAction( AFTER_SLEEP_MODE );
            print( "*** Going into last Match! ***" );
            print( "clearing screen..." );
            scoreboard->clearScreen();
            print( "cleared screen." );
            scoreboard->update();
            print( "updated scoreboard." );
        }                       // else NOT in SLEEP_MODE

        // we still need to set the time for a no score mode
        if ( no_score ) {   // if there is no score, we need to time the input for the Zero score timeout.
            selection = noBlinkInputWithTimer->getInput();
            print( "selection: " + std::to_string( selection ));

            /* CHECK FOR TIMEOUT */
            // if timed out, selection is the timeout value, not a valid game input.  the timeout value is no where near a valid game input.    
            if ( selection > 1000 ) {                       // this is a Zero score timeout
                print( "*** Zero ScoreTimeout!  going to sleep mode... ***" );
                gameState->setCurrentAction( SLEEP_MODE );  // set to sleep mode and go to the beginning
                continue;
            }
        }

        if ( no_score ) { 
            print( "setting no_score to false..." ); no_score = false; // no need for the zero score timer anymore
        } else {
            selection = gameInput->getInput();  // this is either cin << or a remote call, both block until input is made
        }                                       // there are no timers in regular game play at the time of this writing
                                                // January 25, 2025  mass deportation began.  it was very cold in FL last night.
        if ( selection == 0 ) {  // not sure if we need this here, but it won't hurt for now - 012525
            print( "\n\nselection: " + std::to_string( selection ) + " ***\n\n" );
            print( "*** Invalid selection! ***  continuing..." );
            continue;
        }

        int serve_flag = remoteLocker->playerNotServing( selection );
        print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
        if ( serve_flag ) {
            print( "*** Warning: player not serving! ***" );
            continue;  // do not want to kill the no score flag quite yet
        }

        print( "setting player button to selection: " + std::to_string( selection ) + " before calling loopGame()..." );
        if ( selection == GREEN_REMOTE_GREEN_SCORE ||
             selection == GREEN_REMOTE_RED_SCORE   ||
             selection == RED_REMOTE_GREEN_SCORE   ||
             selection == RED_REMOTE_RED_SCORE ) {
            if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE ) {
                print( "*** \n\n\nGreen player scored ***\n\n\n" ); 
                selection = 1; // represent GREEN
            } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE ) {
                print( "\n\n\n*** Red player scored ***\n\n\n" );
                selection = 2; // represent RED
            }
            gameObject->playerScore( selection );
        }
        else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
            print( "\n\n\n*** Undo ***\n\n\n" );
            gameObject->undo();
        }
        else {
            print( "\n\n\n*** Invalid selection ***\n\n\n" );
            print( "------------" );
            print( "GREEN REMOTE: " );
            print( "   green remote green score: " <<  GREEN_REMOTE_GREEN_SCORE );
            print( "or green remote, red score: " << GREEN_REMOTE_RED_SCORE );
            print( " ------------ \n");
            print( "RED REMOTE: " );
            print( "or red remote, green score: " << RED_REMOTE_GREEN_SCORE );
            print( "or red remote, red score: " << RED_REMOTE_RED_SCORE );
            print( " ------------ \n");
            std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
            continue;
        }

        std::this_thread::sleep_for( std::chrono::seconds( SCORE_DELAY ));
        gameObject->loopGame();  // handle the player score flag
        loop_count++;

        // Just to mimic the original code's retrieval of set history
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
    } ///////// End Game Loop /////////
}

int main( int argc, char* argv[] ) {
    std::unique_ptr<MonitoredObject> logger = LoggerFactory::createLogger( "TestLogger" );
    // int mode = 0; // used to compile the --manual option.  not used as of 122924
    if ( argc > 1 ) {
        std::string arg1 = argv[1];
        if ( arg1 == "--manual" ) {
            print( "running menu mode..." );
            // mode = 1;
        }
        else {
            print( "running remote listening mode..." );
        }
    }
    print( "creating game state object..." );
    GameState* gameState = new GameState();  // make this 1st!!! cost me 3 days
    print( "creating game object..." );
    // FontManager* fontManager = new FontManager();
    ColorManager* colorManager = new ColorManager();
    bool isOnPi = is_on_raspberry_pi();
    print( "isOnPi: " << isOnPi );
    IDisplay* display = new ConsoleDisplay( colorManager );
    if ( isOnPi ) {
        print( "creating display object with matrix display..." );
    }
    else {
        print( "creating display object with console display..." );
        display = new ConsoleDisplay( colorManager );
    }
    GameObject* gameObject = new GameObject( gameState, display );
    print( "creating reset object..." );
    Reset* reset = new Reset( gameObject->getPlayer1(), gameObject->getPlayer2(), gameObject->getPinInterface(), gameState );
    run_remote_listener( gameObject, gameState, reset ); return 0;
}
```

Here is the Makefile that I am trying to use to compile this code:
```makefile
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -g -I../include
LDFLAGS = -L../lib -lrgbmatrix -lrt -lm -lpthread

# Target executable
TARGET = run_remote_listener

# Source files
SOURCES = \
run_remote_listener.cpp \
BatteryTest/BatteryTest.cpp \
BlankBlinker/BlankBlinker.cpp \
CanvasCreator/CanvasCreator.cpp \
ColorManager/ColorManager.cpp \
ConsoleDisplay/ConsoleDisplay.cpp \
Drawer/Drawer.cpp \
FontLoader/FontLoader.cpp \
GameLedTranslator/GameLedTranslator.cpp \
GameLeds/GameLeds.cpp \
GameModes/GameModes.cpp \
GameObject/GameObject.cpp \
GameState/GameState.cpp \
GameTimer/GameTimer.cpp \
History/History.cpp \
IInputWithTimer/IInputWithTimer.cpp \
Inputs/Inputs.cpp \
KeyboardGameInput/KeyboardGameInput.cpp \
KeyboardInputWithTimer/KeyboardInputWithTimer.cpp \
Logger/Logger.cpp \
LoggerFactory/LoggerFactory.cpp \
MatchWinSequence/MatchWinSequence.cpp \
Mode1Functions/Mode1Functions.cpp \
Mode1Score/Mode1Score.cpp \
Mode2Functions/Mode2Functions.cpp \
MonitoredObject/MonitoredObject.cpp \
PairingBlinker/PairingBlinker.cpp \
PinInterface/PinInterface.cpp \
PinState/PinState.cpp \
Player/Player.cpp \
PointLeds/PointLeds.cpp \
RemoteGameInput/RemoteGameInput.cpp \
RemoteInputWithTimer/RemoteInputWithTimer.cpp \
RemoteLocker/RemoteLocker.cpp \
RemotePairingScreen/RemotePairingScreen.cpp \
Reset/Reset.cpp \
ScoreBoard/ScoreBoard.cpp \
ScoreboardBlinker/ScoreboardBlinker.cpp \
ServeLeds/ServeLeds.cpp \
SetDrawer/SetDrawer.cpp \
SetHistoryText/SetHistoryText.cpp \
SetLeds/SetLeds.cpp \
SetWin/SetWin.cpp \
SubjectManager/SubjectManager.cpp \
TieBreaker/TieBreaker.cpp \
TieLeds/TieLeds.cpp \
TranslateConstant/TranslateConstant.cpp \
Undo/Undo.cpp \
WatchTimer/WatchTimer.cpp \
WebLiquidCrystal/WebLiquidCrystal.cpp \
WinSequences/WinSequences.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Build target
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Clean target
clean:
	rm -f $(TARGET) $(OBJECTS)

```

Here is the output of the make command:
```bash
adamsl@DESKTOP-SHDBATI:~/rpi-rgb-led-matrix/tennis-game$ make -f Makefile.remote_listener -k 2>&1 | tee /dev/tty | grep -c "error" | awk '{print "*** Make Error Count: " $1 " ***"}'
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o run_remote_listener.o run_remote_listener.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o BatteryTest/BatteryTest.o BatteryTest/BatteryTest.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o BlankBlinker/BlankBlinker.o BlankBlinker/BlankBlinker.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o CanvasCreator/CanvasCreator.o CanvasCreator/CanvasCreator.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o ColorManager/ColorManager.o ColorManager/ColorManager.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o ConsoleDisplay/ConsoleDisplay.o ConsoleDisplay/ConsoleDisplay.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Drawer/Drawer.o Drawer/Drawer.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o FontLoader/FontLoader.o FontLoader/FontLoader.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o GameLedTranslator/GameLedTranslator.o GameLedTranslator/GameLedTranslator.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o GameLeds/GameLeds.o GameLeds/GameLeds.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o GameModes/GameModes.o GameModes/GameModes.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o GameObject/GameObject.o GameObject/GameObject.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o GameState/GameState.o GameState/GameState.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o GameTimer/GameTimer.o GameTimer/GameTimer.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o History/History.o History/History.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o IInputWithTimer/IInputWithTimer.o IInputWithTimer/IInputWithTimer.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Inputs/Inputs.o Inputs/Inputs.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o KeyboardGameInput/KeyboardGameInput.o KeyboardGameInput/KeyboardGameInput.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o KeyboardInputWithTimer/KeyboardInputWithTimer.o KeyboardInputWithTimer/KeyboardInputWithTimer.cpp
KeyboardInputWithTimer/KeyboardInputWithTimer.cpp: In member function ‘virtual int KeyboardInputWithTimer::getInput()’:
KeyboardInputWithTimer/KeyboardInputWithTimer.cpp:63:33: warning: comparison of integer expressions of different signedness: ‘long int’ and ‘long unsigned int’ [-Wsign-compare]
   63 |             if ( _elapsedTimeMs >= _timeout_ms ) {
      |                  ~~~~~~~~~~~~~~~^~~~~~~~~~~~~~
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Logger/Logger.o Logger/Logger.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o LoggerFactory/LoggerFactory.o LoggerFactory/LoggerFactory.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o MatchWinSequence/MatchWinSequence.o MatchWinSequence/MatchWinSequence.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Mode1Functions/Mode1Functions.o Mode1Functions/Mode1Functions.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Mode1Score/Mode1Score.o Mode1Score/Mode1Score.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Mode2Functions/Mode2Functions.o Mode2Functions/Mode2Functions.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o MonitoredObject/MonitoredObject.o MonitoredObject/MonitoredObject.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o PairingBlinker/PairingBlinker.o PairingBlinker/PairingBlinker.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o PinInterface/PinInterface.o PinInterface/PinInterface.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o PinState/PinState.o PinState/PinState.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Player/Player.o Player/Player.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o PointLeds/PointLeds.o PointLeds/PointLeds.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o RemoteGameInput/RemoteGameInput.o RemoteGameInput/RemoteGameInput.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o RemoteInputWithTimer/RemoteInputWithTimer.o RemoteInputWithTimer/RemoteInputWithTimer.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o RemoteLocker/RemoteLocker.o RemoteLocker/RemoteLocker.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o RemotePairingScreen/RemotePairingScreen.o RemotePairingScreen/RemotePairingScreen.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Reset/Reset.o Reset/Reset.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o ScoreBoard/ScoreBoard.o ScoreBoard/ScoreBoard.cpp
ScoreBoard/ScoreBoard.cpp: In constructor ‘ScoreBoard::ScoreBoard(Player*, Player*, GameState*, IDisplay*, FontManager*, ColorManager*)’:
ScoreBoard/ScoreBoard.cpp:13:27: warning: variable ‘defaultColor’ set but not used [-Wunused-but-set-variable]
   13 |         rgb_matrix::Color defaultColor = _colorManager->getColor( "WHITE" );
      |                           ^~~~~~~~~~~~
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o ScoreboardBlinker/ScoreboardBlinker.o ScoreboardBlinker/ScoreboardBlinker.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o ServeLeds/ServeLeds.o ServeLeds/ServeLeds.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o SetDrawer/SetDrawer.o SetDrawer/SetDrawer.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o SetHistoryText/SetHistoryText.o SetHistoryText/SetHistoryText.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o SetLeds/SetLeds.o SetLeds/SetLeds.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o SetWin/SetWin.o SetWin/SetWin.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o SubjectManager/SubjectManager.o SubjectManager/SubjectManager.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o TieBreaker/TieBreaker.o TieBreaker/TieBreaker.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o TieLeds/TieLeds.o TieLeds/TieLeds.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o TranslateConstant/TranslateConstant.o TranslateConstant/TranslateConstant.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o Undo/Undo.o Undo/Undo.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o WatchTimer/WatchTimer.o WatchTimer/WatchTimer.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o WebLiquidCrystal/WebLiquidCrystal.o WebLiquidCrystal/WebLiquidCrystal.cpp
g++ -std=c++17 -Wall -O3 -g -I../include   -c -o WinSequences/WinSequences.o WinSequences/WinSequences.cpp
g++ run_remote_listener.o BatteryTest/BatteryTest.o BlankBlinker/BlankBlinker.o CanvasCreator/CanvasCreator.o ColorManager/ColorManager.o ConsoleDisplay/ConsoleDisplay.o Drawer/Drawer.o FontLoader/FontLoader.o GameLedTranslator/GameLedTranslator.o GameLeds/GameLeds.o GameModes/GameModes.o GameObject/GameObject.o GameState/GameState.o GameTimer/GameTimer.o History/History.o IInputWithTimer/IInputWithTimer.o Inputs/Inputs.o KeyboardGameInput/KeyboardGameInput.o KeyboardInputWithTimer/KeyboardInputWithTimer.o Logger/Logger.o LoggerFactory/LoggerFactory.o MatchWinSequence/MatchWinSequence.o Mode1Functions/Mode1Functions.o Mode1Score/Mode1Score.o Mode2Functions/Mode2Functions.o MonitoredObject/MonitoredObject.o PairingBlinker/PairingBlinker.o PinInterface/PinInterface.o PinState/PinState.o Player/Player.o PointLeds/PointLeds.o RemoteGameInput/RemoteGameInput.o RemoteInputWithTimer/RemoteInputWithTimer.o RemoteLocker/RemoteLocker.o RemotePairingScreen/RemotePairingScreen.o Reset/Reset.o ScoreBoard/ScoreBoard.o ScoreboardBlinker/ScoreboardBlinker.o ServeLeds/ServeLeds.o SetDrawer/SetDrawer.o SetHistoryText/SetHistoryText.o SetLeds/SetLeds.o SetWin/SetWin.o SubjectManager/SubjectManager.o TieBreaker/TieBreaker.o TieLeds/TieLeds.o TranslateConstant/TranslateConstant.o Undo/Undo.o WatchTimer/WatchTimer.o WebLiquidCrystal/WebLiquidCrystal.o WinSequences/WinSequences.o -L../lib -lrgbmatrix -lrt -lm -lpthread -o run_remote_listener
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::processQueryResult(IQueryResultProcessor*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:43: undefined reference to `Json::Value::Value(Json::ValueType)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:44: undefined reference to `Json::CharReaderBuilder::CharReaderBuilder()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:45: undefined reference to `Json::CharReaderBuilder::newCharReader() const'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:56: undefined reference to `Json::Value::toStyledString[abi:cxx11]() const'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:57: undefined reference to `Json::CharReaderBuilder::~CharReaderBuilder()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:57: undefined reference to `Json::Value::~Value()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::updateDataConfig()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:62: undefined reference to `Json::Value::Value(Json::ValueType)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:63: undefined reference to `Json::Value::Value(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:63: undefined reference to `Json::Value::operator[](char const*)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:63: undefined reference to `Json::Value::operator=(Json::Value&&)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:63: undefined reference to `Json::Value::~Value()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:65: undefined reference to `Json::StreamWriterBuilder::StreamWriterBuilder()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:66: undefined reference to `Json::writeString[abi:cxx11](Json::StreamWriter::Factory const&, Json::Value const&)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:67: undefined reference to `Json::StreamWriterBuilder::~StreamWriterBuilder()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:67: undefined reference to `Json::Value::~Value()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::initializeModel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:12: undefined reference to `SourceData::SourceData(ISourceDataConfig const&)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:13: undefined reference to `MonitorLed::MonitorLed()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::MonitoredObject(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:4: undefined reference to `LogObjectFactory::LogObjectFactory()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:4: undefined reference to `MonitorLed::MonitorLed()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::logUpdate(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:27: undefined reference to `MonitorLed::setFail()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:30: undefined reference to `MonitorLed::setPass()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:32: undefined reference to `MonitorLed::setLedText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:33: undefined reference to `LogObjectFactory::createLogObject(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:35: undefined reference to `Model::updateObject(ISourceQueryConfig const&, IQueryResultProcessor*)'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::~MonitoredObject()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.h:13: undefined reference to `LogObjectFactory::~LogObjectFactory()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::~MonitoredObject()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.h:13: undefined reference to `LogObjectFactory::~LogObjectFactory()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::processQueryResult(IQueryResultProcessor*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:57: undefined reference to `Json::CharReaderBuilder::~CharReaderBuilder()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:57: undefined reference to `Json::Value::~Value()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::updateDataConfig()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:63: undefined reference to `Json::Value::~Value()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:67: undefined reference to `Json::Value::~Value()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:67: undefined reference to `Json::StreamWriterBuilder::~StreamWriterBuilder()'
/usr/bin/ld: MonitoredObject/MonitoredObject.o: in function `MonitoredObject::MonitoredObject(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/MonitoredObject/MonitoredObject.cpp:6: undefined reference to `LogObjectFactory::~LogObjectFactory()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p2GameWinSequence()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:82: undefined reference to `GameWinSequence::GameWinSequence()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:84: undefined reference to `GameWinSequence::run(Player*, GameState*, GameLeds*, ScoreBoard*, int)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:105: undefined reference to `GameWinSequence::~GameWinSequence()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p1TBGameWinSequence()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:112: undefined reference to `GameWinSequence::GameWinSequence()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:113: undefined reference to `GameWinSequence::run(Player*, GameState*, GameLeds*, ScoreBoard*, int)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:114: undefined reference to `GameWinSequence::~GameWinSequence()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p2TBGameWinSequence()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:132: undefined reference to `GameWinSequence::GameWinSequence()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:133: undefined reference to `GameWinSequence::run(Player*, GameState*, GameLeds*, ScoreBoard*, int)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:134: undefined reference to `GameWinSequence::~GameWinSequence()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p1GameWinSequence()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:27: undefined reference to `GameWinSequence::GameWinSequence()'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:28: undefined reference to `GameWinSequence::run(Player*, GameState*, GameLeds*, ScoreBoard*, int)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:45: undefined reference to `GameWinSequence::~GameWinSequence()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p2GameWinSequence()':
/usr/include/c++/11/ext/new_allocator.h:89: undefined reference to `GameWinSequence::~GameWinSequence()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p1TBGameWinSequence()':
/usr/include/c++/11/ext/new_allocator.h:89: undefined reference to `GameWinSequence::~GameWinSequence()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p2TBGameWinSequence()':
/usr/include/c++/11/ext/new_allocator.h:89: undefined reference to `GameWinSequence::~GameWinSequence()'
/usr/bin/ld: WinSequences/WinSequences.o: in function `Mode1WinSequences::p1GameWinSequence()':
/home/adamsl/rpi-rgb-led-matrix/tennis-game/WinSequences/WinSequences.cpp:208: undefined reference to `GameWinSequence::~GameWinSequence()'
collect2: error: ld returned 1 exit status
make: *** [Makefile.remote_listener:68: run_remote_listener] Error 1
*** Make Error Count: 1 ***
adamsl@DESKTOP-SHDBATI:~/rpi-rgb-led-matrix/tennis-game$ 
```

Rewrite the Makefile to fix this problem.

# my link
https://chatgpt.com/c/6795a85b-199c-8006-ac6b-cc7dfc02d26a?model=gpt-4o


# shared link
https://chatgpt.com/share/6795af87-b468-8006-9db1-0da68ddc0228
