#include <gtest/gtest.h>
#include "FileReader.h"

TEST( FileReaderTest, ReadFileContent_ValidFile_ReturnsFileContent ) {
    // Arrange
    std::string filePath = "/path/to/valid/file.txt";
    std::string expectedContent = "This is the content of the file.";

    // Act
    FileReader reader;
    std::string actualContent = reader.readFileContent( filePath );

    // Assert
    EXPECT_EQ( actualContent, expectedContent );
}


TEST( FileReaderTest, ReadFileContent_InvalidFile_ReturnsEmptyString ) {
    // Arrange
    std::string filePath = "/path/to/invalid/file.txt";

    // Act
    FileReader reader;
    std::string actualContent = reader.readFileContent( filePath );

    // Assert
    EXPECT_EQ( actualContent, "" );
}

TEST( FileReaderTest, ReadFileContent_EmptyFile_ReturnsEmptyString ) {
    // Arrange
    std::string filePath = "./empty_file.txt";

    // Act
    FileReader reader;
    std::string actualContent = reader.readFileContent(filePath);

    // Assert
    EXPECT_EQ(actualContent, "");
}
