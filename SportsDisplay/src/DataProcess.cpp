#include <curl/curl.h>
#include "DataProcess.h"
#include "Team.h"
#include "Event.h"
#include <json/json.h>
#include <filesystem>
#include <unordered_map>
#include <future>
#include "simdjson.h"
#include "NewsItem.h"


//std::unordered_map<int, Team*> teams;
//std::unordered_map<int, Event*> events;

SportsType mode;

std::vector<int> in_progress_games;
std::vector<NewsItem> news;

std::vector<std::future<void>> futures;


string newsURL;
string scoreURL;
string teamsURL;
string specificTeamURL;

using std::string, std::string_view;
simdjson::ondemand::parser parser;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
static size_t imageWriteCallback(const void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}


string* getRequestCurl(const string* url) {
	auto* readBuffer = new string();

	if (CURL* curl = curl_easy_init()) {
        curl_easy_setopt(curl, CURLOPT_URL, url->c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);
        CURLcode res;
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // function that receives this 
        // needs to delete it
        return readBuffer;
    }
    return nullptr;
}

int downloadImageCurl(const string* url, const std::filesystem::path* destinationFilePath) {
	if (CURL* curl = curl_easy_init()) {
        curl_easy_setopt(curl, CURLOPT_URL, url->c_str());
        if (std::filesystem::is_directory("fullres") == 0) {
            std::filesystem::create_directory("fullres");
        }
        FILE* fp = fopen(destinationFilePath->string().c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imageWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        CURLcode res;
        res = curl_easy_perform(curl);
        // use "res" to determine success
        curl_easy_cleanup(curl);
        fclose(fp);
        return 0;
    }
    return 1;
    // log
}


void processDataSIMD(string* rawJsonData, RequestType type) {

    using namespace simdjson;
	auto const ps = padded_string(*rawJsonData);

    auto doc = parser.iterate(ps);


    switch (type) {

        case SCORES:
	    {
		    //events.clear();
		    for (std::pair<int64_t, Event*> event : events)
		    {
			    delete(event.second);
		    }

		    for (auto currEvent : doc["events"])
		    {
			    int64_t id = -1;
			    string_view str_scheduledDatetime;
			    string_view detail;
			    string_view shortDetail;
			    string_view name;
			    string_view shortName;
			    double clock = 999;
			    string_view displayClock;
			    int64_t period = -1;
			    bool completed = false;
			    string_view state;
			    string_view locationName;
			    int64_t homeTeamId = -1;
			    int64_t awayTeamId = -1;
			    int64_t homeTeamScore = -1;
			    int64_t awayTeamScore = -1;
			    string_view briefDownText;
			    int64_t posessionTeamId = -1;
			    int64_t yardLine = -1;
			    bool isRedZone = false;
				int64_t homeTeamTimeouts = -1;
				int64_t awayTeamTimeouts = -1;

			    currEvent["id"].get_int64_in_string().get(id);
			    currEvent["date"].get_string().get(str_scheduledDatetime);
			    currEvent["name"].get_string().get(name);
			    currEvent["shortName"].get_string().get(shortName);
			    currEvent["status"]["clock"].get_double().get(clock);
			    currEvent["status"]["displayClock"].get_string().get(displayClock);
			    currEvent["status"]["period"].get_int64().get(period);
			    currEvent["status"]["type"]["detail"].get_string().get(detail);
			    currEvent["status"]["type"]["shortDetail"].get_string().get(shortDetail);
			    currEvent["status"]["type"]["completed"].get_bool().get(completed);
			    currEvent["status"]["type"]["state"].get_string().get(state);
			    currEvent["competitions"].at(0)["venue"]["fullName"].get_string().get(locationName);
			    currEvent["competitions"].at(0)["competitors"].at(0)["id"].get_int64_in_string().get(homeTeamId);
			    currEvent["competitions"].at(0)["competitors"].at(1)["id"].get_int64_in_string().get(awayTeamId);
			    currEvent["competitions"].at(0)["competitors"].at(0)["score"].get_int64_in_string().get(homeTeamScore);
			    currEvent["competitions"].at(0)["competitors"].at(1)["score"].get_int64_in_string().get(awayTeamScore);
			    currEvent["competitions"].at(0)["situation"]["shortDownDistanceText"].get_string().get(briefDownText);
			    currEvent["competitions"].at(0)["situation"]["possession"].get_int64_in_string().get(posessionTeamId);
			    currEvent["competitions"].at(0)["situation"]["yardLine"].get_int64().get(yardLine);
			    currEvent["competitions"].at(0)["situation"]["isRedZone"].get_bool().get(isRedZone);
				currEvent["competitions"].at(0)["situation"]["homeTimeouts"].get_int64().get(homeTeamTimeouts);
				currEvent["competitions"].at(0)["situation"]["awayTimeouts"].get_int64().get(awayTeamTimeouts);


			    auto* e = new Event(
                    id, string(str_scheduledDatetime), string(detail), string(shortDetail), string(name), string(shortName), clock, string(displayClock),
                    period, completed, string(state), string(locationName),
				    homeTeamId, awayTeamId, homeTeamScore, awayTeamScore,
                    string(briefDownText), posessionTeamId, yardLine, isRedZone, homeTeamTimeouts, awayTeamTimeouts
			    );

			    events[id] = e;


			    // team record information is stored in the event
			    // request so we will get that data now
			    string_view total;
			    string_view home;
			    string_view away;

			    currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(0)["summary"].get_string().
				    get(total);
			    currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(1)["summary"].get_string().get(home);
			    currEvent["competitions"].at(0)["competitors"].at(0)["records"].at(2)["summary"].get_string().get(away);


			    // pass the string to the Team to be processed and stored
			    // TODO: need to use string_view instead of casting to string
			    teams[homeTeamId]->setRecords(string(total), string(home), string(away));

			    currEvent["competitions"].at(0)["competitors"].at(1)["records"].at(0)["summary"].get_string().
				    get(total);			  
			    currEvent["competitions"].at(0)["competitors"].at(1)["records"].at(1)["summary"].get_string().get(home);
			    currEvent["competitions"].at(0)["competitors"].at(1)["records"].at(2)["summary"].get_string().get(away);

			    // add to the away teams as well
			    teams[awayTeamId]->setRecords(string(total), string(home), string(away));
		    }
	    }
		break;
		case SPECIFIC_EVENT:
	    {

	        int64_t id = -1;
	        string_view str_scheduledDatetime;
	        string_view detail;
	        string_view shortDetail;
	        string_view name;
	        string_view shortName;
	        double clock = 999;
	        string_view displayClock;
	        int64_t period = -1;
	        bool completed = false;
	        string_view state;
	        string_view locationName;
	        int64_t homeTeamId = -1;
	        int64_t awayTeamId = -1;
	        int64_t homeTeamScore = -1;
	        int64_t awayTeamScore = -1;
	        string_view briefDownText;
	        int64_t posessionTeamId = -1;
			int64_t yardLine = -1;
	        bool isRedZone = false;
			int64_t homeTeamTimeouts = -1;
			int64_t awayTeamTimeouts = -1;

	        doc["id"].get_int64_in_string().get(id);
	        doc["date"].get_string().get(str_scheduledDatetime);
	        doc["name"].get_string().get(name);
	        doc["shortName"].get_string().get(shortName);
	        doc["status"]["clock"].get_double().get(clock);
	        doc["status"]["displayClock"].get_string().get(displayClock);
	        doc["status"]["period"].get_int64().get(period);
	        doc["status"]["type"]["detail"].get_string().get(detail);
	        doc["status"]["type"]["shortDetail"].get_string().get(shortDetail);
	        doc["status"]["type"]["completed"].get_bool().get(completed);
	        doc["status"]["type"]["state"].get_string().get(state);
	        doc["competitions"].at(0)["venue"]["fullName"].get_string().get(locationName);
	        doc["competitions"].at(0)["competitors"].at(0)["id"].get_int64_in_string().get(homeTeamId);
	        doc["competitions"].at(0)["competitors"].at(1)["id"].get_int64_in_string().get(awayTeamId);
	        doc["competitions"].at(0)["competitors"].at(0)["score"].get_int64_in_string().get(homeTeamScore);
	        doc["competitions"].at(0)["competitors"].at(1)["score"].get_int64_in_string().get(awayTeamScore);
	        doc["competitions"].at(0)["situation"]["shortDownDistanceText"].get_string().get(briefDownText);
	        doc["competitions"].at(0)["situation"]["possession"].get_int64_in_string().get(posessionTeamId);
	        doc["competitions"].at(0)["situation"]["yardLine"].get_int64().get(yardLine);
	        doc["competitions"].at(0)["situation"]["isRedZone"].get_bool().get(isRedZone);
			doc["competitions"].at(0)["situation"]["homeTimeouts"].get_int64().get(homeTeamTimeouts);
			doc["competitions"].at(0)["situation"]["awayTimeouts"].get_int64().get(awayTeamTimeouts);


			auto* e = new Event(
				id, string(str_scheduledDatetime), string(detail), string(shortDetail), string(name), string(shortName), clock, string(displayClock),
				period, completed, string(state), string(locationName),
				homeTeamId, awayTeamId, homeTeamScore, awayTeamScore,
				string(briefDownText), posessionTeamId, yardLine, isRedZone, homeTeamTimeouts, awayTeamTimeouts
			);


	        delete events[id];
	        events[id] = e;

			//std::cout << displayClock << std::endl;

	    }
        break;
        case NEWS:
        {
            for (auto article : doc["articles"]) {
                string_view headline;
                int64_t relatedTeamId = 0;

                article["categories"].at(2)["teamId"].get_int64().get(relatedTeamId);
                article["headline"].get_string().get(headline);

                news.emplace_back(headline, relatedTeamId);
            }
        }
        break;
    case TEAMS:
    {}
    break;

    case SPECIFIC_TEAM:
    {}
    break;

    case SPECIFIC_TEAM_RECORD:
	{}
    break;

    case TEAMS_ON_BYE:
	{}
    break;

    }
}

void processData(string* unformatted, RequestType type) {
    Json::Value jsonData;
    Json::Reader jsonReader;
    auto* headlineString = new string(); // MEMORY LEAK!!!!!
    if (!jsonReader.parse(*unformatted, jsonData))
	    return;
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
			    string downDistance;
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
				    downDistance, posessionId,
                    currEventJson["competitions"][0]["situation"]["lastPlay"]["end"]["yardLine"].asInt64(),
                    redZone, -1, -1
			    );
			    // set team records
			    teams[homeTeamId]->setRecords(
				    currEventJson["competitions"][0]["competitors"][0]["records"]["items"][0]["summary"].asString(),
				    currEventJson["competitions"][0]["competitors"][0]["records"]["items"][1]["summary"].asString(),
				    currEventJson["competitions"][0]["competitors"][0]["records"]["items"][2]["summary"].asString()
			    );
			    teams[awayTeamId]->setRecords(
				    currEventJson["competitions"][0]["competitors"][1]["records"]["items"][0]["summary"].asString(),
				    currEventJson["competitions"][0]["competitors"][1]["records"]["items"][1]["summary"].asString(),
				    currEventJson["competitions"][0]["competitors"][1]["records"]["items"][2]["summary"].asString()
			    );
		    }

		    // Set records for teams on bye week
		    int numTeamsOnBye = jsonData["week"]["teamsOnBye"].size();
		    for (int i = 0; i < numTeamsOnBye; i++) {
			    int teamID = stoi(jsonData["week"]["teamsOnBye"][i]["id"].asString());
			    string url_string = specificTeamURL + std::to_string(teamID);
			    string* teamData = getRequestCurl(&url_string);
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
			    // TODO: Need to clear out team map before making more
		    }
		    break;
	    }

    case SPECIFIC_TEAM_RECORD:
	    {
		    int teamID = stoi(jsonData["team"]["id"].asString());
           
		    teams[teamID]->setRecords(
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
			    string teamID = jsonData["week"]["teamsOnBye"][i]["id"].asString();
			    auto urlString = string();
			    urlString.append(specificTeamURL);
			    urlString.append(teamID);
			    string* teamData = getRequestCurl(&urlString);
			    processData(teamData, SPECIFIC_TEAM_RECORD);
		    }
	    }
    default:
	    break;
    }
}


