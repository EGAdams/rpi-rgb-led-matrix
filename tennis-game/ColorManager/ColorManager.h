// ColorManager.h
#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include "graphics.h"  // Assuming this includes rgb_matrix::Color
#include <string>
#include <unordered_map>

class ColorManager {
public:
    static rgb_matrix::Color getColor(const std::string& colorName);
};

#endif // COLORMANAGER_H
