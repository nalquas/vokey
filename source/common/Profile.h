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
// Contains functions to access profiles

#ifndef Profile_h
#define Profile_h

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sys/stat.h>

#include "Common.h"

using json = nlohmann::json;

inline std::string profile_location = "";

// Header

void ensure_default_profile_exists(void);
json generate_default_profile(void);
json load_profile(std::string profile_filename);
void save_profile(json profile, std::string profile_filename);
void set_profile_directory(std::string path);

// Implementation

inline void ensure_default_profile_exists(void) {
	std::string profile_path = profile_location + "/default_profile.json";

	// Ensure default profile exists
	if (stat(profile_path.c_str(), &st) == -1) {
		// No default profile exists, create default profile...
		std::cout << "First-time setup, creating default profile at \"" << profile_path << "\"...\n";

		save_profile(generate_default_profile(), "default_profile.json");
	}
}

inline json generate_default_profile() {
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

	return profile;
}

inline json load_profile(std::string profile_filename) {
	std::string temp = "";

	std::ifstream f((profile_location + "/" + profile_filename).c_str());
	temp.assign( (std::istreambuf_iterator<char>(f) ), (std::istreambuf_iterator<char>()));
	f.close();

	return json::parse(temp);
}

inline void save_profile(json profile, std::string profile_filename) {
	std::ofstream of;
	of.open((profile_location + "/" + profile_filename).c_str());
	of << profile.dump(1, '\t');
	of.close();
}

inline void set_profile_directory(std::string path) {
	profile_location = path;

	// Ensure profile directory exists
	if (stat(profile_location.c_str(), &st) == -1) {
		mkdir(profile_location.c_str(), 0744);
	}
}

#endif