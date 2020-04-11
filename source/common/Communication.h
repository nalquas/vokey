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
// Common functionality related to communication between the manager and the service

#ifndef Communication_h
#define Communication_h

#define VOKEY_TMP "/tmp/vokey"
#define VOKEY_TMP_LISTENING "/tmp/vokey/listening.bool"
#define VOKEY_TMP_LOG "/tmp/vokey/service.log"
#define VOKEY_TMP_PID "/tmp/vokey/service.pid"
#define VOKEY_TMP_PROFILE "/tmp/vokey/profile.path"

#include <csignal>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Common.h"

using namespace std;

// Header

pid_t get_service_pid(void);
void ensure_tmp_exists(void);
void ensure_log_exists(void);
void clean_log(void);
void print_log(string text);

// Implementation

inline pid_t get_service_pid() {
	// Check if directory and the PID-file exist
	if (stat(VOKEY_TMP, &st) != -1 && stat(VOKEY_TMP_PID, &st) != -1) {
		string temp = "";
		pid_t pid = 0;
		
		ifstream ifs;
		ifs.open(VOKEY_TMP_PID);
		ifs >> temp;
		ifs.close();

		// Is the PID given in the file still running?
		try {
			pid = stoi(temp);
			if (0 == kill(pid, 0)) return pid;
		}
		catch(const exception& e) {
			cerr << e.what() << '\n';
			cout << "As the temporary PID file caused an error, we'll just assume there is no instance running...\n";
		}
	}

	// Process does not exist
	return -1;
}

inline void ensure_tmp_exists() {
	// Make sure directory exists
	if (stat(VOKEY_TMP, &st) == -1) {
		mkdir(VOKEY_TMP, 0744);
	}
}

inline void ensure_log_exists() {
	ensure_tmp_exists();

	// If there is no log or there is no instance of the service running, write a clean log into the file
	if (stat(VOKEY_TMP_LOG, &st) == -1 || get_service_pid() == -1) {
		clean_log();
	}
}

inline void clean_log() {
	ofstream of;
	of.open(VOKEY_TMP_LOG);
	of << "";
	of.close();
}

inline void print_log(string text) {
	cout << text;
	fstream f;
	f.open(VOKEY_TMP_LOG, std::fstream::app);
	f << text;
	f.close();
}

#endif
