#pragma once
#include <string>
#include <future>
#include <filesystem>
#include <vector>


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
	void setRecords(string str_totalRecord, string str_homeRecord, string str_awayRecord);
	void readBitmapToMemory(const std::filesystem::path* filePath);
	[[nodiscard]] std::vector<char>* getBitmap() const;
	[[nodiscard]] string getAbbrName() const;
	string getRecord() const;
	[[nodiscard]] string getId() const;

private:
	string id;
	string name;
	string shortName;
	string abbreviation;
	string color;
	string alternateColor;
	string totalRecord;
	string homeRecord;
	string awayRecord;
	string logoUrl;
	string fullResLogoFilename;
	std::future<int> future;
	std::vector<char>* raw_bitmap{};
};