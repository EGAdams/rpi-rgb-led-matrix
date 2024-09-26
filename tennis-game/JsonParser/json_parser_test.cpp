// main.cpp
#include <iostream>
#include "JsonParser.h"

int jsonParserMain() {
    std::string jsonString = "{\"runner\":\"FetchRunner\",\"url\":\"https://americansjewelry.com/libraries/local-php-api/index.php/\",\"new_id\":\"InitSupervisor_2021\"}";
    JsonParser parser;
    auto parsedJson = parser.parse(jsonString);

    // Print parsed data
    for (const auto& kv : parsedJson) {
        std::cout << kv.first << ": " << kv.second << std::endl;
    }

    return 0;
}