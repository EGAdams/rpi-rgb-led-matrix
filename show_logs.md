# Who you are
You are an expert Python programmer and seasoned user of the GoF Design pattern book.

# Example Raw Log File
Here is an example of what a log file looks like:
```json
{"timestamp":1694621373000,"id":"Undo_730351728_1694621373000","message":"updating leds...","method":"mode1Undo"}
{"timestamp":1694621438000,"id":"Undo_1660302372_1694621438000","message":"updating leds...","method":"mode1Undo"}
{"timestamp":1694621439000,"id":"Undo_1058280846_1694621439000","message":"updating leds...","method":"mode1Undo"}
```

# Tree command example output
```bash
adamsl@penguin:~/rpi-rgb-led-matrix/tennis-game$ tree -P 'log.txt' -L 2
.
├── agency-swarm
├── Arduino
├── BatteryTest
├── CanvasCreator
├── documentation
├── Drawer
│   └── log.txt
├── FetchRunner
├── FileReader
├── FontLoader
│   └── log.txt
├── fonts
├── GameLeds
├── GameLedTranslator
├── GameModes
│   └── log.txt
├── GameObject
│   └── log.txt
├── GameState
├── GameStateSerializer
├── GameTimer
├── GameWinSequence
│   └── log.txt
├── googletest
│   ├── build
│   ├── ci
│   ├── docs
│   ├── googlemock
│   └── googletest
├── History
├── Inputs
├── JsonParser
├── Logger
├── LoggerFactory
├── LogObject
├── LogObjectContainer
├── LogObjectFactory
├── log.txt
├── MatchWinSequence
│   └── log.txt
├── Mode1Functions
│   └── log.txt
├── Mode1Score
│   └── log.txt
├── Mode2Functions
│   └── log.txt
├── Model
├── MonitoredObject
├── MonitorLed
├── MonitorLedClassObject
├── nlohmann
├── PinInterface
├── PinState
├── Player
├── PointLeds
├── Reset
│   └── log.txt
├── ScoreBoard
│   └── log.txt
├── ServeLeds
├── SetDrawer
│   └── log.txt
├── SetHistoryText
│   └── log.txt
├── SetLeds
│   └── log.txt
├── SetWin
├── SourceData
├── SubjectManager
├── TennisConstants
│   └── log.txt
├── TextDrawer
├── TieBreaker
│   └── log.txt
├── TieLeds
├── TranslateConstant
├── Undo
│   └── log.txt
├── WatchTimer
├── WebLiquidCrystal
├── WinSequence
├── WinSequences
│   └── log.txt
└── z_end_list_instruct

65 directories, 19 files
adamsl@penguin:~/rpi-rgb-led-matrix/tennis-game$
```

# Your Task
Make a pexpect script to run the following command:
```bash
tree -P 'log.txt' -L 2
```
Use the `Tree command example output` from above to come up with this menu:
```bash
Show Logs for Object:
01. Drawer
02. FontLoader
03. GameModes
04. GameObject
05. GameWinSequence
06. LogObjectFactory
07. MatchWinSequence
08. Mode1Functions
09. Mode1Score
10. Mode2Functions
11. Reset
12. ScoreBoard
13. SetDrawer
14. SetHistoryText
15. SetLeds
16. TennisConstants
17. TieBreaker
18. Undo
19. WinSequences
20. Delete
21. Delete All
```

As you can see, I have selected all of the Objects with
log.txt in their directory.

So when I click "01" for example, I want all of the logs for
the Drawer to be printed.

So when "01" is clicked, we need to read the Drawer/log.txt file and print the output like this:
```bash
Date   Time       Method       Message
May 26 06:42 PM   mode1Undo()  updating leds...
May 26 06:48 PM   mode1Undo()  updating timer...
May 26 06:50 PM   show()       showing score.

Show Logs for Object:
01. Drawer
02. FontLoader
03. GameModes
04. GameObject
05. GameWinSequence
06. LogObjectFactory
07. MatchWinSequence
08. Mode1Functions
09. Mode1Score
10. Mode2Functions
11. Reset
12. ScoreBoard
13. SetDrawer
14. SetHistoryText
15. SetLeds
16. TennisConstants
17. TieBreaker
18. Undo
19. WinSequences
20. Delete
21. Delete All
```

And when I click "20." for Delete, you will ask me which object to delete and I will say for example "18." for Undo.

Then delete the log.txt file for undo:
```bash
rm Undo/log.txt
```

The new menu will now look like this:
```bash
Show Logs for Object:
01. Drawer
02. FontLoader
03. GameModes
04. GameObject
05. GameWinSequence
06. LogObjectFactory
07. MatchWinSequence
08. Mode1Functions
09. Mode1Score
10. Mode2Functions
11. Reset
12. ScoreBoard
13. SetDrawer
14. SetHistoryText
15. SetLeds
16. TennisConstants
17. TieBreaker
18. WinSequences
19. Delete
20. Delete All
```

If I click "20." to Delete All, you run the command:
```
rm ...*/log.txt
```
or whatever command that we need to remove all of the log.txt files.

After all of the logs are deleted, the output looks like this:
```bash
Show Logs for Object:
No Logs
```

Please write the Python pexpect script that will accomplish this.






