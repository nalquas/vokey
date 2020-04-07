// .          .  __________   __    _____  ___________  __      __
// |\        /| /   ____   \ |  |  /  __/ /   _______/ |  |    |  |
// | \      / | |  /    \  | |  \_/  /    |  |______   |  |    |  |
// \  \    /  / | /      \ | |      /     |   ______|  \  \____/  |
//  \  \  /  /  | \      / | |   _  \     |  |          \______   |
//   \  \/  /   |  \____/  | |  / \  \__  |  |_______    ______\  |
//    \____/    \__________/ |__|   \___\ \__________\  /_________/
//
// A voice-based hotkey application
// Copyright (C) 2020  Nalquas
//
// Licensed under the GNU GENERAL PUBLIC LICENSE Version 3.
// Find the complete license in the LICENSE-file located in the project root.
//
// File description:
// EventRecognizer.h provides macro functionality.
// Essentially, it waits for the VoiceRecognizer to record a new voice command and then activates the corresponding action.

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
	int run(void);
private:
	Action _action;
	json _profile;
	VoiceRecognizer _vr;
};

#endif
