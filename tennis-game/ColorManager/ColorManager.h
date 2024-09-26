#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <unordered_map>
#include <string>
#include "../../include/graphics.h" // Assuming this is where rgb_matrix::Color is defined

class ColorManager {
public:
    // Returns a reference to the color corresponding to the colorName
    // If the color is not found, returns a reference to the color black
    const rgb_matrix::Color& getColor(const std::string& colorName);
};

#endif // COLORMANAGER_H

