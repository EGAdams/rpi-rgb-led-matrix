#include "SetHistoryText.h"

SetHistoryText::SetHistoryText( GameState* gameState ) : _gameState( gameState ) {}
SetHistoryText::~SetHistoryText() { std::cout << "destructing SetHistoryText..." << std::endl; }

std::string SetHistoryText::getSetHistoryText( int player ) {
    std::string setHistoryText = _buildString( player );
    return setHistoryText;
}

std::string SetHistoryText::_buildString( int player ) {
    std::cout << "Entering _buildString, player: " << player << std::endl;

    // Debugging player index and gameState validity
    if ( _gameState == nullptr ) {
        std::cout << "Error: _gameState is nullptr" << std::endl;
        return ""; // Early return to prevent dereferencing nullptr
    }

    std::map<int, int> setHistory = player == PLAYER_ONE_SET_INDEX ?
        _gameState->getPlayer1SetHistory() : _gameState->getPlayer2SetHistory();

    std::cout << "setHistory obtained. Size: " << setHistory.size() << std::endl;

    std::string setHistoryText = "";
    int currentSet = _gameState->getCurrentSet();

    std::cout << "Current Set: " << currentSet << std::endl;

    for ( int i = 1; i <= currentSet; i++ ) {
        if ( setHistory.find( i ) == setHistory.end() ) {
            std::cout << "Warning: Set history does not contain key: " << i << std::endl;
        }
        else {
            setHistoryText += std::to_string( setHistory[i] );
        }
        setHistoryText += " ";

        // Debugging each iteration
        std::cout << "Iteration " << i << ", setHistoryText: " << setHistoryText << std::endl;
    }

    std::cout << "Final setHistoryText: " << setHistoryText << std::endl;
    return setHistoryText;
}
