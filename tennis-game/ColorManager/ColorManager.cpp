#include "ColorManager.h"

rgb_matrix::Color ColorManager::getColor(const std::string& colorName) {
    static std::unordered_map<std::string, rgb_matrix::Color> colorMap = {
        {"RED", rgb_matrix::Color(255, 0, 0)},
        {"GREEN", rgb_matrix::Color(0, 255, 0)},
        {"BLUE", rgb_matrix::Color(0, 0, 255)},
        {"YELLOW", rgb_matrix::Color(255, 255, 0)},
        {"ORANGE", rgb_matrix::Color(255, 165, 0)},
        {"PURPLE", rgb_matrix::Color(128, 0, 128)},
        {"WHITE", rgb_matrix::Color(255, 255, 255)},
        {"BLACK", rgb_matrix::Color(0, 0, 0)}
    };

    auto it = colorMap.find(colorName);
    if (it != colorMap.end()) {
        return it->second;
    } else {
        // Default to black if color not found
        return rgb_matrix::Color(0, 0, 0);
    }
}
