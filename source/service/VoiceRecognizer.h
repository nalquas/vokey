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
	// Constructor, Destructor
	VoiceRecognizer(int type);
	~VoiceRecognizer(void);
	
	// Interfacing methods
	int process_file(const char* file_path);
	std::string get_text(void);
	int get_score(void);
private:
	// Results:
	std::string _text = "";
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
