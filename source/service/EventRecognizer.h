#ifndef EventRecognizer_h
#define EventRecognizer_h

#include <cstring>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Action.h"
#include "VoiceRecognizer.h"

using namespace std;
using json = nlohmann::json;

class EventRecognizer {
public:
	// Constructor, Destructor
	EventRecognizer(void);
	~EventRecognizer(void);
	
	// Interfacing methods
	void load_profile(const char* file_path);
private:
	Action _action;
	json _profile;
};

#endif
