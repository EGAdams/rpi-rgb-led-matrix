#include "PointLeds.h"

PointLeds::PointLeds( Player* player1,
    Player* player2,
    PinInterface* pinInterface )
    : _player1( player1 ), _player2( player2 ), _pinInterface( pinInterface ) {
    _scoreBoardSet = false;
    }

PointLeds::PointLeds( Player* player1,
    Player* player2,
    ScoreBoard* scoreBoard )
    : _player1( player1 ), _player2( player2 ), _scoreBoard(   scoreBoard   ) {
    _scoreBoardSet = true;
}

PointLeds::~PointLeds() {}

void PointLeds::setScoreBoard( ScoreBoard* scoreBoard ) {
    // std::cout << "*** setting _scoreBoard in PointLeds object ... ***" << std::endl;
    _scoreBoard = scoreBoard;
    _scoreBoardSet = true;
}

void PointLeds::updateTBPoints() {
    /* std::cout << "*** ERROR: updateTBPoints is not implemented ***" << std::endl; */
}

void PointLeds::updatePoints() {
    // std::cout << "calling _scoreBoard->update() from inside PointLeds.  checking scoreboard bool.. " << std::endl;
    if ( _scoreBoardSet == true && _scoreBoard != nullptr ) {
        // std::cout << "Updating scoreboard..." << std::endl;
        _scoreBoard->update();
    } else {
        if (_scoreBoard == nullptr) {
            // std::cerr << "Error: _scoreBoard is nullptr" << std::endl;
        }
        // std::cout << "scoreboard bool is false, not updating scoreboard." << std::endl;
    }
    
    switch ( _player1->getPoints()) {
        case 0:
            // draw "00"
            break;
        case 1:
            // draw "10"
            break;
        case 2:
            // draw "15"
            break;
        case 3:
            // draw "30"
            break;
        case 4:
            // draw "40"
            break;
        case 5:
            // draw "40"
            break;

        case 99:
            // draw "Ad"
            break;

        case 100:
            // draw "blank"
            break;
    }

    switch ( _player2->getPoints() ) {
    case 0:
        // draw "00"
        break;
    case 1:
        // draw "10"
        break;
    case 2:
        // draw "15"
        break;
    case 3:
        // draw "30"
        break;
    case 4:
        // draw "40"
        break;
    case 5:
        // draw "40"
        break;
    case 99:
        // draw "Ad"
        break;

    case 100:
        // draw "blank"
        break;
    }
}


