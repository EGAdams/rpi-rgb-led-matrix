#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "../../include/graphics.h" // Assuming this includes rgb_matrix::Font
#include <string>
#include <unordered_map>

class FontManager {
public:
    const rgb_matrix::Font& getFont(const std::string& fontName);

private:
    std::unordered_map<std::string, rgb_matrix::Font> _fontCache;
    void _loadFont(const std::string& fontName);
};
#endif // FONTMANAGER_H
