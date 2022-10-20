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
	//getRequest(RequestType::TEAMS);
	std::cout << "Successfully downloaded and parsed team data" << "\n";
	//time_t now = time(nullptr);
	//if (now - prev > 1) {
		while (true) {
			auto start = std::chrono::high_resolution_clock::now();
			getRequest(RequestType::NEWS);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;
			std::cout << duration.count() << std::endl;
		}
	std::cout << "Received updated scores" << std::endl;;
	//	time_t prev = time(nullptr);
	std::cin.get();
}