# Usage:
# make       - Build Vokey
# make clean - Remove compiled binaries

.PHONY: all clean

COMPILER = g++
C_STD = -std=c++17
LINKERFLAGS_SERVICE = `pkg-config --cflags --libs pocketsphinx sphinxbase` -lX11 -lXtst
LINKERFLAGS_MANAGER = `pkg-config --cflags --libs Qt5Widgets`
POCKETSPHINX_MODELDIR = `pkg-config --variable=modeldir pocketsphinx`
VOKEY_VERSION = `cat version`

all: vokey_service vokey_manager

vokey_service: source/service/VokeyService.cpp source/service/Action.cpp source/service/EventRecognizer.cpp source/service/VoiceRecognizer.cpp source/service/Candy.h source/common/Common.h source/common/Communication.h source/common/Config.h source/common/Profile.h version
	$(COMPILER) $(C_STD) $(LINKERFLAGS_SERVICE) -DMODELDIR=\"$(POCKETSPHINX_MODELDIR)\" -DVOKEY_VERSION=\"$(VOKEY_VERSION)\" source/service/VokeyService.cpp source/service/Action.cpp source/service/EventRecognizer.cpp source/service/VoiceRecognizer.cpp -o vokey_service
	chmod +x vokey_service

vokey_manager: source/manager/VokeyManager.cpp source/manager/ui/manager.h source/manager/ui/about.h source/common/Common.h source/common/Communication.h source/common/Config.h source/common/Profile.h version
	$(COMPILER) $(C_STD) $(LINKERFLAGS_MANAGER) -DVOKEY_VERSION=\"$(VOKEY_VERSION)\" -fPIC source/manager/VokeyManager.cpp -o vokey_manager
	chmod +x vokey_manager

source/manager/ui/manager.h: ui/manager.ui
	uic -o source/manager/ui/manager.h ui/manager.ui

source/manager/ui/about.h: ui/about.ui
	uic -o source/manager/ui/about.h ui/about.ui

clean:
	if test -f "vokey_service"; then rm vokey_service; fi
	if test -f "vokey_manager"; then rm vokey_manager; fi
	if test -f "source/manager/ui/manager.h"; then rm source/manager/ui/manager.h; fi
	if test -f "source/manager/ui/about.h"; then rm source/manager/ui/about.h; fi
