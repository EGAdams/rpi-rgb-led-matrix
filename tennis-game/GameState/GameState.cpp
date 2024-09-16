#include "GameState.h"

GameState::~GameState() {}
GameState::GameState() {
    _match_blink_on = 1;
    _gameRunning = 1;
    _current_set = 1;
    _pointFlash = 0;
    _serve = 0;
    _tieBreak = 0;
    _matchTieBreak = 0;
    _tieLEDsOn = 0;
    _started = 0;
    _serveSwitch = 1;
    _playerButton = 0;
    _undo = 0;
    _freezePlayerButton = 0;
    _winDelay           = 500;
    _buttonDelay        = 150;
    _flashDelay         = 250;
    _gameWinPulseCount = 4;
    _tieBreakMem     = 0;
    _matchTieBreakMem  = 0;
    _p1PointsMem     = 0; _p2PointsMem     = 0;
    _p1GamesMem      = 0; _p2GamesMem      = 0;
    _p1SetsMem       = 0; _p2SetsMem       = 0;
    _player1_points  = 0; _player2_points  = 0;
    _player1_games   = 0; _player2_games   = 0;
    _player1_sets    = 0; _player2_sets    = 0;
    _player1_matches = 0; _player2_matches = 0; }

int GameState::gameRunning() { return _gameRunning; }
void GameState::stopGameRunning() { _gameRunning = 0; }
void GameState::setCurrentSet( int current_set ) { _current_set = current_set; }
void GameState::setMatchBlinkOn() {  _match_blink_on = 1;    }
void GameState::setMatchBlinkOff() { _match_blink_on = 0;    }
int  GameState::getMatchBlink() {    return _match_blink_on; }
int GameState::getCurrentSet() const { return _current_set; }
void GameState::setCurrentAction( std::string currentAction ) { _current_action = currentAction; }
std::string GameState::getCurrentAction() const { return _current_action; }
void GameState::setCurrentState( std::string current_state_arg ) { _current_state = current_state_arg; }
std::string GameState::getCurrentState() const { return _current_state; }
void GameState::setGameWinPulseCount( int gameWinPulseCount ) { _gameWinPulseCount = gameWinPulseCount; }
int GameState::getGameWinPulseCount() { return _gameWinPulseCount; }
void GameState::setButtonDelay( int buttonDelay ) { _buttonDelay = buttonDelay; }
int GameState::getButtonDelay() { return _buttonDelay; }
void GameState::setFlashDelay( unsigned long flashDelay ) { _flashDelay = flashDelay; }
unsigned long GameState::getFlashDelay() { return _flashDelay; }
void GameState::setWinDelay( int winDelay ) { _winDelay = winDelay; }
int GameState::getWinDelay() { return _winDelay; }
void GameState::setPointFlash( int pointFlash ) { _pointFlash = pointFlash; }
int GameState::getPointFlash() { return _pointFlash; }
void GameState::setServe( int serve ) { /* std::cout << "setting serve in GameState: " << serve << std::endl; */ _serve = serve; }
int GameState::getServe() { /*std::cout << "getting serve in GameState: " << _serve << std::endl;*/ return _serve; }
void GameState::setTieBreak( int tieBreak ) { _tieBreak = tieBreak; }
int GameState::getTieBreak() { return _tieBreak; }
void GameState::setMatchTieBreak( int matchTieBreak ) { _matchTieBreak = matchTieBreak; }
int GameState::getMatchTieBreak() { return _matchTieBreak; }
void GameState::setTieLEDsOn( int tieLEDsOn ) { _tieLEDsOn = tieLEDsOn; }
int GameState::getTieLEDsOn() { return _tieLEDsOn; }
void GameState::setServeSwitch( int serveSwitch ) { _serveSwitch = serveSwitch; }
int GameState::getServeSwitch() { return _serveSwitch; }
void GameState::setPlayerButton( int playerButton ) { _playerButton = playerButton; }
int GameState::getPlayerButton() { return _playerButton; }
void GameState::setStarted( int started ) { _started = started; }
int GameState::getStarted() { return _started; }
void GameState::setUndo( int undo ) { _undo = undo; }
int GameState::getUndo() { return _undo; }
void GameState::setRotaryChange( int rotaryChange ) { _rotaryChange = rotaryChange; }
int GameState::getRotaryChange() { return _rotaryChange; }
int GameState::getRotaryPosition() { return _rotaryPosition; }
void GameState::setRotaryPosition( int rotaryPosition ) { _rotaryPosition = rotaryPosition; }
void GameState::setPreviousTime( unsigned long previousTime ) { _previousTime = previousTime; }
void GameState::setPrevRotaryPosition( int prevRotaryPosition ) { _prevRotaryPosition = prevRotaryPosition; }
int GameState::getPrevRotaryPosition() { return _prevRotaryPosition; }
unsigned long GameState::getPreviousTime() { return _previousTime; }
void GameState::setFreezePlayerButton( int freezePlayerButton ) { _freezePlayerButton = freezePlayerButton; }
int GameState::getFreezePlayerButton() { return _freezePlayerButton; }
void GameState::setP1PointsMem( int p1PointsMem ) { _p1PointsMem = p1PointsMem; }
int GameState::getP1PointsMem() { return _p1PointsMem; }
void GameState::setP2PointsMem( int p2PointsMem ) { _p2PointsMem = p2PointsMem; }
int GameState::getP2PointsMem() { return _p2PointsMem; }
void GameState::setP1GamesMem( int p1GamesMem ) { _p1GamesMem = p1GamesMem; }
int GameState::getP1GamesMem() { return _p1GamesMem; }
void GameState::setP2GamesMem( int p2GamesMem ) { _p2GamesMem = p2GamesMem; }
int GameState::getP2GamesMem() { return _p2GamesMem; }
void GameState::setP1SetsMem( int p1SetsMem ) { _p1SetsMem = p1SetsMem; }
int GameState::getP1SetsMem() { return _p1SetsMem; }
void GameState::setP2SetsMem( int p2SetsMem ) { _p2SetsMem = p2SetsMem; }
int GameState::getP2SetsMem() { return _p2SetsMem; }
void GameState::setNow( unsigned long now ) { _now = now; }
unsigned long GameState::getNow() { return _now; }
void GameState::setToggle( int toggle ) { _toggle = toggle; }
int GameState::getToggle() { return _toggle; }
void GameState::setTieBreakOnly( int tieBreakOnly ) { _tieBreakOnly = tieBreakOnly; }
int GameState::getTieBreakOnly() { return _tieBreakOnly; }
void GameState::setTieBreakMem( int tieBreakMem ) { _tieBreakMem = tieBreakMem; }
int GameState::getTieBreakMem() { return _tieBreakMem; }
void GameState::setMatchTieBreakMem( int setMatchTieBreakMem ) { _matchTieBreakMem = setMatchTieBreakMem; }
int GameState::getMatchTieBreakMem() { return _matchTieBreakMem; }
int GameState::getPlayer1Points() const { return _player1_points; }
void GameState::setPlayer1Points( int player1Points ) { _player1_points = player1Points; }
int GameState::getPlayer2Points() const { return _player2_points; }
void GameState::setPlayer2Points( int player2Points ) { _player2_points = player2Points; }
int GameState::getPlayer1Games() const { return _player1_games; }
void GameState::setPlayer1Games( int player1Games ) { _player1_games = player1Games; }
int GameState::getPlayer2Games() const { return _player2_games; }
void GameState::setPlayer2Games( int player2Games ) { _player2_games = player2Games; }
int GameState::getPlayer1Sets() const { return _player1_sets; }
void GameState::setPlayer1Sets( int player1Sets ) { _player1_sets = player1Sets; }
int GameState::getPlayer2Sets() const { return _player2_sets; }
void GameState::setPlayer2Sets( int player2Sets ) { _player2_sets = player2Sets; }
int GameState::getPlayer1Matches() const { return _player1_matches; }
void GameState::setPlayer1Matches( int player1Matches ) { _player1_matches = player1Matches; }
int GameState::getPlayer2Matches() { return _player2_matches; }
void GameState::setPlayer2Matches( int player2Matches ) { _player2_matches = player2Matches; }

std::map<int, int> GameState::getPlayer1SetHistory() {
    return _player1_set_history; }
void GameState::setPlayer1SetHistory( std::map<int, int> player1_set_history ) {
    _player1_set_history = player1_set_history;
}
std::map<int, int> GameState::getPlayer2SetHistory() {
    return _player2_set_history; }
void GameState::setPlayer2SetHistory( std::map<int, int> player2_set_history ) {
    _player2_set_history = player2_set_history;}

int GameState::getUpdateDisplayDelay() { return 0; }
void GameState::setGameHistory(std::map<int, int> game_history) { _game_history = game_history; }
std::map<int, int> GameState::getGameHistory() { return _game_history; }
