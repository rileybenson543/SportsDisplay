#pragma once
#include <string>
#include <filesystem>
#include "Event.h"
#include "Team.h"
#include "NewsItem.h"
#include <unordered_map>
#include "Scheduler.h"

struct SportsDates;
enum class SportsType;
inline std::unordered_map<int64_t, Event*> events;
inline std::unordered_map<int64_t, Team*> teams;


enum RequestType {
    NEWS,
    SCORES,
    TEAMS,
    SPECIFIC_TEAM,
    SPECIFIC_TEAM_RECORD,
    TEAMS_ON_BYE,
    SPECIFIC_EVENT,
    DATES_ESPN,
    DATES_NASCAR
};
using std::string;
void getRequest(RequestType type, int64_t id = 0);
int downloadImageCurl(const string* url, const std::filesystem::path* destinationFilePath);
void processData(string* unformatted, RequestType type);
std::vector<string> getEventsStrings();
std::vector<int>* get_in_progress_games();
std::unordered_map<int64_t, Event*>* getEvents();
std::unordered_map<int64_t, Team*>* getTeams();
void set_sports_mode(SportsType type);
SportsDates get_start_end_date(SportsType st, const jed_utils::datetime* today);