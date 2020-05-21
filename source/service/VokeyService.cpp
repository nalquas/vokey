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

// Includes
#include <csignal>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Vokey includes
#include "../common/Config.h"
#include "../common/Communication.h"
#include "../common/Profile.h"
#include "Candy.h"
#include "EventRecognizer.h"

EventRecognizer *er;

void store_pid() {
	std::ofstream of;
	of.open(VOKEY_TMP_PID);
	of << getpid();
	of.close();
}

void store_listening() {
	std::ofstream of;
	of.open(VOKEY_TMP_LISTENING);
	of << er->get_listening();
	of.close();
}

void store_current_profile() {
	std::ofstream of;
	of.open(VOKEY_TMP_PROFILE);
	of << er->get_profile();
	of.close();
}

bool already_running() {
	ensure_tmp_exists();
	
	// Check if there already is an instance of vokey_service running
	if (get_service_pid() >= 0) return true;

	// Since there is no other instance running, store our PID
	store_pid();
	return false;
}

// Ignore a signal entirely
void ignore_signal(int signum) {}

void handle_signal(int signum) {
	if (signum == SIGUSR1) {
		// SIGUSR1 is used to tell the service to reload the profile and config
		
		// Reload global config
		print_log("[INFO] Reloading config...\n");
		ensure_config_exists();
		load_config();
		
		// Request EventRecognizer to load the specified profile
		std::string temp = "";
		std::ifstream ifs;
		ifs.open(VOKEY_TMP_PROFILE);
		temp.assign( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()));
		ifs.close();
		er->request_reload(temp);
	}
	else if (signum == SIGUSR2) {
		// SIGUSR2 is used to tell the service to change listening status
		std::string temp = "";
		bool listening = true;
		std::ifstream ifs;
		ifs.open(VOKEY_TMP_LISTENING);
		ifs >> temp;
		ifs.close();

		try
		{
			listening = (stoi(temp) > 0);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}

		er->set_listening(listening);
	}
}

int main(int argc, char const *argv[]) {
	// Ignore user-defined signals during startup so that we don't accidentally exit
	signal(SIGUSR1, ignore_signal);
	signal(SIGUSR2, ignore_signal);

	// Go through launch arguments
	for (int i = 0; i < argc; i++) {
		if (std::string(argv[i]) == "--version") {
			// Print version and exit
			std::cout << "vokey_service " << VOKEY_VERSION << " (built on " << BUILD_DATE << ")\n";
			return 0;
		}
	}

	// Make sure we are the only instance
	if (already_running()) {
		std::cout << "There already is an instance of vokey_service running. Exiting...\n";
		return 0;
	}

	// Prepare for service
	ensure_config_exists();
	load_config();
	set_profile_directory(config_location + "/profiles");
	ensure_default_profile_exists();
	er = new EventRecognizer(get_default_profile_filename(), config["listening_on_startup"]);
	store_listening();
	store_current_profile();
	clean_log();

	// Activate interrupt signal handlers
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	
	// Increase niceness
	nice(10);

	// Run the service
	print_ascii_vokey();
	return er->run();
}
