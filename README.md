# Vokey
```
.          .  __________   __    _____  ___________  __      __
|\        /| /   ____   \ |  |  /  __/ /   _______/ |  |    |  |
| \      / | |  /    \  | |  \_/  /    |  |______   |  |    |  |
\  \    /  / | /      \ | |      /     |   ______|  \  \____/  |
 \  \  /  /  | \      / | |   _  \     |  |          \______   |
  \  \/  /   |  \____/  | |  / \  \__  |  |_______    ______\  |
   \____/    \__________/ |__|   \___\ \__________\  /_________/
```
Vokey is a voice-based hotkey application.

The goal is to have an easily manageable application executing macros when voice commands are given.
The planned use case is gaming - Imagine flying a spaceship and just being able to command your vessel by speech.

## Compilation
You need to have the following libraries installed to your system to compile Vokey:

- [JSON for Modern C++](https://github.com/nlohmann/json)
- [Pocketsphinx](https://github.com/cmusphinx/pocketsphinx) and [Sphinxbase](https://github.com/cmusphinx/sphinxbase)
- [Qt5](https://www.qt.io/download-open-source)

To compile the project, simply execute:

```
make
```

## Installation
Vokey currently requires several terminal commands to be available for its action system.
Namely, these are `aplay` and `festival`.

## Usage
Vokey is split into two sub-programs.

The main program is a service reacting to your voice and acting according to a selected profile, executed using:

```
./vokey_service.out
```

The second program is a GUI-based manager used to configure the service:

```
./vokey_manager.out
```
