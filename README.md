# Vokey
```
.          .  __________   __    _____  ___________  __      __
|\        /| /   ____   \ |  |  /  __/ /   _______/ |  |    |  |
| \      / | |  /    \  | |  \_/  /    |  |______   |  |    |  |
\  \    /  / | /      \ | |      /     |   ______|  \  \____/  |
 \  \  /  /  | \      / | |   _  \     |  |          \______   |
  \  \/  /   |  \____/  | |  / \  \__  |  |_______    ______\  |
   \____/    \__________/ |__|  \____\ \__________\  /_________/
```
Vokey is a voice-based hotkey application.

The goal is to have an easily manageable service executing macros when voice commands are spoken.
The planned use case is gaming - Imagine flying a spaceship and just being able to command your vessel by speech.

Vokey is intended for use on Linux systems with Pulseaudio.

## Usage
Vokey is split into two sub-programs.

The main program is a background service reacting to your voice in a manner described by the selected profile:

```
vokey_service
```

The second program is a GUI-based manager used to configure the service.
You *could* use the service without this if you prefer editing .json files manually.

```
vokey_manager
```

## Installation
### AUR
If you're on Arch-based systems, you can get Vokey from the AUR:
https://aur.archlinux.org/packages/vokey-git/

## Compilation
You must have the following libraries installed to your system to compile Vokey yourself:

- [JSON for Modern C++](https://github.com/nlohmann/json)
- [Pocketsphinx](https://github.com/cmusphinx/pocketsphinx) and [Sphinxbase](https://github.com/cmusphinx/sphinxbase)
- [Pulseaudio](https://www.freedesktop.org/wiki/Software/PulseAudio/)
- [Qt5](https://www.qt.io/download-open-source)

```
# On Ubuntu (20.04) systems, install the necessary dev packages and dependencies using:
sudo apt install build-essential qt5-default libpulse-dev pocketsphinx pocketsphinx-en-us libsphinxbase3 libpocketsphinx-dev alsa-utils festival nlohmann-json-dev

# On Arch-based systems, install the necessary dev packages and dependencies using:
sudo pacman -Syu base-devel qt5-base pulseaudio alsa-utils festival festival-english nlohmann-json
# Pocketsphinx is not in the Arch repositories, you'll have to get it from the AUR:
# https://aur.archlinux.org/packages/pocketsphinx
# https://aur.archlinux.org/packages/sphinxbase/
```

To compile the project, follow these steps:

```
mkdir build
cd build
cmake ..

# If you want to install the project without a package manager:
sudo make install

# If you're a package maintainer and want to "install" in a packaging-friendly way:
make install DESTDIR=<Your package directory here>
```
