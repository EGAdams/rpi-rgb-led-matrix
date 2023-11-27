#ifndef FETCHRUNNER_H
#define FETCHRUNNER_H

#include <string>
#include <curl/curl.h>

class FetchRunner {
public:
    FetchRunner(const std::string& config);
    ~FetchRunner();

    void run(const std::string& apiArgsType, const std::string& apiArgs);

private:
    std::string url;
    struct curl_slist* url_encoded_header;
    struct curl_slist* json_header;
    CURL* curl;

    void initializeCurl();
    void setupHeaders();
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // FETCHRUNNER_H

