# Usage:
# make       - Build Vokey
# make clean - Remove compiled binaries

.PHONY: all clean

COMPILER = g++
LINKERFLAGS_SERVICE = `pkg-config --cflags --libs pocketsphinx sphinxbase`
LINKERFLAGS_MANAGER = `pkg-config --cflags --libs Qt5Widgets`
POCKETSPHINX_MODELDIR = `pkg-config --variable=modeldir pocketsphinx`

all: vokey_service.out vokey_manager.out

vokey_service.out: source/service/VokeyService.cpp source/service/Action.cpp source/service/EventRecognizer.cpp source/service/VoiceRecognizer.cpp
	$(COMPILER) $(LINKERFLAGS_SERVICE) -DMODELDIR=\"$(POCKETSPHINX_MODELDIR)\" source/service/VokeyService.cpp  source/service/Action.cpp source/service/EventRecognizer.cpp source/service/VoiceRecognizer.cpp -o vokey_service.out
	chmod +x vokey_service.out

vokey_manager.out: source/manager/VokeyManager.cpp source/manager/ui/manager.h source/manager/ui/about.h
	$(COMPILER) $(LINKERFLAGS_MANAGER) -fPIC source/manager/VokeyManager.cpp -o vokey_manager.out
	chmod +x vokey_manager.out

source/manager/ui/manager.h: ui/manager.ui
	uic -o source/manager/ui/manager.h ui/manager.ui

source/manager/ui/about.h: ui/about.ui
	uic -o source/manager/ui/about.h ui/about.ui

clean:
	if test -f "vokey_service.out"; then rm vokey_service.out; fi
	if test -f "vokey_manager.out"; then rm vokey_manager.out; fi
	if test -f "source/manager/ui/manager.h"; then rm source/manager/ui/manager.h; fi
	if test -f "source/manager/ui/about.h"; then rm source/manager/ui/about.h; fi
