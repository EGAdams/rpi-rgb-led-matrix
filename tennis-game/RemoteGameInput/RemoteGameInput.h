#ifndef REMOTE_GAME_INPUT_H
#define REMOTE_GAME_INPUT_H
#include "../IGameInput/IGameInput.h"
#include "../Inputs/Inputs.h"

class RemoteGameInput : public IGameInput {
public:
    RemoteGameInput( Inputs* inputs );
    ~RemoteGameInput();
    int getInput() override;
private:
    Inputs* _inputs;
};

#endif
