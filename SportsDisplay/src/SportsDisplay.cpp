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
	getRequest(RequestType::TEAMS);
	std::cout << "Successfully downloaded and parsed team data" << "\n";
	//while (true) {
		//time_t now = time(nullptr);
		//if (now - prev > 1) {
			getRequest(RequestType::SCORES);
			std::cout << "Received updated scores" << "\n";
		//	time_t prev = time(nullptr);
		//}
	//}
			std::cin.get();
}