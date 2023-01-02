#include <string>
#include "DataProcess.h"
#include <Team.h>
#include <filesystem>

#if defined (_WIN32) || defined(_WIN64)
#define PYTHON_COMMAND "python"
#endif
#if defined (__linux__) || defined(__APPLE__)
#define PYTHON_COMMAND "python3"
#endif




using std::string, std::to_string;

int processImage(Team* self, string* url, string* filename, int newsize_px, string bg_color_hex) {
	// Need PIL installed in python

	//python3 if ubuntu
	std::filesystem::path fullpath = std::filesystem::current_path() / "fullres" / *filename;
	int result = downloadImageCurl(url, &fullpath);
	string systemcall = string(PYTHON_COMMAND) + " ConvertImages.py " + fullpath.string() + " " + to_string(newsize_px) + " #" + bg_color_hex;
	int py_result = system(systemcall.c_str());
	const std::filesystem::path bmp_file_path = std::filesystem::current_path() / "bmp" / (self->getId() + "_" + to_string(newsize_px) + "px.bmp");
	self->readBitmapToMemory(&bmp_file_path);
	return result;
}