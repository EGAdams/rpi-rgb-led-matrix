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
