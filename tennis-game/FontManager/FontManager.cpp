#include "FontManager.h"
#include <iostream>

const rgb_matrix::Font& FontManager::getFont(const std::string& fontName) {
    auto it = _fontCache.find(fontName);
    if (it != _fontCache.end()) {
        return it->second;
    }
    _loadFont(fontName);
    return _fontCache[fontName];
}

void FontManager::_loadFont(const std::string& fontName) {
    rgb_matrix::Font font;
    if (!font.LoadFont(fontName.c_str())) {
        std::cerr << "Failed to load font: " << fontName << std::endl;
        exit(1);
    }
    _fontCache[fontName] = font;
}
