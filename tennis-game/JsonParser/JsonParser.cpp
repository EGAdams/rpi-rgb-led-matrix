// JsonParser.cpp
#include "JsonParser.h"

std::map<std::string, std::string> JsonParser::parse(const std::string& jsonString) {
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
