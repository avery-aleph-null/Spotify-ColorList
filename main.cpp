#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Function to get Spotify API token
std::string getAccessToken(const std::string& clientId, const std::string& clientSecret) {
    CURL* curl = curl_easy_init();
    std::string token;

    if (curl) {
        std::string authUrl = "https://accounts.spotify.com/api/token";
        std::string authData = "grant_type=client_credentials";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        std::string credentials = clientId + ":" + clientSecret;
        std::string authHeader = "Authorization: Basic " + credentials;

        headers = curl_slist_append(headers, authHeader.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, authUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, authData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Callback to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char* data, size_t size, size_t nmemb, std::string* output) {
            output->append(data, size * nmemb);
            return size * nmemb;
            });

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &token);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    auto jsonResponse = nlohmann::json::parse(token);
    return jsonResponse["access_token"];
}
