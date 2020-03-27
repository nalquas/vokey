#ifndef Action_h
#define Action_h

#include <iostream>
#include <cstdlib>
#include <cstring>

class Action {
public:
	Action(void);
	void execute(const char* command);
	void print(std::string text);
	void bell(void);
	void play_audio(const char* file_path);
	void speak(const char* text);
};

#endif
