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
// Common functionality related to configuration

#ifndef Config_h
#define Config_h

#define VOKEY_CONFIG_VERSION 1

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Common.h"

using namespace std;
using json = nlohmann::json;

inline json config;
inline string config_location = string(getenv("HOME")) + "/.config/vokey";

// Header
void ensure_config_exists(void);
string get_default_profile_path(void);
void load_config(void);
void reset_config_to_default(void);
void save_config(void);

// Implementation

inline void ensure_config_exists() {
	string config_path = config_location + "/config.json";
	string profile_location = config_location + "/profiles";
	string profile_path = profile_location + "/default_profile.json";

	// Ensure vokey directory exists
	if (stat(config_location.c_str(), &st) == -1) {
		mkdir(config_location.c_str(), 0744);
	}

	// Ensure profile directory exists
	if (stat(profile_location.c_str(), &st) == -1) {
		mkdir(profile_location.c_str(), 0744);
	}

	// Ensure config exists
	if (stat(config_path.c_str(), &st) == -1) {
		// No config exists, create default config...
		cout << "First-time setup, creating default config at \"" << config_path << "\"...\n";

		reset_config_to_default();

		save_config();
	}

	// Ensure default profile exists
	if (stat(profile_path.c_str(), &st) == -1) {
		// No default profile exists, create default profile...
		cout << "First-time setup, creating default profile at \"" << profile_path << "\"...\n";

		json profile = {
			{"name", "Default Profile"},
			{"description", "This is the default profile"},
			{"events", {
				{
					{"title", "Example Event"},
					{"Description", "This is an example event."},
					{"commands", {"example", "test"}},
					{"actions", {
						{
							{"type", "tts"},
							{"text", "You have triggered the example event. Congratulations, it is working!"}
						}
					}}
				}
			}}
		};

		ofstream of;
		of.open(profile_path);
		of << profile.dump(1, '\t');
		of.close();
	}
}

inline string get_default_profile_path() {
	return config_location + "/profiles/" + string(config["default_profile"]);
}

inline void load_config() {
	string temp = "";

	ifstream ifs(config_location + "/config.json");
	temp.assign( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	ifs.close();

	config = json::parse(temp);
}

inline void reset_config_to_default() {
	config = {
		{"version", VOKEY_CONFIG_VERSION},
		{"default_profile", "default_profile.json"},
		{"listening_on_startup", true}
	};
}

inline void save_config() {
	config["version"] = VOKEY_CONFIG_VERSION;
	
	ofstream ofs(config_location + "/config.json");
	ofs << config.dump(1, '\t');
	ofs.close();
}

#endif
