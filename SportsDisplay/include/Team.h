#pragma once
#include <string>
#include <iostream>

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
	void setRecordFromString(string str_totalRecord, 
		string str_homeRecord, string str_awayRecord);

private:
	string id;
	string name;
	string shortName;
	string abbreviation;
	string color;
	string alternateColor;
	record totalRecord;
	record homeRecord;
	record awayRecord;
	string logoUrl;
	string fullResLogoFilename;
};