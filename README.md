# vokey
Vokey is a voice-based hotkey program.
The goal is to have an application that, when configured voice commands are given, executes macros.
This would be a cool thing to have in games, thus possible actions should not just include executing scripts or playing sounds, but sending inputs to the game windows through X.

## Compilation
You need to have the following libraries installed to compile Vokey:

- Pocketsphinx
- Sphinxbase
- Qt5

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
