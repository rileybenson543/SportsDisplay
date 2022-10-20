
//
#include <iostream>
#include <curl/curl.h>
#include "DataProcess.h"
#include "Team.h"
#include "Event.h"
#include <json/json.h>
#include <filesystem>
#include <unordered_map>
#include <future>
#include "simdjson.h"




std::unordered_map<int, Team*> teams;
std::unordered_map<int, Event*> events;

std::vector<std::future<void>> futures;

string* newsURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/news");
string* scoreURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/scoreboard");
string* teamsURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/teams");
string* specificTeamURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/teams/"); // append team id //before request


using std::string, std::string_view;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
static size_t imageWriteCallback(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}


string* getRequestCurl(string* url) {
    CURL* curl;
    CURLcode res;

    string* readBuffer = new string();

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url->c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // function that receives this 
        // needs to delete it
        return readBuffer;
    }
    return nullptr;
}

void downloadImageCurl(string* url, string* filename) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url->c_str());
        if (std::filesystem::is_directory("fullres") == 0) {
            std::filesystem::create_directory("fullres");
        }
        std::filesystem::path fullpath = std::filesystem::current_path() / "fullres" / *filename;
        //string fullfilename = ".\\fullres\\" + *filename;
        FILE* fp = fopen(fullpath.string().c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imageWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    
}


void processDataSIMD(string* rawJsonData, RequestType type) {

    using namespace simdjson;
    padded_string ps = padded_string(*rawJsonData);
    ondemand::parser parser;
    auto doc = parser.iterate(ps);

    for (auto event : events) {
        delete(event.second);
    }

    for (auto currEvent : doc["events"]) {

        int64_t id = -1;
        string_view str_scheduledDatetime = "";
        string_view detail = "";
        string_view shortDetail = "";
        string_view name = "";
        string_view shortName = "";
        double clock = 999;
        string_view displayClock;
        int64_t period = -1;
        bool completed = false;
        string_view state = "";
        string_view locationName = "";
        int64_t homeTeamId = -1;
        int64_t awayTeamId = -1;
        int64_t homeTeamScore = -1;
        int64_t awayTeamScore = -1;
        string_view briefDownText = "";
        int64_t posessionTeamId = -1;
        bool isRedZone = false;

        currEvent["id"].get_int64_in_string().get(id);
        currEvent["date"].get_string().get(str_scheduledDatetime);
        currEvent["name"].get_string().get(name);
        currEvent["shortName"].get_string().get(shortName);
        currEvent["status"]["clock"].get_double().get(clock);
        currEvent["status"]["displayClock"].get_string().get(displayClock);
        currEvent["status"]["period"].get_int64_in_string().get(period);
        currEvent["status"]["type"]["detail"].get_string().get(detail);
        currEvent["status"]["type"]["shortDetail"].get_string().get(shortDetail);
        currEvent["status"]["type"]["completed"].get_bool().get(completed);
        currEvent["status"]["type"]["state"].get_string().get(state);
        currEvent["competitions"].at(0)["venue"]["fullName"].get_string().get(locationName);
        currEvent["competitions"].at(0)["competitors"].at(0)["id"].get_int64_in_string().get(homeTeamId);
        currEvent["competitions"].at(0)["competitors"].at(1)["id"].get_int64_in_string().get(awayTeamId);
        currEvent["competitions"].at(0)["competitors"].at(0)["score"].get_int64_in_string().get(homeTeamScore);
        currEvent["competitions"].at(0)["competitors"].at(1)["score"].get_int64_in_string().get(awayTeamScore);
        currEvent["competitions"].at(0)["situation"]["downDistanceText"].get_string().get(briefDownText);
        currEvent["competitions"].at(0)["situation"]["posession"].get_int64_in_string().get(posessionTeamId);
        currEvent["competitions"].at(0)["situation"]["isRedZone"].get_bool().get(isRedZone);

        Event* e = new Event(
            id, str_scheduledDatetime , detail, shortDetail, name, shortName, clock, displayClock,
            period, completed, state, locationName,
            homeTeamId, awayTeamId, homeTeamScore, awayTeamScore,
            briefDownText, posessionTeamId, isRedZone
         );

        events[id] = e;


        // team record information is stored in the event
        // request so we will gwt that data now
        string_view total = "";
        string_view home = "";
        string_view away = "";

        currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(0)["summary"].get_string().get(total);
        currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(1)["summary"].get_string().get(home);
        currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(2)["summary"].get_string().get(away);


        // pass the string to the Team to be processed and stored
        // TODO: need to use string_view instead of casting to string
        teams[homeTeamId]->setRecordFromString((string)total,(string)home,(string)away);

        currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(0)["summary"].get_string().get(total);
        currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(1)["summary"].get_string().get(home);
        currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(2)["summary"].get_string().get(away);

        // add to the away teams as well
        teams[awayTeamId]->setRecordFromString((string)total, (string)home, (string)away);
    }
}

