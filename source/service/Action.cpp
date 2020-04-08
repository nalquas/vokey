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
// Action.h handles the execution of actions.

#include "Action.h"

Action::Action() {}

void Action::execute(const char* command) {
	cout << "\nExecuting: " << command << "\n";
	system(command);
}

void Action::print(string text) {
	cout << "\n" << text << "\n";
}

void Action::bell() {
	cout << '\a';
}

void Action::play_audio(const char* file_path) {
	// TODO: Use a proper library for sound playback (ALSA?)
	char buf[4096];
	strcpy(buf, "aplay ");
	strcat(buf, file_path);
	strcat(buf, " &");
	execute(buf);
}

void Action::speak(const char* text) {
	// TODO: Directly interface Festival or Flite; Flite would be less resource-intensive, but incompatible with ALSA...
	char buf[4096];
	strcpy(buf, "echo \"");
	strcat(buf, text);
	strcat(buf, "\" | festival --tts &");
	execute(buf);
}
