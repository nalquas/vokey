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
// VoiceRecognizer.h provides an interface to a voice recognition library, pocketsphinx.

#ifndef VoiceRecognizer_h
#define VoiceRecognizer_h

#include <iostream>
#include <pocketsphinx.h>
#include <pulse/simple.h>

#include "../common/Config.h"
#include "../common/Communication.h"

class VoiceRecognizer {
public:
	// Constructor, Destructor
	VoiceRecognizer(void);
	~VoiceRecognizer(void);
	
	// Interfacing methods
	bool wait_for_keyword();
	int process_microphone(bool process);
	std::string get_text(void);
	int get_score(void);
	void request_finish(void);
	void reload(void);
private:
	bool _finish_requested;
	// Results:
	std::string _text = "";
	int _score = 0;
	
	// Decoder variables:
	ps_decoder_t *_ps = NULL;
	cmd_ln_t *_ps_config = NULL;
	char const *_hyp = "";
	int16 _buf[2048];
	int _rv = 0;
	uint8_t _utt_started=FALSE, _in_speech=FALSE;

	// Pulseaudio:
	pa_simple *_pulse = NULL;
	pa_sample_spec _pulse_spec;
};

#endif
