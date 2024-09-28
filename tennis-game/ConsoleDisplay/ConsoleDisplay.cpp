#include "../ColorManager/ColorManager.h"
#include "../ConsoleDisplay/ConsoleDisplay.h"

ConsoleDisplay::ConsoleDisplay( ColorManager* colorManager ) : _colorManager( colorManager ) {
    // Initialize the console display
    // ...
}

void ConsoleDisplay::drawText( const std::string& text, int x, int y ) {
    // Retrieve the ASCII color code from ColorManager
    const std::string& colorCode = _colorManager->getAsciiColorCode( _current_color_string );
    const std::string& resetCode = _colorManager->getAsciiColorCode( "RESET" );

    // Move the cursor to the specified position
    std::cout << "\033[" << y << ";" << x << "H";  // Cursor movement to (x, y)

    // Output the text with the color and then reset the color
    std::cout << colorCode << text << resetCode << std::endl;
}

void ConsoleDisplay::clearScreen() {
    // Clear the terminal screen and move cursor to the top-left
    std::cout << "\033[2J\033[H";
}
