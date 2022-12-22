// SportsDisplay.cpp : Defines the entry point for the application.
#include "SportsDisplay.h"
#include "DataProcess.h"
#include <iostream>
#include "ImageProcessing.h"
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
				for (const auto& [key, val] : events)
				{
					// First message containing scores
					// and time remaining
					SerialMessage s;
					s.type = TransmissionType::SingleEvent;
					s.homeTeamId = val->homeTeamId;
					s.awayTeamId = val->awayTeamId;
					s.string_data = val->printString();

					sendMessage(s);

					// Messages for bitmaps
					s.type = TransmissionType::Bitmap;
					s.bmp = teams[val->homeTeamId]->getBitmap();
					sendMessage(s);

					s.bmp = teams[val->awayTeamId]->getBitmap();
					sendMessage(s);

					auto start = high_resolution_clock::now();
					getRequest(RequestType::SCORES);
					events = *getEvents();
					teams = *getTeams();
					auto end = high_resolution_clock::now();
					auto diff = end - start;

					sleep_for(seconds(10));

					//sleep_for(seconds(1)-nanoseconds(diff));
					//std::cout << seconds(1) - nanoseconds(diff) << "\t";
					std::cout << "Iterate" << "\n";
				}
			}

			// TODO need to allow updating a single
			// event and only updating all the events once
			// we are done iterating through them



			//s.homeTeamId = events[401437908]->homeTeamId;
			//s.awayTeamId = events[401437908]->awayTeamId;
			//std::vector<char>* homeBitmap_ptr = teams[events[401437908]->homeTeamId]->getBitmap();
			//std::vector<char>* awayBitmap_ptr = teams[events[401437908]->awayTeamId]->getBitmap();
			/*sendMessage(s);*/


	//	time_t prev = time(nullptr);
	std::cin.get();
}

// Need to still implement a way to validate the data received
// with bool ok = simdjson::validate_utf8(string, std::strlen(string));