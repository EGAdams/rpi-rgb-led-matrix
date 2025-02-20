#include "RemoteLocker.h"

RemoteLocker::RemoteLocker( GameState* gameStateArg ) { _gameState = gameStateArg; }
RemoteLocker::~RemoteLocker(){}

bool RemoteLocker::playerNotServing( int button ) {
    bool green_serve = _gameState->getServe() == 1 ? true : false;

    switch ( button ) {
    case UMPIRE_REMOTE_GREEN_SCORE:
    case UMPIRE_REMOTE_RED_SCORE:
    case UMPIRE_REMOTE_UNDO:
        return false;
        break;

    case RED_REMOTE_RED_SCORE:
    case RED_REMOTE_GREEN_SCORE:
    case RED_REMOTE_UNDO:
        return green_serve ? false : true;
        break;

    case GREEN_REMOTE_GREEN_SCORE:
    case GREEN_REMOTE_RED_SCORE:
    case GREEN_REMOTE_UNDO:
        return green_serve ? true : false;
        break;

    default:
        return false;
    }
}
