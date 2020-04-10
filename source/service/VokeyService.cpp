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
#include "../common/config.h"
#include "../common/communication.h"
#include "EventRecognizer.h"

using namespace std;

EventRecognizer *er;

void store_pid() {
	ofstream of;
	of.open(VOKEY_TMP_PID);
	of << getpid();
	of.close();
}

void store_listening() {
	ofstream of;
	of.open(VOKEY_TMP_LISTENING);
	of << er->get_listening();
	of.close();
}

void store_current_profile() {
	ofstream of;
	of.open(VOKEY_TMP_PROFILE);
	of << er->get_profile();
	of.close();
}

bool already_running() {
	ensure_tmp_exists();
	
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

void handle_signal(int signum) {
	if (signum == SIGUSR1) {
		// SIGUSR1 is used to tell the service to reload the profile
		// TODO: store and get which profile to load
		er->request_reload();
	}
	else if (signum == SIGUSR2) {
		// SIGUSR2 is used to tell the service to change listening status
		string temp = "";
		bool listening = true;
		ifstream ifs;
		ifs.open(VOKEY_TMP_LISTENING);
		ifs >> temp;
		ifs.close();

		try
		{
			listening = (stoi(temp) > 0);
		}
		catch(const exception& e)
		{
			cerr << e.what() << '\n';
		}

		er->set_listening(listening);
	}
}

int main(int argc, char const *argv[]) {
	// Make sure we are the only instance
	if (already_running()) {
		cout << "There already is an instance of vokey_service running. Exiting...\n";
		return 0;
	}

	// Prepare for service
	ensure_config_exists();
	load_config();
	er = new EventRecognizer(get_default_profile_path(), config["listening_on_startup"]);
	store_listening();
	store_current_profile();
	clean_log();

	// Activate interrupt handlers
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	
	// Run the service
	return er->run();
}
