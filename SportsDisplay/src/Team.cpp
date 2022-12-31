#include "Team.h"
#include "DataProcess.h"
#include "ImageProcessing.h"
#include <fstream>

std::future<int> future;


Team::Team() {}

Team::Team(
	string  _id, 
	string _name, 
	string _shortName, 
	string _abbreviation, 
	string _color, 
	string _alternateColor,  
	string _logoUrl) 
	: id(std::move(_id)), name(std::move(_name)), shortName(std::move(_shortName)), abbreviation(std::move(_abbreviation)),
	color(std::move(_color)), alternateColor(std::move(_alternateColor)), logoUrl(std::move(_logoUrl)) 
{
	// Download the image and process it
	fullResLogoFilename = id + ".png";
	future = std::async(std::launch::async, processImage, this, &logoUrl, &fullResLogoFilename, 30);
	//Causes Memory Leak!!!!
}
Team::~Team() { delete raw_bitmap; }

void Team::setRecordFromString(string str_totalRecord, 
	string str_homeRecord, string str_awayRecord) 
{
	// this funciton is wrong
	// it doesnt account for ties

	//auto f = id.c_str();
	//strtok(id.data(), "-");

	totalRecord = record{ atoi(&str_totalRecord[0]), atoi(&str_totalRecord[2]) };
	homeRecord = record{ atoi(&str_homeRecord[0]), atoi(&str_homeRecord[2]) };
	awayRecord = record{ atoi(&str_awayRecord[0]), atoi(&str_awayRecord[2]) };
}

string Team::getId() {
	return id;
}

void Team::readBitmapToMemory(std::filesystem::path filePath) {
	using namespace std;

	// open file stream in binary
	ifstream ifd(filePath.string(), ios::binary | ios::ate);

	//get the file size
	int size = ifd.tellg();

	// set file pointer to beginning
	ifd.seekg(0, ios::beg);

	// create a buffer
	// and resize it to the 
	// size of the file
	auto* buffer = new vector<char>;
	buffer->resize(size);

	// read file data into buffer
	ifd.read(buffer->data(), buffer->size());

	// checks to validate bitmap is
	// in standard Windows format
	if (buffer->data()[0] == (char)0x42 && buffer->data()[1] == (char)0x4d) {
		// remove file header
		buffer->erase(buffer->begin(), buffer->begin() + 54);
	}
	raw_bitmap = buffer;
}

std::vector<char>* Team::getBitmap() const
{
	return raw_bitmap;
}

string Team::getAbbrName()
{
	return abbreviation;
}