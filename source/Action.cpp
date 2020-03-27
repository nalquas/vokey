#include "Action.h"

Action::Action() {}

void Action::execute(const char* command) {
	std::cout << "\nExecuting: " << command << "\n";
	system(command);
}

void Action::print(std::string text) {
	std::cout << "\n" << text << "\n";
}

void Action::bell() {
	std::cout << '\a';
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
