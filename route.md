# Compile the score test

## Unit test wont compile

### Debug error message
```bash
adamsl@DESKTOP-76N0UOT:~/linuxBash/SMOL_AI/tennis_unit_tests$ make
g++ GameWinSequence.o SetWin.o SetHistoryText.o SetDrawer.o GameLedTranslator.o SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o TieBreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o NumberDrawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o -o tennis-game -L../lib -lrgbmatrix -lrt -lm -lpthread -L/home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/build/lib -lgtest_main -lgtest -lpthread
/usr/bin/ld: cannot find -lrgbmatrix
collect2: error: ld returned 1 exit status
Makefile:42: recipe for target 'tennis-game' failed
make: *** [tennis-game] Error 1
adamsl@DESKTOP-76N0UOT:~/linuxBash/SMOL_AI/tennis_unit_tests$
```
#### g4 suggestions
https://chat.openai.com/share/8d020a51-9423-4c46-beb4-918e785940b8

- Code to check for library installation
```python
import os
import subprocess

def check_library_installation(library_name):
    result = subprocess.run(['ldconfig', '-p'], capture_output=True, text=True)
    return library_name in result.stdout

# Usage:
print(check_library_installation('rgbmatrix'))
```

# Create the match win unit test
- Find the g4 instructions for this about the expert who explains the whole elaborate scoring system.




@golden_system_that_needs_to_be_followed.md This file is a guide to what we need to implement in the future. The objects in this diagram have not been built yet. The ultimate goal will be to refactor the system that is in this codebase right now to conform to the design in this mermaid sequence diagram. Let's do this piece by piece and start with where the player serve status logic is.

A place in the code changes the server bar status after every game. There is also a place in the code that changes the server bar status after every Set. The logic needs to be fixed here because the serve bar works up to the point where a player wins a Set.

Please use your vast knowledge of C++ Scoreboard Systems to find this flawed logic.  Think about how we could fix the flawed logic, keeping the system depicted in the mermaid diagram in mind. Take as many shortcuts as you have to because the code is already in a state of disarray.

The serve should alternate between players after each game, but when a new set begins, the player who did not serve first in the previous set should serve first in the new set.

