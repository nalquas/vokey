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

#include "EventRecognizer.h"

// Constructor
EventRecognizer::EventRecognizer(string profile_file_path, bool listening) {
	_listening = listening;
	_reload_requested = true;
	_reload_profile_file_path = profile_file_path;
}

// Destructor
EventRecognizer::~EventRecognizer() {}

int EventRecognizer::run() {
	for (;;) {
		// Handle reload requests
		if (_reload_requested) {
			print_log("\nReloading profile...\n");
			load_profile(_reload_profile_file_path);
			_reload_requested = false;
		}

		// Handle listening
		if (_listening) {
			// Voice processing
			if (_vr.process_microphone() != 0) return -1;

			// Text handling
			print_log("Text: " + _vr.get_text() + "\n");

			// Check for all possible events
			bool any_event_activated = false;
			for (int i = 0; i < _profile["events"].size() && !any_event_activated; i++) {
				// Check for all possible commands
				for (int j = 0; j < _profile["events"][i]["commands"].size(); j++) {
					if (strstr(_vr.get_text().c_str(), string(_profile["events"][i]["commands"][j]).c_str())) {
						// Text matched one of the commands of this event
						
						// Activate all specified actions
						for (int k = 0; k < _profile["events"][i]["actions"].size(); k++) {
							switch (get_action_type(string(_profile["events"][i]["actions"][k]["type"])))
							{
							case action_execute:
								_action.execute(string(_profile["events"][i]["actions"][k]["command"]).c_str());
								break;
							case action_print:
								_action.print(string(_profile["events"][i]["actions"][k]["text"]));
								break;
							case action_bell:
								_action.bell();
								break;
							case action_play_audio:
								_action.play_audio(string(_profile["events"][i]["actions"][k]["file_path"]).c_str());
								break;
							case action_speak:
								_action.speak(string(_profile["events"][i]["actions"][k]["text"]).c_str());
								break;
							default:
								print_log("Unhandled action type \"" + string(_profile["events"][i]["actions"][k]["type"]) + "\"\n");
								break;
							}
						}
						
						// Don't check for any other accepted commands of this event
						any_event_activated = true;
						break;
					}
				}
			}
		}
	}

	// Unreachable
	return 0;
}

int EventRecognizer::get_action_type(string s) {
	if (s == "execute") return action_execute;
	if (s == "print") return action_print;
	if (s == "bell") return action_bell;
	if (s == "play_audio") return action_play_audio;
	if (s == "tts") return action_speak;
	return -1;
}

void EventRecognizer::load_profile(string file_path) {
	load_profile(file_path.c_str());
}
void EventRecognizer::load_profile(const char* file_path) {
	string temp = "";

	ifstream f(file_path);
	temp.assign( (istreambuf_iterator<char>(f) ), (istreambuf_iterator<char>()));
	f.close();

	_profile = json::parse(temp);
}

void EventRecognizer::request_reload(string file_path) {
	_reload_profile_file_path = file_path;
	request_reload();
}
void EventRecognizer::request_reload() {
	_reload_requested = true;
	_vr.request_finish();
}

void EventRecognizer::set_listening(bool listen) {
	string temp = "";
	if (listen)	 {
		temp += "Starting";
	}
	else {
		_vr.request_finish();
		temp += "Stopping";
	}
	temp += " to listen...\n";
	print_log(temp);
	_listening = listen;
}

bool EventRecognizer::get_listening(void) {
	return _listening;
}

string EventRecognizer::get_profile(void) {
	return _reload_profile_file_path;
}