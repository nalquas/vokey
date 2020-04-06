#include "EventRecognizer.h"

// Constructor
EventRecognizer::EventRecognizer() {
	
}

// Destructor
EventRecognizer::~EventRecognizer() {
	
}

void EventRecognizer::load_profile(const char* file_path) {
	string temp = "";

	ifstream f(file_path);
	temp.assign( (istreambuf_iterator<char>(f) ), (istreambuf_iterator<char>()));
	f.close();

	_profile = json::parse(temp);
}