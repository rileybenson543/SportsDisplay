#include <string>



using std::string, std::to_string;

int processImage(const string inputfile, int newsize) {
	// Need PIL installed in python

	//python3 if ubuntu
	string systemcall = "python ConvertImages.py " + inputfile + " " + to_string(newsize);
	return system(systemcall.c_str());
}