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
// The main source file of Vokey. This is where it all starts. Literally.

// Libraries
#include <iostream>

// Classes
#include "EventRecognizer.h"

EventRecognizer er;

int main(int argc, char const *argv[])
{
	er.load_profile("default_profile.json");

	/*// Test: VoiceRecognizer
	std::cout << "\nTesting VoiceRecognizer...\n";
	VoiceRecognizer vr(raw);
	if (vr.process_file("goforward.raw") != 0) return -1;
	std::cout << "Text: " << vr.get_text() << "\n";

	// Test: Action
	std::cout << "\nTesting Action...\n";
	action.print("Hello world, this is a print action.");
	action.execute("ls");
	action.bell();
	action.play_audio("/usr/share/sounds/alsa/Front_Center.wav");
	action.speak("Hello There! I am Vokey, a voice-based hotkey application.");*/
	
	return 0;
}
