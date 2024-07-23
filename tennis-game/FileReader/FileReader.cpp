#include "./FileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string FileReader::readFileContent(const std::string& filePath) {
    std::stringstream buffer;
    std::ifstream file(filePath, std::ios::in);

    if (!file) {
        std::cerr << "Error opening file for reading: " << filePath << std::endl;
        return "";
    }

    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
