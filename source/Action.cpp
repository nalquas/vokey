#include "Action.h"

Action::Action() {}

void Action::execute(const char* command) {
	std::cout << "Executing: " << command << "\n";
	system(command);
}

void Action::print(std::string text) {
	std::cout << text << "\n";
}
