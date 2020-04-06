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
	// Test: VoiceRecognizer
	cout << "\nTesting VoiceRecognizer...\n";
	if (_vr.process_file("goforward.raw") != 0) return -1;
	cout << "Text: " << _vr.get_text() << "\n";

	// Test: Action
	cout << "\nTesting Action...\n";
	_action.print("Hello world, this is a print action.");
	_action.execute("ls");
	_action.bell();
	_action.play_audio("/usr/share/sounds/alsa/Front_Center.wav");
	_action.speak("Hello There! I am Vokey, a voice-based hotkey application.");

	return 0;
}

void EventRecognizer::load_profile(const char* file_path) {
	string temp = "";

	ifstream f(file_path);
	temp.assign( (istreambuf_iterator<char>(f) ), (istreambuf_iterator<char>()));
	f.close();

	_profile = json::parse(temp);
}