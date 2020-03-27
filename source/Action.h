#ifndef Action_h
#define Action_h

#include <iostream>
#include <cstdlib>

class Action {
public:
	Action(void);
	void execute(const char* command);
	void print(std::string text);
};

#endif
