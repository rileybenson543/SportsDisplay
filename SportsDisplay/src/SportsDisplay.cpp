// SportsDisplay.cpp : Defines the entry point for the application.
#include "SportsDisplay.h"
#include "DataProcess.h"
#include <iostream>
#include "ImageProcessing.h"
#include "Team.h"

#include <chrono>
#include <ctime>

int main(void)
{
	//std::cout << getRequest(RequestType::NEWS) << std::endl;
	//time_t prev = time(nullptr);
	//time_t now = time(nullptr);
	//if (now - prev > 1) {
		getRequest(RequestType::TEAMS);
		std::cout << "Successfully downloaded and parsed team data" << "\n";
		while (true) {
			auto start = std::chrono::high_resolution_clock::now();
			getRequest(RequestType::SCORES);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;
			std::cout << duration.count() << std::endl;
		}
	std::cout << "Received updated scores" << std::endl;;
	//	time_t prev = time(nullptr);
	std::cin.get();
}

// Need to still implement a way to validate the data received
// with bool ok = simdjson::validate_utf8(string, std::strlen(string));