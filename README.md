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

## Compilation
You need to have the following libraries installed to your system to compile Vokey:

- [JSON for Modern C++](https://github.com/nlohmann/json)
- [Pocketsphinx](https://github.com/cmusphinx/pocketsphinx) and [Sphinxbase](https://github.com/cmusphinx/sphinxbase)
- [Pulseaudio](https://www.freedesktop.org/wiki/Software/PulseAudio/)
- [Qt5](https://www.qt.io/download-open-source)

To compile the project, simply execute:

```
make
```

## Installation
Vokey currently requires several terminal commands to be available for its action system.
Namely, these are `aplay` and `festival`.

```
# On Ubuntu-based systems, install the dependencies using:
sudo apt install alsa-utils festival

# On Arch-based systems, install the dependencies using:
sudo pacman -Syu alsa-utils festival festival-english
```

To install Vokey on your system, simply execute:

```
bash ./linux_install.sh
```

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
