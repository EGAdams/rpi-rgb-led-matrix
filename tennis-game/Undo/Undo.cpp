#include "Undo.h"

Undo::Undo( Player* player1, Player* player2, PinInterface* pinInterface, GameState* gameState ):
    _player1( player1 ), _player2( player2 ),
    _gameState( gameState ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _serveLeds( pinInterface, gameState ),
    _tieLeds( pinInterface ) {
        _logger = new Logger( "Undo" );
        _scoreBoardSet = false; };
Undo::~Undo(){
    // std::cout << "*** Undo destructor called. ***" << std::endl;
    delete _logger; };

void Undo::setScoreBoard( ScoreBoard* scoreBoard ) {
    // _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard(          scoreBoard );
    _gameLeds.setScoreBoard(           scoreBoard );
    // _mode1WinSequences.setScoreBoards( scoreBoard );
    _setLeds.setScoreBoard(            scoreBoard );
    _scoreBoardSet = true;
}

void Undo::memory() {
    _gameState->setP1PointsMem(       _player1->getPoints()); _gameState->setP2PointsMem( _player2->getPoints());
    _gameState->setP1GamesMem(        _player1->getGames());  _gameState->setP2GamesMem(  _player2->getGames());
    _gameState->setP1SetsMem(         _player1->getSets());   _gameState->setP2SetsMem(   _player2->getSets());
    _gameState->setTieBreakMem(       _gameState->getTieBreak());
    _gameState->setMatchTieBreakMem(  _gameState->getMatchTieBreak());
    _gameState->setPlayer1SetHistory( _player1->getSetHistory());
    _gameState->setPlayer2SetHistory( _player2->getSetHistory()); }

void Undo::snapshot( History* history ) {
    if ( _gameState->getCurrentAction() == RUNNING_MATCH_TIE_BREAK && _player2->getSetHistory()[ 2 ] == 0 ) { 
        std::cout << "*** ERROR: player two set history is blank during match win tie break! ***" << std::endl;
        exit( 0 );
    }
    GameState gameState;
    gameState.setPlayer1Points( _player1->getPoints()); gameState.setP1PointsMem( _gameState->getP1PointsMem());
    gameState.setPlayer2Points( _player2->getPoints()); gameState.setP2PointsMem( _gameState->getP2PointsMem());
    gameState.setPlayer1Games( _player1->getGames());  gameState.setP1GamesMem( _gameState->getP1GamesMem());
    gameState.setPlayer2Games( _player2->getGames()); gameState.setP2GamesMem( _gameState->getP2GamesMem());
    gameState.setPlayer1Sets( _player1->getSets()); gameState.setP1SetsMem( _gameState->getP1SetsMem());
    gameState.setPlayer2Sets( _player2->getSets()); gameState.setP2SetsMem( _gameState->getP2SetsMem());
    gameState.setPlayer1Matches( _player1->getMatches()); gameState.setPlayer2Matches( _player2->getMatches());
    gameState.setServe( _gameState->getServe());
    gameState.setServeSwitch( _gameState->getServeSwitch());
    gameState.setUndo( _gameState->getUndo());
    gameState.setStarted( _gameState->getStarted());
    gameState.setPointFlash( _gameState->getPointFlash());
    gameState.setRotaryPosition( _gameState->getRotaryPosition());
    gameState.setPrevRotaryPosition( _gameState->getPrevRotaryPosition());
    gameState.setRotaryChange( _gameState->getRotaryChange());
    gameState.setToggle( _gameState->getToggle());
    gameState.setTieLEDsOn( _gameState->getTieLEDsOn());
    gameState.setTieBreak( _gameState->getTieBreak());
    gameState.setMatchTieBreak( _gameState->getMatchTieBreak());
    gameState.setMatchTieBreakMem( _gameState->getMatchTieBreakMem());
    gameState.setTieBreakOnly( _gameState->getTieBreakOnly());
    gameState.setTieBreakMem( _gameState->getTieBreakMem());
    gameState.setPlayer1SetHistory( _player1->getSetHistory());
    gameState.setPlayer2SetHistory( _player2->getSetHistory());
    _gameState->setPlayer1SetHistory( _player1->getSetHistory());
    _gameState->setPlayer2SetHistory( _player2->getSetHistory());
    gameState.setCurrentSet( _gameState->getCurrentSet());
    history->push( gameState );
}

void Undo::mode1Undo( History* history ) {
    _gameState->setState( UNDO_STATE );
    GameTimer::gameDelay( 100 );
    print( "history size: " << history->size() );
    if ( history->size() == 0 ) { return; }
    if ( _scoreBoardSet == false ) {
        std::cout << "*** ERROR: trying to call undo when _scoreBoardSet == false exiting process... *** \nMake sure to call undo->setScoreBoard before trying to use the Undo object." << std::endl;
        exit( 1 );
    } // TODO: Update Current Set 1st!
    print( "popping history..." );
    GameState gameState = ( history->pop());
    print( "done popping history." );
    _gameState->setPlayer1SetHistory( gameState.getPlayer1SetHistory());
    print( "done setting player 1 histories" );
    _gameState->setPlayer2SetHistory( gameState.getPlayer2SetHistory());
    print( "done setting player 2 histories" );
    _gameState->setCurrentSet( gameState.getCurrentSet());
    _player1->setPoints( gameState.getPlayer1Points());
    _gameState->setP1PointsMem( gameState.getP1PointsMem());
    _player2->setPoints( gameState.getPlayer2Points());
    _gameState->setP2PointsMem( gameState.getP2PointsMem());
    _player1->setGames( gameState.getPlayer1Games());
    _gameState->setP1GamesMem( gameState.getP1GamesMem());
    _player2->setGames( gameState.getPlayer2Games());
    _gameState->setP2GamesMem( gameState.getP2GamesMem());
    _player1->setSets( gameState.getPlayer1Sets());
    print( "setting set histories... " );
    _player1->setSetHistory( gameState.getPlayer1SetHistory());
    _gameState->setP1SetsMem( gameState.getP1SetsMem());
    _player2->setSets( gameState.getPlayer2Sets());
    _player2->setSetHistory( gameState.getPlayer2SetHistory());
    print( "done setting set histories" );
    _gameState->setP2SetsMem( gameState.getP2SetsMem());
    _player1->setMatches( gameState.getPlayer1Matches());
    _player2->setMatches( gameState.getPlayer2Matches());
    _gameState->setServe( gameState.getServe());
    _gameState->setServeSwitch( gameState.getServeSwitch());
    _gameState->setUndo( gameState.getUndo());
    _gameState->setStarted( gameState.getStarted());
    _gameState->setPointFlash( gameState.getPointFlash());
    _gameState->setRotaryPosition( gameState.getRotaryPosition());
    _gameState->setPrevRotaryPosition( gameState.getPrevRotaryPosition());
    _gameState->setRotaryChange( gameState.getRotaryChange());
    _gameState->setToggle( gameState.getToggle());
    _gameState->setTieLEDsOn( gameState.getTieLEDsOn());
    // _gameState->setPlayerButton( gameState.getPlayerButton()); // this is breaking the undo
    print ( "setting tie break flags..." );
    _gameState->setTieBreak( gameState.getTieBreak());
    _gameState->setMatchTieBreak( gameState.getMatchTieBreak());
    _gameState->setMatchTieBreakMem( gameState.getMatchTieBreakMem());
    _gameState->setTieBreakOnly( gameState.getTieBreakOnly());
    _gameState->setTieBreakMem( gameState.getTieBreakMem());
    print( "done setting tie break flags" );
    if ( _gameState->getTieLEDsOn() == 1 ) { _gameState->setTieLEDsOn( 1 );  _tieLeds.turnOn(); }
    if ( _gameState->getTieLEDsOn() == 0 ) {  _tieLeds.turnOff(); _gameState->setTieLEDsOn( 0 );  }
    _logger->logUpdate( "updating leds..." );
    _pointLeds.updatePoints();
    _gameLeds.updateGames();
    _setLeds.updateSets();
    _serveLeds.updateServeLED();
}
