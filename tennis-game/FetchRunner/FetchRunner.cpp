#include "FetchRunner.h"
#include <curl/curl.h>
#include "/usr/include/jsoncpp/json/json.h"
#include <sstream>

FetchRunner::FetchRunner(const std::string& api_path) : url(api_path) {
    url_encoded_header = { {"Content-Type", "application/x-www-form-urlencoded"} };
    json_header = { {"Content-Type", "application/json"} };
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    }
    catch (std::bad_alloc& e) {
        // Handle memory couldn't be allocated
        return 0;
    }
}

void FetchRunner::run(const std::string& apiArgsType, const std::string& body,
                      std::function<void(const std::string&)> callback) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        // Headers
        struct curl_slist* headers = NULL;
        if (apiArgsType == "POST") {
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        } else {
            headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request, and check for errors
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "curl_easy_perform() failed: " + std::string(curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        response = "Failed to initialize CURL";
    }

    callback(response);
}

std::string FetchRunner::prepareRequestBody(const std::string& apiArgsType, const std::string& body) {
    Json::Value root;
    root["type"] = apiArgsType;
    root["body"] = body;
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, root);
}
