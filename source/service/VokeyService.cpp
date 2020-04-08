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
// The main source file of Vokey. This is where it all starts. Literally.

#define VOKEY_CONFIG_VERSION 1

// Libraries
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

// Classes
#include "EventRecognizer.h"

using namespace std;
using json = nlohmann::json;

EventRecognizer er;
string config_location;

void ensure_config_exists() {
	string config_path = config_location + "/config.json";
	string profile_location = config_location + "/profiles";
	string profile_path = profile_location + "/default_profile.json";
	struct stat st = {0};

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

		json cfg = {
			{"version", VOKEY_CONFIG_VERSION},
			{"default_profile", "default_profile.json"}
		};

		ofstream of;
		of.open(config_path);
		of << cfg.dump(1, '\t');
		of.close();
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

string get_default_profile_path() {
	string temp = "";

	ifstream f(config_location + "/config.json");
	temp.assign( (istreambuf_iterator<char>(f) ), (istreambuf_iterator<char>()));
	f.close();

	return config_location + "/profiles/" + string(json::parse(temp)["default_profile"]);
}

int main(int argc, char const *argv[]) {
	config_location = string(getenv("HOME")) + "/.config/vokey";
	
	ensure_config_exists();

	er.load_profile(get_default_profile_path());
	return er.run();
}
