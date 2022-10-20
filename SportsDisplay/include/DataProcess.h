#pragma once
#include <string>
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
string downloadImage(string* url, string* filename);
void processData(string* unformatted, RequestType type);