void processData(string* unformatted, RequestType type) {
    Json::Value jsonData;
    Json::Reader jsonReader;
    string* headlineString = new string(); // MEMORY LEAK!!!!!
    if (jsonReader.parse(*unformatted, jsonData))
    {
        switch (type)
        {
        case NEWS:
            for (unsigned int i = 0; i < jsonData["articles"].size(); i++) {
                headlineString->append(jsonData["articles"][i]["headline"].asString()+"\n");
            }
            //std::cout << *headlineString << std::endl;
            break;
        case SCORES:
        {
            int jsonEventsSize = jsonData["events"].size();
            for (int i = 0; i < jsonEventsSize; i++) {
                Json::Value currEventJson = jsonData["events"][i];
                long id = stol(currEventJson["id"].asString());
                string downDistance = "";
                int posessionId = 0;
                bool redZone = false;
                if (currEventJson["competitions"][0].isMember("situation")) {
                    downDistance = currEventJson["competitions"][0]["situation"]["downDistanceText"].asString();
                    redZone = currEventJson["competitions"][0]["situation"]["isRedZone"].asBool();
                    if (currEventJson["competitions"][0]["situation"].isMember("posession")) {
                        posessionId = stoi(currEventJson["competitions"][0]["situation"]["posession"].asString());
                    }
                }
                int homeTeamId = stoi(currEventJson["competitions"][0]["competitors"][0]["id"].asString());
                int awayTeamId = stoi(currEventJson["competitions"][0]["competitors"][1]["id"].asString());
                events[id] = new Event(
                    stol(currEventJson["id"].asString()),
                    currEventJson["date"].asString(),
                    currEventJson["status"]["type"]["detail"].asString(),
                    currEventJson["status"]["type"]["shortDetail"].asString(),
                    currEventJson["name"].asString(),
                    currEventJson["shortName"].asString(),
                    currEventJson["status"]["clock"].asFloat(),
                    currEventJson["status"]["displayClock"].asString(),
                    currEventJson["status"]["period"].asInt(),
                    currEventJson["status"]["type"]["completed"].asBool(),
                    currEventJson["status"]["type"]["type"]["state"].asString(),
                    currEventJson["competitions"][0]["venue"]["fullName"].asString(),
                    homeTeamId, awayTeamId,
                    stoi(currEventJson["competitions"][0]["competitors"][0]["score"].asString()),
                    stoi(currEventJson["competitions"][0]["competitors"][1]["score"].asString()),
                    downDistance, posessionId, redZone
                );
                // set team records
                teams[homeTeamId]->setRecordFromString(
                    currEventJson["competitions"][0]["competitors"][0]["records"][0]["summary"].asString(),
                    currEventJson["competitions"][0]["competitors"][0]["records"][1]["summary"].asString(),
                    currEventJson["competitions"][0]["competitors"][0]["records"][2]["summary"].asString()
                );
                teams[awayTeamId]->setRecordFromString(
                    currEventJson["competitions"][0]["competitors"][1]["records"][0]["summary"].asString(),
                    currEventJson["competitions"][0]["competitors"][1]["records"][1]["summary"].asString(),
                    currEventJson["competitions"][0]["competitors"][1]["records"][2]["summary"].asString()
                );
            }

            // Set records for teams on bye week
            int numTeamsOnBye = jsonData["week"]["teamsOnBye"].size();
            for (int i = 0; i < numTeamsOnBye; i++) {
                int teamID = stoi(jsonData["week"]["teamsOnBye"][i]["id"].asString());

                string* teamData = getRequestCurl(&specificTeamURL->append(std::to_string(teamID)));
                processData(teamData, SPECIFIC_TEAM_RECORD);
            }
            break;
        }
        case TEAMS:
        {
            Json::Value jsonTeamsSection = jsonData["sports"][0]["leagues"][0]["teams"];
            int const jsonTeamsSize = jsonTeamsSection.size();
            for (int i = 0; i < jsonTeamsSize; i++) {
                int id = stoi(jsonTeamsSection[i]["team"]["id"].asString());
                teams[id] = new Team(
                    jsonTeamsSection[i]["team"]["id"].asString(),
                    jsonTeamsSection[i]["team"]["displayName"].asString(),
                    jsonTeamsSection[i]["team"]["nickname"].asString(),
                    jsonTeamsSection[i]["team"]["abbreviation"].asString(),
                    jsonTeamsSection[i]["team"]["color"].asString(),
                    jsonTeamsSection[i]["team"]["alternateColor"].asString(),
                    jsonTeamsSection[i]["team"]["logos"][0]["href"].asString()
                );
            }
            break;
        }

        case SPECIFIC_TEAM_RECORD:
        {
            int teamID = stoi(jsonData["team"]["id"].asString());
           
            teams[teamID]->setRecordFromString(
                jsonData["team"]["record"]["items"][0]["summary"].asString(),
                jsonData["team"]["record"]["items"][1]["summary"].asString(),
                jsonData["team"]["record"]["items"][2]["summary"].asString()
            );
            

            break;
        }
        case TEAMS_ON_BYE:
        {
            int numTeamsOnBye = jsonData["week"]["teamsOnBye"].size();
            for (int i = 0; i < numTeamsOnBye; i++) {
                int teamID = stoi(jsonData["week"]["teamsOnBye"][i]["id"].asString());
                string* teamData = getRequestCurl(&specificTeamURL->append(std::to_string(teamID)));
                processData(teamData, SPECIFIC_TEAM_RECORD);
            }
        }
        default:
            break;
        }
    }
}


void getRequest(RequestType type) {
    string* rawData;
    switch (type)
    {
    case(NEWS):
        rawData = getRequestCurl(newsURL);
        processData(rawData, type);
        break;
    case(SCORES):
    {
        rawData = getRequestCurl(scoreURL);        
        processDataSIMD(rawData, TEAMS);
        //for (int i = 0; i < jsonEventsSize; i++) {
            //futures.push_back(std::async(std::launch::async, processDataAsync, doc, TEAMS, i));
        //}
        //std::cout << "Spawned all threads" << std::endl;
        break;
    }
    case(TEAMS):
    {
        rawData = getRequestCurl(teamsURL);
        processData(rawData, type);

        string* rawEventData = getRequestCurl(scoreURL);
        processData(rawEventData, TEAMS_ON_BYE);

        delete rawEventData;

        break;
    }
    case(TEAMS_ON_BYE): 
    {

    }
    default:
        break;
    }

    delete rawData;
}


string downloadImage(string* url, string* filename) {
    downloadImageCurl(url,filename);
    std::filesystem::path fullpath = std::filesystem::current_path() / "fullres" / *filename;
    return fullpath.string();
}
