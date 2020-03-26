#include "VoiceRecognizer.h"

// Constructor
VoiceRecognizer::VoiceRecognizer(int type) {
	switch (type) {
	case raw:
		config = cmd_ln_init(NULL, ps_args(), TRUE,
			"-hmm", MODELDIR "/en-us/en-us",
			"-lm", MODELDIR "/en-us/en-us.lm.bin",
			"-dict", MODELDIR "/en-us/cmudict-en-us.dict",
			NULL);
		break;
	
	default:
		break;
	}
}

// Destructor
VoiceRecognizer::~VoiceRecognizer() {
	//if (fh != NULL) fclose(fh);
	if (ps != NULL) ps_free(ps);
	if (config != NULL) cmd_ln_free_r(config);
}

void VoiceRecognizer::start() {
	ps = ps_init(config);
}
