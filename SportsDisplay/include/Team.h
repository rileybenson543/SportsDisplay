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
	void readBitmapToMemory(const std::filesystem::path* filePath);
	[[nodiscard]] std::vector<char>* getBitmap() const;
	[[nodiscard]] string getAbbrName() const;
	[[nodiscard]] string getId() const;
	//std::vector<char>* getBitmap();

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
	std::vector<char>* raw_bitmap{};
};