// The main source file of Vokey. This is where it all starts. Literally.

// Libraries
#include <iostream>

// Classes
#include "VoiceRecognizer.h"

int main(int argc, char const *argv[])
{
	std::cout << "Testing VoiceRecognizer...\n";

	VoiceRecognizer vr(raw);

	if (vr.process_file("goforward.raw") != 0) return -1;

	std::cout << "Text: " << vr.get_text() << "\n";

	return 0;
}
