#include "WinSequences.h"

Mode1WinSequences::~Mode1WinSequences() {}
Mode1WinSequences::Mode1WinSequences( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState )
    : _player1( player1 ),
    _player2( player2 ),
    _pinInterface( pinInterface ),
    _gameState( gameState ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _reset( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {}

void Mode1WinSequences::setScoreBoard( ScoreBoard* scoreBoard ) {
    _pointLeds.setScoreBoard( scoreBoard );
    _gameLeds.setScoreBoard(  scoreBoard );
    _setLeds.setScoreBoard(   scoreBoard );}

void Mode1WinSequences::p1GameWinSequence() {
    std::cout << "p1GameWinSequence()" << std::endl;
    _undo.memory();
    int pulse_count = _gameState->getGameWinPulseCount();
    for ( int currentPulseCount = 0; currentPulseCount < pulse_count; currentPulseCount++ ) {
        _player1->setGames( 99 );
        _gameLeds.updateGames();
        GameTimer::gameDelay( _gameState->getGameFlashDelay());
        _player1->setGames( _gameState->getP1GamesMem());
        _gameLeds.updateGames();
        GameTimer::gameDelay( _gameState->getGameFlashDelay());}
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );}

void Mode1WinSequences::p1SetWinSequence() {
    std::cout << "p1SetWinSequence()" << std::endl;
    _undo.memory();  
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player1->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player1->setSets( _gameState, _gameState->getP1SetsMem());
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();  
    _setLeds.updateSets();}

void Mode1WinSequences::p1MatchWinSequence() {
    _undo.memory();
    _pointLeds.updateTBPoints();
    _player2->setGames( 99 );
    _gameLeds.updateGames();
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player1, _gameState, &_gameLeds, &_setLeds );
    _reset.resetScoreboard();}

void Mode1WinSequences::p2GameWinSequence() {
    std::cout << "p2GameWinSequence()" << std::endl;
    _undo.memory();  
    for ( int currentPulseCount = 0;
        currentPulseCount < _gameState->getGameWinPulseCount();
        currentPulseCount++ ) {
        _player2->setGames( 99 );
        _gameLeds.updateGames();  
        GameTimer::gameDelay( _gameState->getGameFlashDelay());
        _player2->setGames( _gameState->getP2GamesMem());
        _gameLeds.updateGames();                          
        GameTimer::gameDelay( _gameState->getGameFlashDelay());}
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );}

void Mode1WinSequences::p2SetWinSequence() {
    std::cout << "p2SetWinSequence()" << std::endl;
    _undo.memory();  
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player2->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player2->setSets( _gameState, _gameState->getP2SetsMem());
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay()); }
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();  
    _setLeds.updateSets();}

void Mode1WinSequences::p2MatchWinSequence() {
    _undo.memory();               
    _pointLeds.updateTBPoints();
    _player1->setGames( 99 );
    _gameLeds.updateGames();      
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player2, _gameState, &_gameLeds, &_setLeds );
    _reset.resetScoreboard();}

void Mode1WinSequences::p1TBGameWinSequence() {
    _undo.memory();  
    for ( int currentPulseCount = 0;
        currentPulseCount < _gameState->getGameWinPulseCount();
        currentPulseCount++ ) {
        _player1->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player1->setSets(    _gameState, _gameState->getP1SetsMem());
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}
    tieLEDsOff();
    _gameState->setTieBreak( 0 );
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();}

void Mode1WinSequences::p2TBGameWinSequence() {
    _undo.memory();  
    for ( int currentPulseCount = 0;
        currentPulseCount < _gameState->getGameWinPulseCount();
        currentPulseCount++ ) {
        _player2->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player2->setSets( _gameState, _gameState->getP2SetsMem());
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}
    tieLEDsOff();
    _gameState->setTieBreak( 0 );
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();}    

////////////////////////////////// SET WIN SEQUENCES //////////////////////////////////////////////
void Mode1WinSequences::p1TBSetWinSequence() {  // for entering set t/b
    _undo.memory();                               
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player1->setSets( _gameState, 0 );
        tieLEDsOff();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player1->setSets( _gameState, 1 );
        tieLEDsOn();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    tieLEDsOn();}

void Mode1WinSequences::p2TBSetWinSequence() {  // for entering set t/b
    _undo.memory();                               
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player2->setSets( _gameState, 0 );
        tieLEDsOff();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player2->setSets( _gameState, 1 );
        tieLEDsOn();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    tieLEDsOn();}
///////////////////////////// END OF SET WIN SEQUENCES ////////////////////////////////////////////


////////////////////////// SET TIE BREAKER WIN SEQUENCES //////////////////////////////////////////
void Mode1WinSequences::p1SetTBWinSequence() {  // for winning set t/b
    _undo.memory();                               

    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player1->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player1->setSets( _gameState, 3 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}}

void Mode1WinSequences::p2SetTBWinSequence() {  // for winning set t/b
    _undo.memory();                               
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player2->setSets( _gameState, 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player2->setSets( _gameState, 3 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());}}
//////////////////////// END OF SET TIE BREAKER WIN SEQUENCES /////////////////////////////////////

void Mode1WinSequences::p1TBMatchWinSequence() {
    _player2->setGames( 99 );
    _gameLeds.updateGames();  
    _undo.memory();           
    tieLEDsOff();
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player1, _gameState, &_gameLeds, &_setLeds );
    _reset.resetScoreboard();}

void Mode1WinSequences::p2TBMatchWinSequence() {
    _player1->setGames( 99 );
    _gameLeds.updateGames();  
    _undo.memory();           
    tieLEDsOff();
    MatchWinSequence matchWinSequence; matchWinSequence.run( _player2, _gameState, &_gameLeds, &_setLeds );
    _reset.resetScoreboard();}

void Mode1WinSequences::tieLEDsOn() {
    _gameState->setTieLEDsOn( 1 );  // tieLEDsOn = true;
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH );}

void Mode1WinSequences::tieLEDsOff() {
    _gameState->setTieLEDsOn( 0 );  // tieLEDsOn = false;
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW );}
