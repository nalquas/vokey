#include "VoiceRecognizer.h"

// Constructor
VoiceRecognizer::VoiceRecognizer(int type) {
	switch (type) {
	case raw:
		_config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", MODELDIR "/en-us/en-us",
		"-lm", MODELDIR "/en-us/en-us.lm.bin",
		"-dict", MODELDIR "/en-us/cmudict-en-us.dict",
		NULL);
		if (_config == NULL) {
			fprintf(stderr, "Failed to create config object, check log.\n");
		}
		break;
	default:
		break;
	}
}

// Destructor
VoiceRecognizer::~VoiceRecognizer() {
	if (_fh != NULL) fclose(_fh);
	if (_ps != NULL) ps_free(_ps);
	if (_config != NULL) cmd_ln_free_r(_config);
}

int VoiceRecognizer::process_file(const char* file_path) {
	_ps = ps_init(_config);
	if (_ps == NULL) {
		fprintf(stderr, "Failed to create recognizer, check log.\n");
		return -1;
	}
	
	_fh = fopen(file_path, "rb");
	if (_fh == NULL) {
		fprintf(stderr, "Unable to open input file\n");
		return -1;
	}
	
	_rv = ps_start_utt(_ps);
	
	while (!feof(_fh)) {
		size_t nsamp;
		nsamp = fread(_buf, 2, 512, _fh);
		ps_process_raw(_ps, _buf, nsamp, FALSE, FALSE);
	}
	
	_rv = ps_end_utt(_ps);
	_hyp = ps_get_hyp(_ps, &_score);
	_text = _hyp;
	
	fclose(_fh);
	_fh = NULL;
	ps_free(_ps);
	_ps = NULL;
	return 0;
}

string VoiceRecognizer::get_text() {
	return _text;
}

int VoiceRecognizer::get_score() {
	return _score;
}
