#ifndef FETCHRUNNER_H
#define FETCHRUNNER_H

#include <string>
#include <map>
#include <functional>
#include "../IQueryResultProcessor.h" // Include this if necessary

class FetchRunner {
public:
    FetchRunner(const std::string& api_path);
    void run(const std::string& apiArgsType, const std::string& body, 
             std::function<void(const std::string&)> callback);

private:
    std::string url;
    std::map<std::string, std::string> url_encoded_header;
    std::map<std::string, std::string> json_header;

    std::string prepareRequestBody(const std::string& apiArgsType, const std::string& body);
};

#endif // FETCHRUNNER_H
