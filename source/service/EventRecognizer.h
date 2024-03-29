// .          .  __________   __    _____  ___________  __      __
// |\        /| /   ____   \ |  |  /  __/ /   _______/ |  |    |  |
// | \      / | |  /    \  | |  \_/  /    |  |______   |  |    |  |
// \  \    /  / | /      \ | |      /     |   ______|  \  \____/  |
//  \  \  /  /  | \      / | |   _  \     |  |          \______   |
//   \  \/  /   |  \____/  | |  / \  \__  |  |_______    ______\  |
//    \____/    \__________/ |__|  \____\ \__________\  /_________/
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
#include <sstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "../common/Communication.h"
#include "../common/Profile.h"
#include "Action.h"
#include "VoiceRecognizer.h"

#define VOKEY_SOUND_LISTENING_BEGIN "/usr/share/sounds/vokey/listening_begin.wav"
#define VOKEY_SOUND_LISTENING_END "/usr/share/sounds/vokey/listening_end.wav"

#define IGNORE_SCORE_TRESHOLD -10000

using json = nlohmann::json;

enum action_types {
	action_execute,
	action_print,
	action_bell,
	action_play_audio,
	action_speak,
	action_key
};

class EventRecognizer {
public:
	// Constructor, Destructor
	EventRecognizer(std::string profile_file_path, bool listening);
	
	// Interfacing methods
	void request_reload();
	void request_reload(std::string file_path);
	void set_listening(bool listen);
	bool get_listening();
	std::string get_profile();
	int run();
private:
	static int get_action_type(const std::string& s);
	bool _listening;
	Action _action;
	json _profile;
	VoiceRecognizer _vr;
	bool _reload_requested;
	std::string _reload_profile_file_path;
};

#endif
