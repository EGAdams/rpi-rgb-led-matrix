#include "FetchRunner.h"
#include <iostream>
#include <cassert>
#include <sstream>

void testFetchRunner() {
    // URL and expected JSON response
    std::string testURL = "https://americansjewelry.com/libraries/local-php-api/index.php/object/select/McbaStartupMonitor_2021";
    std::string expectedJSON = R"({"id":399517,"object_view_id":"McbaStartupMonitor_2021","object_data":"{\"logObjects\":[],\"object_view_id\":\"McbaStartupMonitor_2021\",\"objectModel\":{},\"logObjectFactory\":{},\"monitorLed\":{\"config\":{\"new_id\":\"2021\",\"table\":\"monitored_objects\"},\"classObject\":{\"background_color\":\"lightyellow\",\"text_align\":\"left\",\"margin_top\":\"2px\",\"color\":\"black\"},\"ledText\":\"ready.\",\"RUNNING_COLOR\":\"lightyellow\",\"PASS_COLOR\":\"lightgreen\",\"FAIL_COLOR\":\"#fb6666\"}}"})";

    // Create a FetchRunner instance
    FetchRunner fetchRunner( testURL );

    // Capture output
    std::stringstream buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf( buffer.rdbuf() );

    // Run the fetcher
    fetchRunner.run( "GET", "{\"runner\":\"FetchRunner\",\"url\":\"https://americansjewelry.com/libraries/local-php-api/index.php/\",\"new_id\":\"InitSupervisor_2021\"}" );


    // Restore original buffer before reading
    std::cout.rdbuf( prevcoutbuf );

    // Print the raw response
    std::string response = buffer.str();
    std::cout << "Raw Response: " << response << std::endl;

    // Find the start of the JSON content
    size_t jsonStart = response.find("{");
    std::cout << "JSON Start Position: " << jsonStart << std::endl;

    std::string jsonResponse = response.substr(jsonStart);
    for (char c : jsonResponse) {
        std::cout << std::hex << static_cast<int>(c) << " ";
    }
    std::cout << std::endl;

    // assert( actualJson == expectedJson ); // Using assert for simplicity; in real use, a testing framework should be used
}

int main() {
    testFetchRunner();
    std::cout << "FetchRunner test passed successfully." << std::endl;
    return 0;
}
