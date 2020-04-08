// .          .  __________   __    _____  ___________  __      __
// |\        /| /   ____   \ |  |  /  __/ /   _______/ |  |    |  |
// | \      / | |  /    \  | |  \_/  /    |  |______   |  |    |  |
// \  \    /  / | /      \ | |      /     |   ______|  \  \____/  |
//  \  \  /  /  | \      / | |   _  \     |  |          \______   |
//   \  \/  /   |  \____/  | |  / \  \__  |  |_______    ______\  |
//    \____/    \__________/ |__|   \___\ \__________\  /_________/
//
// A voice-based hotkey application
// Copyright (C) 2020  Nalquas
//
// Licensed under the GNU GENERAL PUBLIC LICENSE Version 3.
// Find the complete license in the LICENSE-file located in the project root.
//
// File description:
// EventRecognizer.h provides macro functionality.
// Essentially, it waits for the VoiceRecognizer to record a new voice command and then activates the corresponding action.

#include "EventRecognizer.h"

// Constructor
EventRecognizer::EventRecognizer() {
	
}

// Destructor
EventRecognizer::~EventRecognizer() {
	
}

int EventRecognizer::run() {
	for (;;) {
		// Voice processing
		if (_vr.process_microphone() != 0) return -1;

		// Text handling
		cout << "Text: " << _vr.get_text() << "\nScore: " << _vr.get_score() << "\n";

		// This is a test event-like thing:
		if (strstr(_vr.get_text().c_str(), "test")) {
			cout << "\nTesting Action...\n";
			_action.print("Hello world, this is a print action.");
			_action.execute("ls");
			_action.bell();
			_action.play_audio("/usr/share/sounds/alsa/Front_Center.wav");
			_action.speak("Hello There! I am Vokey, a voice-based hotkey application.");
		}

		// This is a test quit event
		if (strstr(_vr.get_text().c_str(), "quit") || strstr(_vr.get_text().c_str(), "exit")) {
			cout << "\nQuitting...\n";
			return 0;
		}
	}

	// Unreachable
	return 0;
}

void EventRecognizer::load_profile(const char* file_path) {
	string temp = "";

	ifstream f(file_path);
	temp.assign( (istreambuf_iterator<char>(f) ), (istreambuf_iterator<char>()));
	f.close();

	_profile = json::parse(temp);
}