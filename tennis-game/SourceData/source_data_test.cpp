// #include "SourceData.h"
// #include <cassert>
// #include <iostream>
// #include <functional>

// // Test function
// void testSelectObject() {
//     SourceData sourceData("https://americansjewelry.com/libraries/local-php-api/index.php/");

//     // Call the selectObject method with the object_view_id "McbaStartupMonitor_2021"
//     sourceData.selectObject("McbaStartupMonitor_2021", [](const std::string& response) {
//         // Expected JSON output from the API
//         std::string expectedResponse = R"({"id":399517,"object_view_id":"McbaStartupMonitor_2021","object_data":"{\"logObjects\":[],\"object_view_id\":\"McbaStartupMonitor_2021\",\"objectModel\":{},\"logObjectFactory\":{},\"monitorLed\":{\"config\":{\"new_id\":\"2021\",\"table\":\"monitored_objects\"},\"classObject\":{\"background_color\":\"lightyellow\",\"text_align\":\"left\",\"margin_top\":\"2px\",\"color\":\"black\"},\"ledText\":\"ready.\",\"RUNNING_COLOR\":\"lightyellow\",\"PASS_COLOR\":\"lightgreen\",\"FAIL_COLOR\":\"#fb6666\"}}")";

//         // Check if the response matches the expected output
//         assert(response == expectedResponse);
//         std::cout << "Test passed: Response matches the expected output." << std::endl;
//     });
// }

// int main() {
//     testSelectObject();
//     return 0;
// }
