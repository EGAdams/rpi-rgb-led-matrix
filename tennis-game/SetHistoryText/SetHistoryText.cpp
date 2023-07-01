#include "SetHistoryText.h"

SetHistoryText::SetHistoryText( GameState* gameState ) : _gameState( gameState ) {}
SetHistoryText::~SetHistoryText() { std::cout << "destructing SetHistoryText..." << std::endl; }

std::string SetHistoryText::getSetHistoryText( int player ) {  
    std::string setHistoryText = _buildString( player );
    return setHistoryText; }

std::string SetHistoryText::_buildString( int player ) {
    std::map< int, int > setHistory = player == 1 ? 
        _gameState->getPlayer1SetHistory() : _gameState->getPlayer2SetHistory();
    std::string setHistoryText = ""; 
    for ( long unsigned int i = 0; i < setHistory.size(); i++ ) {
        setHistoryText += std::to_string( setHistory[ i ] );
        setHistoryText += " "; }
    return setHistoryText; }