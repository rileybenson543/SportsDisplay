#include <string>
#include "DataProcess.h"
#include <filesystem>


using std::string, std::to_string;

int processImage(string* url, string* filename, int newsize_px) {
	// Need PIL installed in python

	//python3 if ubuntu
	std::filesystem::path fullpath = std::filesystem::current_path() / "fullres" / *filename;
	int result = downloadImageCurl(url, &fullpath);
	string systemcall = "python ConvertImages.py " + fullpath.string() + " " + to_string(newsize_px);
	return system(systemcall.c_str());
}