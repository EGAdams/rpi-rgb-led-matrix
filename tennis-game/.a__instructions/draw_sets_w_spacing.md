I am making a tennis scoreboard that has two rows of sets.  The sets are passed in to the drawSetsWithSpacing method like this for example:
playerOneString = "1 6"
playerTwoString = "0 1"

if the string is "1", the "x" offset should be two clicks to the right to align the digit.  If the string is "2, 3, 4, 5, or 6, the offset should be "0" or no offset.  So the offset added is determined by whether or not the digit is 1 or something other than 1.  Please modify the following C++ code to accomplish this:

# Source code to modify
```cpp
void SetDrawer::drawSetsWithSpacing( std::string playerOneSetString, std::string playerTwoSetString ) {
    int offset = 0;
    int y = START_ROW; 
    int x = 0;
    Color thirdRowColor( 0, 255, 0 );
    drawTextOnCanvas( x + SMALL_BEFORE + offset, y, thirdRowColor, playerOneSetString );
    y += _little_font.height() - 5;
    Color fourthRowColor( 255, 0, 0 );
    drawTextOnCanvas( x + SMALL_BEFORE, y, fourthRowColor, playerTwoSetString );
}
```