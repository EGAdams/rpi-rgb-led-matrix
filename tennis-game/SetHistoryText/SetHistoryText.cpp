#include "SetHistoryText.h"

SetHistoryText::SetHistoryText( GameState* gameState ) : _gameState( gameState ) {}
SetHistoryText::~SetHistoryText() { std::cout << "destructing SetHistoryText..." << std::endl; }

std::string SetHistoryText::getSetHistoryText( int player ) {  
    std::string setHistoryText = _buildString( player );
    std::cout << "setHistoryText: " << setHistoryText << std::endl;
}

std::string _buildString( int player ) {
    std::string setHistoryText = "";
    for ( int i = 0; i < _gameState->getSetHistory( player ).size(); i++ ) {
        setHistoryText += std::to_string( _gameState->getSetHistory( player )[ i ] );
        setHistoryText += " "; }
    return setHistoryText; 
}