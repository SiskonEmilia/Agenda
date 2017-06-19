#include "myCoder.h"
using namespace std;

void myCoder() {
	ifstream fileToCode;
	ofstream fileToWrite;
	string toCode;

	fileToCode.open(Path::userPath, ios::binary);
	getline(fileToCode, toCode, (char)-1);
	fileToCode.close();
	for (auto &ch : toCode) {
		if (ch != 1 && ch != -128)
			ch = -ch;
	}
	fileToWrite.open(Path::userPath, ios::out || ios::binary);
	fileToWrite << toCode;
	fileToWrite.close(); // code user data file

	fileToCode.open(Path::meetingPath, ios::binary);
	getline(fileToCode, toCode, (char)-1);
	fileToCode.close();
	for (auto &ch : toCode) {
		if (ch != 1 && ch != -128)
			ch = -ch;
	}
	fileToWrite.open(Path::meetingPath, ios::out || ios::binary);
	fileToWrite << toCode;
	fileToWrite.close(); // code meeting data file
} // as the way to code is symmetric, coding and decoding functions are exactly the same