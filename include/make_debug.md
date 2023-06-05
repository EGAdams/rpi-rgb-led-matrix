You are a world-class C++ debugger.

Your task will be to debug a C++ project make output.

The links to the relevant files are:
- https://github.com/EGAdams/rpi-rgb-led-matrix/blob/master/tennis-game/tennis-game.cpp
- https://github.com/EGAdams/rpi-rgb-led-matrix/blob/master/tennis-game/Makefile

Here is the output of the make command:
```
g++ -I../include -Wall -O3 -g -Wextra -Wno-unused-parameter -c -o ScoreBoard.o ScoreBoard/ScoreBoard.cpp
g++ SubjectManager.o WebLiquidCrystal.o WatchTimer.o Inputs.o Mode1Tiebreaker.o Mode1Score.o Mode1Functions.o ServeLeds.o Undo.o BatteryTest.o Reset.o SetLeds.o TieLeds.o Mode1WinSequences.o Mode2Functions.o MatchWinSequence.o TennisConstants.o GameLeds.o GameModes.o GameObject.o PinState.o PinInterface.o TranslateConstant.o PointLeds.o Arduino.o CanvasCreator.o FontLoader.o NumberDrawer.o TextDrawer.o GameTimer.o Logger.o History.o GameState.o ScoreBoard.o Player.o tennis-game.o -o tennis-game -L../lib -lrgbmatrix -lrt -lm -lpthread
/usr/bin/ld: cannot find -lrgbmatrix
collect2: error: ld returned 1 exit status
make: *** [Makefile:25: tennis-game] Error 1
```

Please begin your debugging session.