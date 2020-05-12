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

#define VOKEY_PROFILE_VERSION 3

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
inline unsigned long unique_identifier_iterator = 0;

// Header

void ensure_default_profile_exists(void);
json ensure_profile_compatibility(json profile);
std::string generate_unique_identifier(void);
json generate_default_profile(void);
json generate_default_event(void);
json generate_default_action(void);
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

// Ensure profile compatibility by going through every iteration of Vokey's profile standard
inline json ensure_profile_compatibility(json profile) {
	if (profile["version"] <= 2) {
		// Version 3 introduced unique identifier ids
		profile["id"] = generate_unique_identifier();
		for (int i = 0; i < profile["events"].size(); i++) {
			profile["events"][i]["id"] = generate_unique_identifier();
			for (int j = 0; j < profile["events"][i]["actions"].size(); j++) {
				profile["events"][i]["actions"][j]["id"] = generate_unique_identifier();
			}
		}
		profile["version"] = 3;
	}

	// Return the updated profile
	return profile;
}

// Generate a unique identifier for differentiating between profiles/events/actions
inline std::string generate_unique_identifier(void) {
	return std::to_string((unsigned long)time(NULL)) + "-" + std::to_string(unique_identifier_iterator++);
}

// Returns a json object containing default profile contents
inline json generate_default_profile() {
	json profile = {
		{"version", VOKEY_PROFILE_VERSION},
		{"id", generate_unique_identifier()},
		{"name", "Default Profile"},
		{"description", "This is the default profile"},
		{"events", {
			generate_default_event()
		}}
	};

	return profile;
}

// Returns a json object containing default event contents
inline json generate_default_event() {
	json event = {
		{"id", generate_unique_identifier()},
		{"title", "Default Event"},
		{"description", "This is an automatically-generated default event."},
		{"commands", {"example", "test"}},
		{"actions", {
			generate_default_action()
		}}
	};
	return event;
}

// Returns a json object containing default action contents
inline json generate_default_action() {
	json action = {
		{"id", generate_unique_identifier()},
		{"title", "Default Action"},
		{"type", "speak"},
		{"parameter", "You have triggered the default action."}
	};
	return action;
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