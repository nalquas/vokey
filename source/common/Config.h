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

#define VOKEY_CONFIG_VERSION 3

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
json ensure_config_compatibility(json conf);
std::string get_default_profile_filename(void);
void load_config(void);
void reset_config_to_default(void);
void save_config(void);

// Implementation

// Make sure the config directory and the config file exist
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

// Ensure config compatibility by going through every iteration of Vokey's config standard
inline json ensure_config_compatibility(json conf) {
	if (conf["version"] <= 2) {
		// Version 3 introduced activation keyword
		conf["keyword"] = "sphinx";
		conf["use_keyword"] = true;
		conf["version"] = 3;
	}

	// Return the updated config
	return conf;
}

inline std::string get_default_profile_filename() {
	return std::string(config["default_profile"]);
}

// Load config from disk
inline void load_config() {
	std::string temp = "";

	std::ifstream ifs(config_location + "/config.json");
	temp.assign( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()));
	ifs.close();

	config = ensure_config_compatibility(json::parse(temp));
}

// Replace config with default values
// (does not write to disk)
inline void reset_config_to_default() {
	config = {
		{"version", VOKEY_CONFIG_VERSION},
		{"default_profile", "default_profile.json"},
		{"use_pulseaudio_flush", false},
		{"listening_on_startup", true},
		{"keyword", "sphinx"},
		{"use_keyword", true}
	};
}

// Write config to disk
inline void save_config() {
	config["version"] = VOKEY_CONFIG_VERSION;
	
	std::ofstream ofs(config_location + "/config.json");
	ofs << config.dump(1, '\t');
	ofs.close();
}

#endif
