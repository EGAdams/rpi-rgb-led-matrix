#ifndef IFILEREADER_H
#define IFILEREADER_H

#include <string>

class IFileReader {
public:
    virtual ~IFileReader() {}

    // Reads the entire content of the file into a string and returns the result.
    virtual std::string readFileContent(const std::string& filePath) = 0;
};

#endif // IFILEREADER_H
