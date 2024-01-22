#include "IFileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

class FileReader : public IFileReader {
public:
    std::string readFileContent( const std::string& filePath ) override;
};
