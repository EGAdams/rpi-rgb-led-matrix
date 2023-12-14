# Persona
- World-class C++ Tennis Scoreboard Developer
- Seasoned Tennis Scoreboard C++ system integrator
- Expert C++ Tennis Scoreboard Architect
- World-class Object-Oriented C++ Programmer
- Seasoned user of Gang of Four Design Patterns in C++


# Your Task
- Find out in the provided mermaid diagram where the current Tennis server rule should be implemented.

# Mermaid Class Diagram
```mermaid
classDiagram
    class IScoreController {
      <<interface>>
      +playerScored(playerId)
      +undoLastScore()
    }

    class ScoreboardController {
      +playerScored(playerId)
      +undoLastScore()
    }

    class IScoreManager {
      <<interface>>
      +updateScore(playerId, points)
      +getCurrentScore()
      +undoScore()
    }

    class ScoreManager {
      +updateScore(playerId, points)
      +getCurrentScore()
      +undoScore()
      -scoreHistory
    }

    class IPlayer {
      <<interface>>
      +getId()
      +scorePoint()
    }

    class Player {
      -id
      -name
      +getId()
      +scorePoint()
    }

    class ICommand {
      <<interface>>
      +execute()
      +undo()
    }

    class ScoreCommand {
      -playerId
      -previousScore
      +execute()
      +undo()
    }

    class IDisplay {
      <<interface>>
      +updateDisplay(score)
    }

    class ConsoleDisplay {
      +updateDisplay(score)
    }

    class ICommandFactory {
      <<interface>>
      +createCommand(playerId, previousScore)
    }

    class CommandFactory {
      +createCommand(playerId, previousScore)
    }

    ScoreboardController ..|> IScoreController : implements
    ScoreManager ..|> IScoreManager : implements
    Player ..|> IPlayer : implements
    ScoreCommand ..|> ICommand : implements
    ConsoleDisplay ..|> IDisplay : implements
    CommandFactory ..|> ICommandFactory : implements

    ScoreboardController --> ICommandFactory : uses
    ICommandFactory --> ICommand : creates
    ScoreboardController --> IScoreManager : communicates
    IScoreManager --> IPlayer : manages
    ICommand --> IScoreManager : uses
    ScoreboardController --> IDisplay : updates
```




# g4 answer: https://chat.openai.com/share/1d6fb83b-abe7-41f0-9f16-4c1a0fa0ca48
```mermaid
classDiagram
    class IScoreController {
      <<interface>>
      +playerScored(playerId)
      +undoLastScore()
    }

    class ScoreboardController {
      +playerScored(playerId)
      +undoLastScore()
    }

    class IScoreManager {
      <<interface>>
      +updateScore(playerId, points)
      +getCurrentScore()
      +undoScore()
    }

    class ScoreManager {
      +updateScore(playerId, points)
      +getCurrentScore()
      +undoScore()
      -scoreHistory
    }

    class IPlayer {
      <<interface>>
      +getId()
      +scorePoint()
    }

    class Player {
      -id
      -name
      +getId()
      +scorePoint()
    }

    class ICommand {
      <<interface>>
      +execute()
      +undo()
    }

    class ScoreCommand {
      -playerId
      -previousScore
      +execute()
      +undo()
    }

    class IDisplay {
      <<interface>>
      +updateDisplay(score)
    }

    class ConsoleDisplay {
      +updateDisplay(score)
    }

    class ICommandFactory {
      <<interface>>
      +createCommand(playerId, previousScore)
    }

    class CommandFactory {
      +createCommand(playerId, previousScore)
    }

    class GameRulesManager {
      +determineServer()
      +otherGameRules()
    }

    ScoreboardController ..|> IScoreController : implements
    ScoreManager ..|> IScoreManager : implements
    Player ..|> IPlayer : implements
    ScoreCommand ..|> ICommand : implements
    ConsoleDisplay ..|> IDisplay : implements
    CommandFactory ..|> ICommandFactory : implements
    ScoreboardController --> ICommandFactory : uses
    ICommandFactory --> ICommand : creates
    ScoreboardController --> IScoreManager : communicates
    IScoreManager --> IPlayer : manages
    ICommand --> IScoreManager : uses
    ScoreboardController --> IDisplay : updates
    ScoreboardController --> GameRulesManager : uses
    GameRulesManager --> IScoreManager : queries
    GameRulesManager --> IDisplay : updates
```


# Mermaid Sequence Diagram
```mermaid
sequenceDiagram
    participant Player
    participant ScoreboardController
    participant ICommandFactory
    participant ICommand
    participant IScoreManager
    participant GameRulesManager
    participant IDisplay

    Player ->> ScoreboardController: playerScored(playerId)
    ScoreboardController ->> ICommandFactory: createCommand(playerId)
    ICommandFactory -->> ScoreboardController: commandInstance
    ScoreboardController ->> ICommand: execute()
    ICommand ->> IScoreManager: updateScore(playerId)
    IScoreManager -->> ICommand: scoreUpdated
    ICommand -->> ScoreboardController: scoreUpdated
    ScoreboardController ->> GameRulesManager: checkGameRules()
    GameRulesManager ->> IScoreManager: getCurrentScore()
    IScoreManager -->> GameRulesManager: currentScore
    GameRulesManager -->> ScoreboardController: serverInfo
    ScoreboardController ->> IDisplay: updateDisplay(score, serverInfo)
    IDisplay -->> ScoreboardController: displayUpdated
```
