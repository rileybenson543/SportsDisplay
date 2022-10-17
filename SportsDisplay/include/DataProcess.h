#pragma once
#include <string>
enum RequestType {
    NEWS,
    SCORES,
    TEAMS,
    SPECIFIC_TEAM,
    SPECIFIC_TEAM_RECORD
};
using std::string;
void getRequest(RequestType type);
string downloadImage(string* url, string* filename);
