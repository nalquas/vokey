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

#define VOKEY_CONFIG_VERSION 2

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Common.h"

using json = nlohmann::json;

inline json config;
inline std::string config_location = std::string(getenv("HOME")) + "/.config/vokey";

// Header
void ensure_config_exists(void);
std::string get_default_profile_filename(void);
void load_config(void);
void reset_config_to_default(void);
void save_config(void);

// Implementation

inline void ensure_config_exists() {
	std::string config_path = config_location + "/config.json";

	// Ensure vokey directory exists
	if (stat(config_location.c_str(), &st) == -1) {
		mkdir(config_location.c_str(), 0744);
	}
	
	// Ensure config exists
	if (stat(config_path.c_str(), &st) == -1) {
		// No config exists, create default config...
		std::cout << "First-time setup, creating default config at \"" << config_path << "\"...\n";

		reset_config_to_default();

		save_config();
	}
}

inline std::string get_default_profile_filename() {
	return std::string(config["default_profile"]);
}

inline void load_config() {
	std::string temp = "";

	std::ifstream ifs(config_location + "/config.json");
	temp.assign( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()));
	ifs.close();

	config = json::parse(temp);
}

inline void reset_config_to_default() {
	config = {
		{"version", VOKEY_CONFIG_VERSION},
		{"default_profile", "default_profile.json"},
		{"use_pulseaudio_flush", false},
		{"listening_on_startup", true}
	};
}

inline void save_config() {
	config["version"] = VOKEY_CONFIG_VERSION;
	
	std::ofstream ofs(config_location + "/config.json");
	ofs << config.dump(1, '\t');
	ofs.close();
}

#endif
