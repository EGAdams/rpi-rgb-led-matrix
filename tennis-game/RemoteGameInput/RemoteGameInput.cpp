#include "RemoteGameInput.h"

RemoteGameInput::RemoteGameInput( Inputs* inputs ) : _inputs( inputs ) {
    print( "constructing remote game input..." );
}

RemoteGameInput::~RemoteGameInput() {
    print( "destroying remote game input..." );
}

int RemoteGameInput::getInput() {
    int input_read_from_ic = _inputs->read_mcp23017_value();
    print( "input_read_from_ic: " << input_read_from_ic );
    return input_read_from_ic;
}