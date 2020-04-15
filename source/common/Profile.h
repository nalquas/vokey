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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sys/stat.h>
#include <vector>

#include "Common.h"

using json = nlohmann::json;

inline std::string profile_location = "";
inline std::vector<std::string> profile_list;

// Header

void ensure_default_profile_exists(void);
json generate_default_profile(void);
json load_profile(std::string profile_filename);
void refresh_profile_list(void);
void save_profile(json profile, std::string profile_filename);
void set_profile_directory(std::string path);

// Implementation

// Make sure the profile directory and the default_profile file exist
inline void ensure_default_profile_exists(void) {
	std::string profile_path = profile_location + "/default_profile.json";

	// Ensure default profile exists
	if (stat(profile_path.c_str(), &st) == -1) {
		// No default profile exists, create default profile...
		std::cout << "First-time setup, creating default profile at \"" << profile_path << "\"...\n";

		save_profile(generate_default_profile(), "default_profile.json");
	}
}

// Returns a json object containing default profile contents
inline json generate_default_profile() {
	json profile = {
		{"name", "Default Profile"},
		{"description", "This is the default profile"},
		{"events", {
			{
				{"title", "Example Event"},
				{"description", "This is an example event."},
				{"commands", {"example", "test"}},
				{"actions", {
					{
						{"type", "speak"},
						{"text", "You have triggered the example event. Congratulations, it is working!"}
					}
				}}
			}
		}}
	};

	return profile;
}

// Returns a json object of the profile from the specified filename
inline json load_profile(std::string profile_filename) {
	std::string temp = "";

	std::ifstream f((profile_location + "/" + profile_filename).c_str());
	temp.assign( (std::istreambuf_iterator<char>(f) ), (std::istreambuf_iterator<char>()));
	f.close();

	return json::parse(temp);
}

// Refresh profile_list from disk
inline void refresh_profile_list() {
	// Clear profile list
	profile_list.clear();

	// Iterate through all files in the profile directory
	for (auto& p: std::filesystem::directory_iterator(profile_location.c_str())) {
		// Add each filename to profile_list
		if (p.path().has_filename())
			profile_list.push_back(p.path().filename().string());
	}
}

// Save the given json object under the specified filename inside of the profile directory
inline void save_profile(json profile, std::string profile_filename) {
	std::ofstream of;
	of.open((profile_location + "/" + profile_filename).c_str());
	of << profile.dump(1, '\t');
	of.close();
}

// Set the profile directory to the specified path.
// Should be called as early as possible (before any other functions of Profile.h are called)
inline void set_profile_directory(std::string path) {
	profile_location = path;

	// Ensure profile directory exists
	if (stat(profile_location.c_str(), &st) == -1) {
		mkdir(profile_location.c_str(), 0744);
	}
}

#endif