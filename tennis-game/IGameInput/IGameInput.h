#ifndef GAME_INPUT_H
#define GAME_INPUT_H

class IGameInput {
public:
    virtual ~IGameInput() = default;
    virtual int getInput() = 0;
};

#endif