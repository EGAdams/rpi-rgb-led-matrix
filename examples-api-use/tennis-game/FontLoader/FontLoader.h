#pragma once

#include <led-matrix.h>

class FontLoader {
public:
    FontLoader(const char* fontFile);

    bool LoadFont(rgb_matrix::Font& font);

private:
    const char* _fontFile;
};