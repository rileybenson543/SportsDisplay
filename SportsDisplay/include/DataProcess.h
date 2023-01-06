#pragma once
#include <string>
#include <filesystem>
#include "Event.h"
#include "Team.h"
#include "NewsItem.h"
#include <unordered_map>

inline std::unordered_map<int64_t, Event*> events;
inline std::unordered_map<int64_t, Team*> teams;

enum SportsType
{
	NFL,
    NHL,
    NASCAR
};
enum RequestType {
    NEWS,
    SCORES,
    TEAMS,
    SPECIFIC_TEAM,
    SPECIFIC_TEAM_RECORD,
    TEAMS_ON_BYE,
    SPECIFIC_EVENT
};
using std::string;
void getRequest(RequestType type, int64_t id = 0);
int downloadImageCurl(const string* url, const std::filesystem::path* destinationFilePath);
void processData(string* unformatted, RequestType type);
std::vector<string> getEventsStrings();
std::vector<int>* get_in_progress_games();
std::unordered_map<int64_t, Event*>* getEvents();
std::unordered_map<int64_t, Team*>* getTeams();
void setMode(const SportsType type);