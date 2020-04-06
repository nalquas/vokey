#ifndef Action_h
#define Action_h

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

class Action {
public:
	Action(void);
	void execute(const char* command);
	void print(string text);
	void bell(void);
	void play_audio(const char* file_path);
	void speak(const char* text);
};

#endif
