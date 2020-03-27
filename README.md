# vokey
Vokey is a voice-based hotkey program.
The goal is to have an application that, when configured voice commands are given, executes macros.
This would be a cool thing to have in games, thus possible actions should not just include executing scripts or playing sounds, but sending inputs to the game windows through X.

## Compilation
You need to have the following libraries installed to compile Vokey:

- Pocketsphinx
- Sphinxbase
- Qt5

To compile, simply execute:

```
make
```

## Installation
Vokey currently requires several terminal commands to be available for its action system.
Namely, these are `aplay` and `festival`.

## Usage
Run:

```
./vokey.out
```
