#ifndef IHISTORY_H
#define IHISTORY_H

#include "../googletest/googlemock/include/gmock/gmock.h"
#include <string>
#include <vector>
#include "../GameState/GameState.h"

class IHistory {
public:
    virtual ~IHistory() {}
    virtual void push( GameState gameState ) = 0;
    virtual GameState pop() = 0;
    virtual int size() = 0; };
#endif // IHISTORY_H