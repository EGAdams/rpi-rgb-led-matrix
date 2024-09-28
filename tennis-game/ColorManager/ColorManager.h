#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include "../../include/graphics.h" // rgb_matrix::Color defined here
#include <string>
#include <unordered_map>

class ColorManager {
public:
    const rgb_matrix::Color& getColor(const std::string& colorName);  // Correct declaration
    const std::string& getAsciiColorCode(const std::string& colorName);  // Correct declaration
};
#endif // COLORMANAGER_H