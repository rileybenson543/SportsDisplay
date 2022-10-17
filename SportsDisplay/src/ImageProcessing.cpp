#include <string>



using std::string, std::to_string;

int processImage(string inputfile, int newsize) {
	// Need PIL installed in python
	string systemcall = "python3 ConvertImages.py " + inputfile + " " + to_string(newsize);
	return system(systemcall.c_str());
}