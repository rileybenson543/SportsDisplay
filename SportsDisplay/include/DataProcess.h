#pragma once
#include <string>
#include <filesystem>
#include "Event.h"
#include "Team.h"
#include "NewsItem.h"
#include <unordered_map>




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
int downloadImageCurl(const string* url, const std::filesystem::path* destinationFilePath);
void processData(string* unformatted, RequestType type);
std::vector<string> getEventsStrings();
std::unordered_map<int, Event*>* getEvents();
std::unordered_map<int, Team*>* getTeams();