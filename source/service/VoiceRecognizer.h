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
// VoiceRecognizer.h provides an interface to a voice recognition library, pocketsphinx.

#ifndef VoiceRecognizer_h
#define VoiceRecognizer_h

#include <iostream>
#include <pocketsphinx.h>

using namespace std;

// Types of voice recognition procedures
enum recognition_types {
	raw
};

class VoiceRecognizer {
public:
	// Constructor, Destructor
	VoiceRecognizer(int type);
	~VoiceRecognizer(void);
	
	// Interfacing methods
	int process_file(const char* file_path);
	string get_text(void);
	int get_score(void);
private:
	// Results:
	string _text = "";
	int _score = 0;
	
	// Decoder variables:
	ps_decoder_t *_ps = NULL;
	cmd_ln_t *_config = NULL;
	FILE *_fh = NULL;
	char const *_hyp = "";
	int16 _buf[512];
	int _rv = 0;
};

#endif
