
//
#include <iostream>
#include <curl/curl.h>
#include "DataProcess.h"
#include "Team.h"
#include "Event.h"
#include <json/json.h>
#include <filesystem>
#include <unordered_map>

CURL* curl;



//std::vector<Team>* teams = new std::vector<Team>;
std::unordered_map<int, Team*> teams;
std::unordered_map<int, Event> events;


string* newsURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/news");
string* scoreURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/scoreboard");
string* teamsURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/teams");
string* specificTeamURL = new string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/teams/"); // append team id //before request


using std::string;

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
    CURLcode res;

    string* readBuffer = new string();

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url->c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        return readBuffer;
    }
    return nullptr;
}

void downloadImageCurl(string* url, string* filename) {
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

void processData(string* unformatted, RequestType type) {
    Json::Value jsonData;
    Json::Reader jsonReader;
    string* headlineString = new string();
    if (jsonReader.parse(*unformatted, jsonData))
    {
        switch (type)
        {
        case NEWS:
            for (unsigned int i = 0; i < jsonData["articles"].size(); i++) {
                headlineString->append(jsonData["articles"][i]["headline"].asString()+"\n");
            }
            std::cout << *headlineString << std::endl;
            break;
        case SCORES:
        {
            int jsonEventsSize = jsonData["events"].size();
            for (int i = 0; i < jsonEventsSize; i++) {
                long id = stol(jsonData["events"][i]["id"].asString());
                string downDistance = "";
                int posessionId = 0;
                bool redZone = false;
                if (jsonData["events"][i]["competitions"][0].isMember("situation")) {
                    downDistance = jsonData["events"][i]["competitions"][0]["situation"]["downDistanceText"].asString();
                    redZone = jsonData["events"][i]["competitions"][0]["situation"]["isRedZone"].asBool();
                    if (jsonData["events"][i]["competitions"][0]["situation"].isMember("posession")) {
                        posessionId = stoi(jsonData["events"][i]["competitions"][0]["situation"]["posession"].asString());
                    }
                }
                int homeTeamId = stoi(jsonData["events"][i]["competitions"][0]["competitors"][0]["id"].asString());
                int awayTeamId = stoi(jsonData["events"][i]["competitions"][0]["competitors"][1]["id"].asString());
                events[id] = Event(
                    stol(jsonData["events"][i]["id"].asString()),
                    jsonData["events"][i]["date"].asString(),
                    jsonData["events"][i]["status"]["type"]["detail"].asString(),
                    jsonData["events"][i]["status"]["type"]["shortDetail"].asString(),
                    jsonData["events"][i]["name"].asString(),
                    jsonData["events"][i]["shortName"].asString(),
                    jsonData["events"][i]["status"]["clock"].asFloat(),
                    jsonData["events"][i]["status"]["displayClock"].asString(),
                    jsonData["events"][i]["status"]["period"].asInt(),
                    jsonData["events"][i]["status"]["type"]["completed"].asBool(),
                    jsonData["events"][i]["status"]["type"]["type"]["state"].asString(),
                    jsonData["events"][i]["competitions"][0]["venue"]["fullName"].asString(),
                    homeTeamId, awayTeamId,
                    stoi(jsonData["events"][i]["competitions"][0]["competitors"][0]["score"].asString()),
                    stoi(jsonData["events"][i]["competitions"][0]["competitors"][1]["score"].asString()),
                    downDistance, posessionId, redZone
                );
               
                // set team records
                teams[homeTeamId]->setRecordFromString(
                    jsonData["events"][i]["competitions"][0]["competitors"][0]["records"][0]["summary"].asString(),
                    jsonData["events"][i]["competitions"][0]["competitors"][0]["records"][1]["summary"].asString(),
                    jsonData["events"][i]["competitions"][0]["competitors"][0]["records"][2]["summary"].asString()
                );
                teams[awayTeamId]->setRecordFromString(
                    jsonData["events"][i]["competitions"][0]["competitors"][1]["records"][0]["summary"].asString(),
                    jsonData["events"][i]["competitions"][0]["competitors"][1]["records"][1]["summary"].asString(),
                    jsonData["events"][i]["competitions"][0]["competitors"][1]["records"][2]["summary"].asString()
                );

                // Set records for teams on bye week
                int numTeamsOnBye = jsonData["week"]["teamsOnBye"].size();
        
                for (int i = 0; i < numTeamsOnBye; i++) {
                    int teamID = stoi(jsonData["week"]["teamsOnBye"][i]["id"].asString());

                    string* teamData = getRequestCurl(&specificTeamURL->append(std::to_string(teamID)));
                    processData(teamData, SPECIFIC_TEAM_RECORD);
                }
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
        rawData = getRequestCurl(scoreURL);
        processData(rawData, type);
        break;
    case(TEAMS):
        rawData = getRequestCurl(teamsURL);
        processData(rawData, type);
        break;
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
