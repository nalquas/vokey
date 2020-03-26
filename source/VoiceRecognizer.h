#ifndef VoiceRecognizer_h
#define VoiceRecognizer_h

#include <pocketsphinx.h>
#include <iostream>

// Types of voice recognition procedures
enum recognition_types {
	raw
};

class VoiceRecognizer {
public:
	VoiceRecognizer(int type);
	~VoiceRecognizer(void);
	void start(void);
private:
	ps_decoder_t *ps;
	cmd_ln_t *config;
	FILE *fh;
	char const *hyp, *uttid;
	int16 buf[512];
	int rv;
	int32 score;
};

#endif
