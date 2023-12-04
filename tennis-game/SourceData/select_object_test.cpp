#include <iostream>
#include "SourceData.h"

int main() {
    // Replace with the actual API URL and a valid object_view_id for testing
    std::string apiUrl = "https://americansjewelry.com/libraries/local-php-api/index.php/";
    std::string testObjectId = "SaveConfigLogger_2021";

    // Create SourceData instance with the API URL
    SourceData sourceData(apiUrl);

    // Call selectObject and print the result
    // Capture testObjectId in the lambda's capture list
    sourceData.selectObject(testObjectId, [testObjectId](const std::string& response) {
        std::cout << "Response for object " << testObjectId << ": " << response << std::endl;
    });

    // Add a mechanism to keep the program running until the async call is complete
    std::cout << "Press enter to exit..." << std::endl;
    std::getchar();

    return 0;
}

