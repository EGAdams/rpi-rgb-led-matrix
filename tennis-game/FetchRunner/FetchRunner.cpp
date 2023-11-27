#include "FetchRunner.h"
#include <iostream>
#include <sstream>

FetchRunner::FetchRunner( const std::string& config ) {

    // print the config
    std::cout << "Config: " << config << std::endl;
    
    initializeCurl();
    setupHeaders();
}

FetchRunner::~FetchRunner() {
    curl_easy_cleanup(curl);
    curl_slist_free_all(url_encoded_header);
    curl_slist_free_all(json_header);
}

void FetchRunner::initializeCurl() {
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl." << std::endl;
    }
}

void FetchRunner::setupHeaders() {
    url_encoded_header = curl_slist_append(url_encoded_header, "Content-Type: application/x-www-form-urlencoded");
    json_header = curl_slist_append(json_header, "Content-Type: application/json");
}

size_t FetchRunner::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



void FetchRunner::run(const std::string& apiArgsType, const std::string& apiArgs, std::function<void(const std::string&)> callback) {
    // parse apiArgs from a string into a json object
    std::cout << "apiArgs: " << apiArgs << std::endl;
    

    CURLcode res;
    if (curl) {
        // Set the headers based on the method type
        if (apiArgsType == "POST") {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, json_header);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, apiArgs.c_str()); // Assuming apiArgs is already a JSON string
        } else {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, url_encoded_header);
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, apiArgsType.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Process response here
            std::cout << "Response: " << response_string << std::endl;
            callback(response_string);
        }
    }
}
