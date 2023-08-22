YOUR ROLE:
Act as an expert C++ Developer.

I am making a command line tennis game in C++.  When I start the game, I want certain parts of the tennis game to be initialized on the command line for testing.
For player score, I want to use the argument "--player1" or "--player2" arguments to initialize the player points.  For example, to start the game where both players have 3 points each, the command would be:
``` command
./tennis-game --player1 3 --player2 3
```

To start the game with player1 having 2 games, 2 sets and 3 points and player2 having 3 games 3 sets and 2 points, the command would be:
``` command
./tennis-game --player_points 3 --player1_sets 2 --player1_games 2 --player2_points 2 --player2_sets 3 --player2_games 3
```

YOUR TASK:
Please rewrite the main() method below to accept the arguments and set the appropriate variables.

for example, for each argument, you would do something like this setting the default to zero:
``` cpp
int player1_games = args[ --player1_games ]  // this is pseudo code, i don't know how to do this
```

Please do this for all arguments and default them all to zero.
``` arguments
--player1_score
--player2_score
--player1_sets
--player2_sets
--player1_games
--player2_games
```

Here is the code that needs to be modified:
``` cpp
int main( int argc, char *argv[]) {
    int loop_count = 0;
    #define MAX_LOOP_COUNT 350
    #define A_SPACE        13
    #define FOUR_SPACE     14
    #define THREE_SPACE    15
    #define SCORE_DELAY    .15
    
    // set games to --games argument
    // set sets to --sets argument
    // set player1 score to --player1 argument
    // set player2 score to --player2 argument

    std::cout << "creating GameObject..." << std::endl;
    GameState*  gameState  = new GameState();  // make this 1st!!! cost me 3 days of debugging
    GameObject* gameObject = new GameObject( gameState );
    std::cout << "done creating game object.  sleeping...\n\n\n\n\n" << std::endl;
    sleep( 1 );
    std::cout << "done sleeping.  calling gameObject->loopGame()..." << std::endl;
    gameObject->loopGame();

    std::cout << "done calling loopGame().  sleeping...\n\n\n\n\n" << std::endl;
    sleep( 1 );

    int player = 1;
    std::signal( SIGINT, GameObject::_signalHandler );
    /*/// Begin Game Loop ///*/ while ( gameState->gameRunning() && GameObject::gSignalStatus != SIGINT ) { 
        if ( loop_count >  MAX_LOOP_COUNT ) { gameState->stopGameRunning(); }
        sleep( SCORE_DELAY );
        std::cout << "\n\nenter 1 or 2 to score for player 1 or 2: ";
        std::cin >> player;
        std::cout << "\n\n\n\n\n\n\n*** Player " << player << " scored ***\n" << std::endl;
        gameObject->playerScore( player );  // flip the player score flag
        sleep( SCORE_DELAY );
        gameObject->loopGame();  // handle the player score flag
        loop_count++;
        std::cout << "player 1 points: " << gameState->getPlayer1Points();
        std::cout << "  player 2 points: " << gameState->getPlayer2Points() << std::endl;
        std::cout << "player 1 games:  "  << gameState->getPlayer1Games();
        std::cout << "  player 2 games:  "  << gameState->getPlayer2Games()  << std::endl;
        std::cout << "player 1 sets:   "   << gameState->getPlayer1Sets();
        std::cout << "  player 2 sets:   "   << gameState->getPlayer2Sets();
        std::cout << "     current set: "     << gameState->getCurrentSet()      << std::endl;
        std::map<int, int> _player1_set_history = gameState->getPlayer1SetHistory();
        std::map<int, int> _player2_set_history = gameState->getPlayer2SetHistory();
        std::cout << "end of game loop.  loop_count: " << loop_count << std::endl;
    } ///////// End Game Loop /////////
    std::cout << "game loop exited.  loop_count: " << loop_count << std::endl;
    if ( loop_count > MAX_LOOP_COUNT ) {
        // sleep for 5 seconds
        std::cout << "sleeping for 5 seconds..." << std::endl;
        sleep( 120 );
        std::cout << "MAX_LOOP_COUNT reached.  Exiting...\n\n\n\n\n" << std::endl; }
    return 0; }
```
 