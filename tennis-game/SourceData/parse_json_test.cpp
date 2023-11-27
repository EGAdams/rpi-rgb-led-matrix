#include <iostream>
#include <string>
#include <map>
#include <sstream>

std::map<std::string, std::string> parseJson(const std::string& jsonString) {
    std::map<std::string, std::string> jsonMap;
    std::string key, value;
    bool isKey = true;
    std::string::const_iterator it = jsonString.begin();

    while (it != jsonString.end()) {
        std::string token;
        if (*it == '{' || *it == '}' || *it == ',' || *it == '\"' || *it == ':') {
            ++it; // Skip the JSON punctuation
            continue;
        }

        while (it != jsonString.end() && *it != '\"' && *it != ':' && *it != ',') {
            token += *it++;
        }

        if (isKey) {
            key = token;
            isKey = false;
        } else {
            value = token;
            jsonMap[key] = value;
            isKey = true;
        }
    }

    return jsonMap;
}

int main() {
    std::string jsonString = "{\"runner\":\"FetchRunner\",\"url\":\"https://americansjewelry.com/libraries/local-php-api/index.php/\",\"new_id\":\"InitSupervisor_2021\"}";
    auto parsedJson = parseJson(jsonString);

    // Print parsed data
    for (const auto& kv : parsedJson) {
        std::cout << kv.first << ": " << kv.second << std::endl;
    }

    return 0;
}