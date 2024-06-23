#include "Mode1Score.h"

Mode1Score::Mode1Score(
    Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState,
    History* history )
    : _player1( player1 ),
    _player2( player2 ),
    _gameState( gameState ),
    _history( history ),
    _tieBreaker( player1, player2, pinInterface, gameState, history ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _mode1WinSequences( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {
    try {
        try {
            try {
                try {
                    std::cout << "creating mode 1 score logger..." << std::endl;
                    _logger = new Logger( "Mode1Score" );
                    std::cout << "created new logger object with name [" << _logger->getName() << "]" << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error creating logger: " << e.what() << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error creating logger: " << e.what() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error creating logger: " << e.what() << std::endl;
        }
        std::cout << "Exiting playerGameWin for player: " << player->number() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating logger: " << e.what() << std::endl;
    }
    std::cout << "Exiting updateScore for player: " << currentPlayer->number() << std::endl;
}
Mode1Score::~Mode1Score() {}

TieBreaker* Mode1Score::getTieBreaker() { return &_tieBreaker; }

void Mode1Score::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard( scoreBoard );
    _gameLeds.setScoreBoard( scoreBoard );
    _mode1WinSequences.setScoreBoards( scoreBoard );
    _setLeds.setScoreBoard( scoreBoard );
    if (scoreBoard == nullptr) {
        std::cerr << "Error: scoreBoard is NULL in setScoreBoard" << std::endl;
    } else {
        std::cout << "ScoreBoard Address: " << scoreBoard << std::endl;
    }
    _tieBreaker.setScoreBoards(scoreBoard);   // all day debug. in the future.
    _undo.setScoreBoard( scoreBoard );
} // find a way to avoid this.

ScoreBoard* Mode1Score::getScoreBoard() { return _scoreBoard; }

void Mode1Score::_resetGame() {
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _gameState->setPlayer1Points( 0 );
    _gameState->setPlayer2Points( 0 );
    _gameState->setServeSwitch( 1 );
    // _gameState->setServe( 0 ); // set serve in game win only
    _pointLeds.updatePoints();
}

void Mode1Score::updateScore( Player* currentPlayer ) {
    _logger->setName( "updateScore" );
    std::cout << "done setting name to updateScore." << std::endl;
    std::cout << "Entering updateScore for player: " << currentPlayer->number() << std::endl;
    if ( _gameState->getTieBreak() == 1 ) {           // Tie Break
        std::cout << "Tie Break mode" << std::endl;
        _logger->logUpdate( "tie break run..." );
        std::cout << "Current Player Address: " << currentPlayer << std::endl;
        std::cout << "Opponent Address: " << currentPlayer->getOpponent() << std::endl;
        std::cout << "Current Player Points: " << currentPlayer->getPoints() << std::endl;
        std::cout << "Opponent Points: " << currentPlayer->getOpponent()->getPoints() << std::endl;
        std::cout << "TieBreaker iteration before setting serve: " << _tieBreaker.getIteration() << std::endl;
        std::cout << "Serve value from _getServe(): " << _gameState->getServe() << std::endl;
        _tieBreaker.run(currentPlayer);
        std::cout << "Tie Break run completed" << std::endl;
        std::cout << "TieBreaker iteration after setting serve: " << _tieBreaker.getIteration() << std::endl;
        std::cout << "Serve value after _tieBreaker.run(): " << _gameState->getServe() << std::endl;
    }
    else if ( _gameState->getSetTieBreak() == 1 ) { // Set Tie Break
        _logger->logUpdate( "set tie breaker..." );
        _tieBreaker.setTieBreaker();
        std::cout << "Set Tie Breaker completed" << std::endl;
    }
    else {                                          // Regular Game
        std::cout << "Regular Game mode" << std::endl;
        Player* otherPlayer = currentPlayer->getOpponent();
        std::cout << "Current Player Points: " << currentPlayer->getPoints() << std::endl;
        std::cout << "Other Player Points: " << otherPlayer->getPoints() << std::endl;
        int current_player_points = currentPlayer->getPoints();
        int other_player_points = otherPlayer->getPoints();
        if ( current_player_points >= 3 ) {
            _logger->logUpdate( "player " + std::to_string( currentPlayer->number() ) + " has 3 points or more." );
            if ( current_player_points == other_player_points ) {
                _logger->logUpdate( "player " + std::to_string( currentPlayer->number() ) + " has 3 points and tied with " + std::to_string( otherPlayer->number() ) + "." );
                std::cout << "Players are tied at 3 points" << std::endl;
                currentPlayer->setPoints( 3 );
                otherPlayer->setPoints( 3 );
            }
            else if ( current_player_points > 3
             && ( current_player_points - other_player_points ) > 1 ) {
                _logger->logUpdate( "player " + std::to_string( currentPlayer->number() ) + " has " + std::to_string( current_player_points ) + " points and won by " + std::to_string( current_player_points - other_player_points ) + "." );
                std::cout << "Player " << currentPlayer->number() << " won the game" << std::endl;
                currentPlayer->setGames( currentPlayer->getGames() + 1 );
                _undo.memory();
                currentPlayer->number() == 0 ? playerOneGameWin() : playerTwoGameWin();
            }

            if ( currentPlayer->getPoints() == 4 ) {
                _logger->logUpdate( "player " + std::to_string( currentPlayer->number() ) + " has 4 points." );
                _gameState->setPointFlash( 1 );       // "Ad" mode
                std::cout << "Player " << currentPlayer->number() << " has 4 points (Advantage)" << std::endl;
                _gameState->setPreviousTime( GameTimer::gameMillis() );
                _gameState->setToggle( 0 );
            }
        }
        _pointLeds.updatePoints();
    }
}

void Mode1Score::playerOneScore() { updateScore( _player1 ); }
void Mode1Score::playerTwoScore() { updateScore( _player2 ); }


//////////////////////////////// GAME WIN SCENARIOS ///////////////////////////
void Mode1Score::playerGameWin( Player* player ) {
    std::cout << "Entering playerGameWin for player: " << player->number() << std::endl;
    Player* opponent = player->getOpponent();
    std::cout << "Player " << player->number() << " games: " << player->getGames() << std::endl;
    std::cout << "player " << std::to_string( player->number() ) << " games: " << player->getGames() << std::endl;
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    if ( player->getGames() >= GAMES_TO_WIN_SET ) {
        std::cout << "Player " << player->number() << " has enough games to win the set" << std::endl;
        if ( player->getGames() == GAMES_TO_WIN_SET && opponent->getGames() == GAMES_TO_WIN_SET ) {
            _gameState->setTieBreak( 1 );
            _tieBreaker.initializeTieBreakMode();
        }
        if ( _gameState->getTieBreak() == 0 ) {  // if this is not a tie break game...
            std::cout << "This is not a tie break game" << std::endl;
            if ( ( player->getGames() - opponent->getGames() ) > 1 ) {  // player ahead by 2 games.
                player->setSets( _gameState, player->getSets() + 1 ); // Set win
                std::cout << "Player " << player->number() << " won the set" << std::endl;
                _setLeds.updateSets(); // sets the tiebreak, wins
                // the match, or just wins the set.
                if ( player->getSets() == opponent->getSets() ) {  // set tie break
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1TBSetWinSequence() : _mode1WinSequences.p2TBSetWinSequence();
                    std::cout << "Set tie break" << std::endl;
                    _gameState->setSetTieBreak( 1 );
                    _tieBreaker.setTieBreakEnable();
                }
                else if ( player->getSets() == SETS_TO_WIN_MATCH ) {  // match win, done playing
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.playerOneMatchWin() : _mode1WinSequences.playerTwoMatchWin();
                    std::cout << "Match win" << std::endl;
                    _gameState->stopGameRunning();
                }
                else {                                              // regular set win, then reset
                    _gameState->setPlayer1SetHistory( player->getSetHistory() );
                    _gameState->setPlayer2SetHistory( opponent->getSetHistory() );
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1SetWinSequence() : _mode1WinSequences.p2SetWinSequence();
                    std::cout << "Regular set win" << std::endl;
                    _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
                    _setLeds.updateSets();
                    GameTimer::gameDelay( _gameState->getWinDelay() );
                    _resetGame();
                }
                player->setGames( 0 );
                opponent->setGames( 0 );
            }
            else {                        // player is ahead by 1 game, but not enough to win the set.
                player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1GameWinSequence() : _mode1WinSequences.p2GameWinSequence();
                std::cout << "Player " << player->number() << " is ahead by 1 game" << std::endl;
                _gameLeds.updateGames();
                _gameState->setPlayer1SetHistory( player->getSetHistory() );
                _gameState->setPlayer2SetHistory( opponent->getSetHistory() );
                _resetGame();
            }
        }
    }
    else { // this is a regualar game win...
        if ( player->number() == PLAYER_1_INITIALIZED ) {
            std::cout << "Regular game win for Player 1" << std::endl;
            _mode1WinSequences.p1GameWinSequence();
            _gameState->setPlayer1SetHistory( player->getSetHistory() );
            _gameState->setPlayer2SetHistory( opponent->getSetHistory() );
        }
        else {
            _mode1WinSequences.p2GameWinSequence();
            _gameState->setPlayer2SetHistory( player->getSetHistory() );
            _gameState->setPlayer1SetHistory( opponent->getSetHistory() );
        }
        _gameLeds.updateGames();
        _resetGame();
    }
}
void Mode1Score::playerOneGameWin() { playerGameWin( _player1 ); }
void Mode1Score::playerTwoGameWin() { playerGameWin( _player2 ); }
//////////////////// END GAME WIN SCENARIOS ///////////////////////////////////

void Mode1Score::mode1TBP1Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 15 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );

        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p1TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
    if ( _player1->getGames() >= 10 &&
        ( _player1->getGames() - _player2->getGames() ) > 1 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p1TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
}

void Mode1Score::mode1SetTBP1Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 7 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        _setLeds.updateSets();
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p1SetTBWinSequence();
        _tieBreaker.tieLEDsOff();
        _mode1WinSequences.playerOneMatchWin();
        _gameState->stopGameRunning();
    }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}

void Mode1Score::mode1TBP2Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 15 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
    if ( _player2->getGames() >= 10 &&
        ( _player2->getGames() - _player1->getGames() ) > 1 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );  // set set history, set++
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
}

void Mode1Score::mode1SetTBP2Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 7 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        _setLeds.updateSets();
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p2SetTBWinSequence();
        _tieBreaker.tieLEDsOff();
        _mode1WinSequences.playerTwoMatchWin();
        _gameState->stopGameRunning();
    }
    std::cout << "setting serve switch to: " << _gameState->getServeSwitch() + 1 << std::endl;
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}
