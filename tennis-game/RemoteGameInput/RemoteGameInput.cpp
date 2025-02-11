#include "RemoteGameInput.h"

RemoteGameInput::RemoteGameInput( Inputs* inputs ) : _inputs( inputs ) {
    print( "constructing remote game input..." );
}

RemoteGameInput::~RemoteGameInput() {
    print( "destroying remote game input..." );
}

int RemoteGameInput::getInput() {
    print( "\n\n*** Getting input from inside remote game input... ***\n\n" );
    int input_read_from_ic = _inputs->read_mcp23017_value();
    if ( input_read_from_ic != 15 ) {
        print( "*** returing input_read_from_ic = " << std::to_string( input_read_from_ic ) << " ***\n\n" );
    }
    return input_read_from_ic;
}