#ifndef FETCHRUNNER_H
#define FETCHRUNNER_H

#include <string>
#include <map>
#include <functional>
#include "../IQueryResultProcessor.h"
#include <curl/curl.h>
#include "/usr/include/jsoncpp/json/json.h"
#include <sstream>

class FetchRunner {
public:
    FetchRunner(const std::string& api_path);
    ~FetchRunner();
    void run(const std::string& apiArgsType, const std::string& body, 
             std::function<void(const std::string&)> callback);

private:
    std::string url;
    std::map<std::string, std::string> url_encoded_header;
    std::map<std::string, std::string> json_header;

    std::string prepareRequestBody(const std::string& apiArgsType, const std::string& body);
};

#endif // FETCHRUNNER_H 
