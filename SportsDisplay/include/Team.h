#pragma once
#include <string>
#include <iostream>
#include <future>
#include <filesystem>
#include <vector>

struct record {
	int wins;
	int loss;
};

using std::string;

class Team {
public:
	Team();
	Team(string id,
		string name,
		string shortName,
		string abbreviation,
		string color,
		string alternateColor,
		string logoUrl
	);
	~Team();
	void setRecordFromString(string str_totalRecord, 
		string str_homeRecord, string str_awayRecord);
	void readBitmapToMemory(std::filesystem::path filePath);
	string getId();

private:
	string id;
	string name;
	string shortName;
	string abbreviation;
	string color;
	string alternateColor;
	record totalRecord{};
	record homeRecord{};
	record awayRecord{};
	string logoUrl;
	string fullResLogoFilename;
	std::future<int> future;
	std::vector<char>* raw_bitmap;
};