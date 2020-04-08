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

#include "VoiceRecognizer.h"

// Constructor
VoiceRecognizer::VoiceRecognizer() {
	// Pulseaudio
	_pulse_spec.format = PA_SAMPLE_S16LE;
	_pulse_spec.channels = 1;
	_pulse_spec.rate = 16000;
	_pulse = pa_simple_new(
		NULL,
		"Vokey",
		PA_STREAM_RECORD,
		NULL,
		"Vokey Voice Recorder",
		&_pulse_spec,
		NULL,
		NULL,
		NULL
	);

	// Pocketsphinx
	_config = cmd_ln_init(NULL, ps_args(), TRUE,
	"-hmm", MODELDIR "/en-us/en-us",
	"-lm", MODELDIR "/en-us/en-us.lm.bin",
	"-dict", MODELDIR "/en-us/cmudict-en-us.dict",
	"-logfn", "/dev/null",
	NULL);
	if (_config == NULL) {
		fprintf(stderr, "Failed to create config object, check log.\n");
	}
	_ps = ps_init(_config);
	if (_ps == NULL) {
		fprintf(stderr, "Failed to create recognizer, check log.\n");
	}
}

// Destructor
VoiceRecognizer::~VoiceRecognizer() {
	if (_pulse != NULL) pa_simple_free(_pulse);
	if (_ps != NULL) ps_free(_ps);
	if (_config != NULL) cmd_ln_free_r(_config);
}

int VoiceRecognizer::process_microphone() {
	_rv = ps_start_utt(_ps);
	
	cout << "\nStarting recording\n";

	_in_speech = FALSE;
	_utt_started = FALSE;
	while (!(!_in_speech && _utt_started)) {
		size_t nsamp = 2048 / sizeof(int16);
		pa_simple_read(_pulse, _buf, 2048, NULL);
		ps_process_raw(_ps, _buf, nsamp, FALSE, FALSE);

		_in_speech = ps_get_in_speech(_ps);
		if (_in_speech && !_utt_started) {
			_utt_started = TRUE;
		}
	}
	pa_simple_flush(_pulse, NULL);

	cout << "Finished recording\nLatency: " << pa_simple_get_latency(_pulse, NULL) << "\n";

	_rv = ps_end_utt(_ps);
	_hyp = ps_get_hyp(_ps, &_score);
	_text = _hyp;

	return 0;
}

string VoiceRecognizer::get_text() {
	return _text;
}

int VoiceRecognizer::get_score() {
	return _score;
}
