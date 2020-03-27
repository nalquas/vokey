# Usage:
# make       - Build Vokey
# make clean - Remove compiled binaries

.PHONY: all clean

COMPILER = g++
LINKERFLAGS = `pkg-config --cflags --libs Qt5Widgets pocketsphinx sphinxbase`
POCKETSPHINX_MODELDIR = `pkg-config --variable=modeldir pocketsphinx`

all: vokey.out

vokey.out: ./source/Vokey.cpp ./source/VoiceRecognizer.cpp ./source/Action.cpp
	$(COMPILER) $(LINKERFLAGS) -DMODELDIR=\"$(POCKETSPHINX_MODELDIR)\" -fPIC ./source/Vokey.cpp ./source/VoiceRecognizer.cpp ./source/Action.cpp -o vokey.out
	chmod +x vokey.out

clean:
	rm vokey.out
