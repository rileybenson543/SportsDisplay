#include "Team.h"
#include "DataProcess.h"
#include "ImageProcessing.h"





Team::Team() {}

Team::Team(
	string  _id, 
	string _name, 
	string _shortName, 
	string _abbreviation, 
	string _color, 
	string _alternateColor,  
	string _logoUrl) 
	: id(_id), name(_name), shortName(_shortName), abbreviation(_abbreviation),
	color(_color), alternateColor(_alternateColor), logoUrl(_logoUrl) 
{
	// Download the image and process it
	fullResLogoFilename = id + ".png";
	string path = downloadImage(&logoUrl, &fullResLogoFilename);
	processImage(path, 30);
}

void Team::setRecordFromString(string str_totalRecord, 
	string str_homeRecord, string str_awayRecord) 
{
	// this funciton is wrong
	// it doesnt account for ties
	totalRecord = record{ atoi(&str_totalRecord[0]), atoi(&str_totalRecord[2]) };
	homeRecord = record{ atoi(&str_homeRecord[0]), atoi(&str_homeRecord[2]) };
	awayRecord = record{ atoi(&str_awayRecord[0]), atoi(&str_awayRecord[2]) };
}