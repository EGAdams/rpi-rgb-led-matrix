// JsonParser.h
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <map>

class JsonParser {
public:
    std::map<std::string, std::string> parse(const std::string& jsonString);
};

#endif // JSON_PARSER_H
