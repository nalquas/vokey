// The main source file of Vokey. This is where it all starts. Literally.

// Libraries
#include <iostream>

// Classes
#include "Action.h"
#include "VoiceRecognizer.h"

Action action;

int main(int argc, char const *argv[])
{
	// Test: VoiceRecognizer
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
	
	return 0;
}