void getRequest(RequestType type, int64_t id) {
	string* rawData;
    switch (type)
    {
    case NEWS:
        rawData = getRequestCurl(&newsURL);
        processData(rawData, type);
        break;
    case SCORES:
    {
        rawData = getRequestCurl(&scoreURL);        
        processDataSIMD(rawData, SCORES);
        //processData(rawData, SCORES);
        break;
    }
    case TEAMS:
    {
        rawData = getRequestCurl(&teamsURL);
        processData(rawData, type);

        string* rawEventData = getRequestCurl(&scoreURL);
        processData(rawEventData, TEAMS_ON_BYE);
        delete rawEventData; // may not be necessary

        break;
    }
    case TEAMS_ON_BYE: 
    {

    }
    case SPECIFIC_EVENT:
    {
        const string id_str = std::to_string(id);
        const string url_string = scoreURL + "/" + id_str;
        rawData = getRequestCurl(&url_string);
        processDataSIMD(rawData, SPECIFIC_EVENT);
    }   
    default:
        break;
    }

    delete rawData;
}

//std::vector<string> getEventsStrings() {
//    std::vector<string> events_strings = std::vector<string>();
//    for (const auto& [key, value] : events) {
//        events_strings.push_back(value->printString());
//    }
//    return events_strings;
//}
std::vector<int>* get_in_progress_games()
{
	in_progress_games.clear();
	for (auto i = events.begin(); i != events.end(); i++)
	{
		if (i->second->is_in_progress())
			in_progress_games.push_back(i->first);
	}
	return &in_progress_games;
}
inline std::unordered_map<int64_t, Event*>* getEvents()
{
    return &events;
}
inline std::unordered_map<int64_t, Team*>* getTeams()
{
    return &teams;
}
void setMode(const SportsType type)
{
	mode = type;
	switch (mode)
	{
	case NFL:
		{
			newsURL = string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/news");
			scoreURL = string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/scoreboard");
			teamsURL = string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/teams");
			specificTeamURL = string("http://site.api.espn.com/apis/site/v2/sports/football/nfl/teams/");
		}
		break;
	case NHL:
		{
			newsURL = string("http://site.api.espn.com/apis/site/v2/sports/hockey/nhl/news");
			scoreURL = string("http://site.api.espn.com/apis/site/v2/sports/hockey/nhl/scoreboard");
			teamsURL = string("http://site.api.espn.com/apis/site/v2/sports/hockey/nhl/teams");
			specificTeamURL = string("http://site.api.espn.com/apis/site/v2/sports/hockey/nhl/teams/");
		}
		break;
	case NASCAR:
		{}
		break;
	}
}