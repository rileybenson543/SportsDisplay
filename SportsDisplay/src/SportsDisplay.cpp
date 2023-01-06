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
using namespace Serial;
//
//std::unordered_map<int, Event*> events;
//std::unordered_map<int, Team*> teams;


int main()
{
	setMode(SportsType::NFL);

	getRequest(RequestType::TEAMS);
	std::cout << "Successfully downloaded and parsed team data" << "\n";
	auto start_t = std::chrono::high_resolution_clock::now();
	getRequest(RequestType::SCORES);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start_t;
	std::cout << duration.count() << std::endl;
	std::cout << "Received updated scores" << std::endl;

	//std::unordered_map<int, Event*> events = *getEvents();
	//std::unordered_map<int, Team*> teams = *getTeams();
	
	start();

	sleep_for(seconds(5));

	while (true) {
		if (get_in_progress_games()->empty() == false)
		{
			std::vector<int> in_progress = *get_in_progress_games();
			for (const int key : in_progress)
				display_game(key);
		}
		else {
			for (auto event = events.begin(); event != events.end(); event++)
				display_game(event->first);
		}
		
		// Update data before beginning next iteration
		getRequest(RequestType::SCORES);
		//events = *getEvents();
		//teams = *getTeams();
	}
}

// Need to still implement a way to validate the data received
// with bool ok = simdjson::validate_utf8(string, std::strlen(string));

void display_game(int event_id)
{
	//events = *getEvents();
	Event* val = events[event_id];
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
	if (sendMessage(s) == 1) return;

	s.bmp = teams.at(val->awayTeamId)->getBitmap();
	if (sendMessage(s) ==  1) return;

	SerialMessage previousMessage;

	for (int j = 0; j < 10; j++)
	{
		sleep_for(milliseconds(500));
		getRequest(RequestType::SPECIFIC_EVENT, event_id);
		//events = *getEvents();
		Event* e = events[event_id];
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