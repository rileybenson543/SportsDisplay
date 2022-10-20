#pragma once
#include <string>
#include <filesystem>
enum RequestType {
    NEWS,
    SCORES,
    TEAMS,
    SPECIFIC_TEAM,
    SPECIFIC_TEAM_RECORD,
    TEAMS_ON_BYE
};
using std::string;
void getRequest(RequestType type);
int downloadImageCurl(string* url, std::filesystem::path* destinationFilePath);
void processData(string* unformatted, RequestType type);