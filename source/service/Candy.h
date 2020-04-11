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
// Candy.h only contains visual treats, like the ASCII-logo above.

#ifndef Candy_h
#define Candy_h

#include "../common/Communication.h"

inline void print_ascii_vokey() {
	print_log(
	".          .  __________   __    _____  ___________  __      __\n"
	"|\\        /| /   ____   \\ |  |  /  __/ /   _______/ |  |    |  |\n"
	"| \\      / | |  /    \\  | |  \\_/  /    |  |______   |  |    |  |\n"
	"\\  \\    /  / | /      \\ | |      /     |   ______|  \\  \\____/  |\n"
	" \\  \\  /  /  | \\      / | |   _  \\     |  |          \\______   |\n"
	"  \\  \\/  /   |  \\____/  | |  / \\  \\__  |  |_______    ______\\  |\n"
	"   \\____/    \\__________/ |__|  \\____\\ \\__________\\  /_________/\n"
	);
}

#endif