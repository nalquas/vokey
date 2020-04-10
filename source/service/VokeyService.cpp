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
// The main source file of vokey_service, responsible for handling startup procedures

#define VOKEY_CONFIG_VERSION 1

#define VOKEY_TMP "/tmp/vokey"
#define VOKEY_TMP_PID "/tmp/vokey/vokey.pid"
#define VOKEY_TMP_LOG "/tmp/vokey/vokey.log"

// Libraries
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Classes
#include "EventRecognizer.h"

using namespace std;
using json = nlohmann::json;

EventRecognizer er;
string config_location;
struct stat st = {0};

void ensure_config_exists() {
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

void store_pid() {
	ofstream of;
	of.open(VOKEY_TMP_PID);
	of << getpid();
	of.close();
}

void clean_tmp() {
	// Delete PID file
	unlink(VOKEY_TMP_PID);
}

bool already_running() {
	// Make sure directory exists
	if (stat(VOKEY_TMP, &st) == -1) {
		mkdir(VOKEY_TMP, 0744);
	}
	
	// Does file already exist?
	if (stat(VOKEY_TMP_PID, &st) != -1) {
		string temp = "";
		pid_t pid = 0;
		
		ifstream ifs;
		ifs.open(VOKEY_TMP_PID);
		ifs >> temp;
		ifs.close();

		// Is the PID given in the file still running?
		try
		{
			pid = stoi(temp);
			if (0 == kill(pid, 0)) return true;
		}
		catch(const exception& e)
		{
			cerr << e.what() << '\n';
			cout << "As the temporary PID file caused an error, we'll just assume there is no other instance running...\n";
		}
		
	}

	// Since there is no other instance running, store our PID
	store_pid();
	return false;
}

int main(int argc, char const *argv[]) {
	if (already_running()) {
		cout << "There already is an instance of vokey_service running. Exiting...\n";
		return 0;
	}

	config_location = string(getenv("HOME")) + "/.config/vokey";
	
	ensure_config_exists();

	er.load_profile(get_default_profile_path());
	
	int result = er.run();
	clean_tmp(); // TODO: What about when the application is killed, do we just leave the temporary files there?
	return result;
}
