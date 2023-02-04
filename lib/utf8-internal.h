#ifndef RPI_GRAPHICS_UTF8_H
#define RPI_GRAPHICS_UTF8_H

#include <stdint.h>

// Utility function that reads and decodes UTF-8 encoded characters from a string.
// Assumes that the string is encoded correctly in UTF-8 format.
template <typename byte_iterator>
uint32_t next_UTF8_codepoint( byte_iterator &string_iterator ) {
uint32_t codepoint = string_iterator++;
// Check if the first byte of the character is one byte long (7 bits).
if ( codepoint < 0x80 ) {
    return codepoint; // Iterator already incremented.
}

// Check if the character is two bytes long (11 bits).
/*
    The code next fragment ( else if ) is checking for 11 bits in the bottom of the codepoint variable, 
    which represents two bytes of UTF-8 data. If the value is equal to 0xC0 (11000000 in binary) 
    then the next 6 bits in the codepoint variable are shifted 6 bits to the left and are combined 
    with the 6 least significant bits of the string iterator variable. The resulting value is the 
    decoded code point.
*/
else if (( codepoint & 0xE0 ) == 0xC0 ) {

    // the next line of code shifts the next 6 bits in the codepoint variable to the left
    // and then combines the result with the 6 least significant bits of the string iterator variable.
    codepoint = (( codepoint & 0x1F ) << 6 ) + ( string_iterator & 0x3F );

} else if (( codepoint & 0xF0 ) == 0xE0 ) { 
    
    // Check if the character is three bytes long (16 bits).
    codepoint = (( codepoint & 0x0F) << 12) + (( string_iterator & 0x3F ) << 6 );
    codepoint += ( ++string_iterator & 0x3F );
}

// Check if the character is four bytes long (21 bits).
else if (( codepoint & 0xF8) == 0xF0) {
    codepoint = (( codepoint & 0x07) << 18) + (( string_iterator & 0x3F ) << 12 );
    codepoint += ( ++string_iterator & 0x3F ) << 6;
    codepoint += ( ++string_iterator & 0x3F );
}
// Check if the character is five bytes long (26 bits).
else if (( codepoint & 0xFC) == 0xF8) {
    codepoint = (( codepoint & 0x03) << 24 ) + (( string_iterator & 0x3F ) << 18 );
    codepoint += ( ++string_iterator & 0x3F ) << 12;
    codepoint += ( ++string_iterator & 0x3F ) << 6;
    codepoint += ( ++string_iterator & 0x3F );
}
// Check if the character is six bytes long (31 bits).
else if (( codepoint & 0xFE) == 0xFC) {
    codepoint = (( codepoint & 0x01) << 30) + (( string_iterator & 0x3F ) << 24 );
    codepoint += ( ++string_iterator & 0x3F ) << 18;
    codepoint += ( ++string_iterator & 0x3F ) << 12;
    codepoint += ( ++string_iterator & 0x3F ) << 6;
    codepoint += ( ++string_iterator & 0x3F );
}
++string_iterator;
return codepoint;
}

#endif // RPI_GRAPHICS_UTF8_H