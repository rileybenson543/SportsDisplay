// SportsDisplay.cpp : Defines the entry point for the application.
#include "SportsDisplay.h"
#include "DataProcess.h"
#include <iostream>
#include "Team.h"
#include "Event.h"
#include "Serial.h"
#include "NewsItem.h"
#include <chrono>
#include <ctime>
#include <ranges>


//std::unordered_map<int, Team*> teams;
//std::unordered_map<int, Event*> events;
//std::vector<NewsItem> news;

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

int main()
{
	//std::cout << getRequest(RequestType::NEWS) << std::endl;
	//time_t prev = time(nullptr);
	//time_t now = time(nullptr);
	//if (now - prev > 1) {
		getRequest(RequestType::TEAMS);
		std::cout << "Successfully downloaded and parsed team data" << "\n";
			auto start_t = std::chrono::high_resolution_clock::now();
			getRequest(RequestType::SCORES);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start_t;
			std::cout << duration.count() << std::endl;
			std::cout << "Received updated scores" << std::endl;


			std::unordered_map<int, Event*> events = *getEvents();
			std::unordered_map<int, Team*> teams = *getTeams();



			using namespace Serial;
			start();
			//std::vector vec = getEventsStrings();
			//SerialMessage s;
			//s.type = TransmissionType::SingleEvent;
			//s.num_instances = vec.size();
			//s.instances = vec;

			sleep_for(seconds(5));
			while (true) {
				for (auto i = events.begin(); i != events.end(); i++)
				{
					// First message containing scores
					// and time remaining
					int key = i->first;
					Event* val = i->second;
					SerialMessage s;
					s.type = TransmissionType::SingleEventWithBitmap;
					s.homeTeamId = val->homeTeamId;
					s.awayTeamId = val->awayTeamId;
					s.string_data = val->printString();
					s.sendingBitmap = true;

					sendMessage(s);

					// Messages for bitmaps
					s.type = TransmissionType::Bitmap;
					s.bmp = teams.at(val->homeTeamId)->getBitmap();
					sendMessage(s);

					s.bmp = teams.at(val->awayTeamId)->getBitmap();
					sendMessage(s);

					//auto start = high_resolution_clock::now();

					//auto end = high_resolution_clock::now();
					//auto diff = end - start;


					//sleep_for(seconds(1)-nanoseconds(diff));
					//std::cout << seconds(1) - nanoseconds(diff) << "\t";
					//std::cout << "Iterate" << "\n";

					SerialMessage previousMessage;

					for (int j = 0; j < 10; j++)
					{
						sleep_for(milliseconds(500));
						getRequest(RequestType::SPECIFIC_EVENT, key);
						events = *getEvents();
						Event* e = events[key];
						s.type = TransmissionType::SingleEvent;
						s.homeTeamId = e->homeTeamId;
						s.awayTeamId = e->awayTeamId; // needs to be phased out
						s.string_data = e->printString();
						s.sendingBitmap = false;
						if (s == previousMessage)
							continue;
						sendMessage(s);
						previousMessage = s;
					}

					//sleep_for(seconds(10));
				}

				// Update data before beginning next iteration
				getRequest(RequestType::SCORES);
				events = *getEvents();
				teams = *getTeams();
			}
	
	//	time_t prev = time(nullptr);
	std::cin.get();
}

// Need to still implement a way to validate the data received
// with bool ok = simdjson::validate_utf8(string, std::strlen(string));