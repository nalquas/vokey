// .          .  __________   __    _____  ___________  __      __
// |\        /| /   ____   \ |  |  /  __/ /   _______/ |  |    |  |
// | \      / | |  /    \  | |  \_/  /    |  |______   |  |    |  |
// \  \    /  / | /      \ | |      /     |   ______|  \  \____/  |
//  \  \  /  /  | \      / | |   _  \     |  |          \______   |
//   \  \/  /   |  \____/  | |  / \  \__  |  |_______    ______\  |
//    \____/    \__________/ |__|  \____\ \__________\  /_________/
//
// A voice-based hotkey application
// Copyright (C) 2020  Nalquas
//
// Licensed under the GNU GENERAL PUBLIC LICENSE Version 3.
// Find the complete license in the LICENSE-file located in the project root.
//
// File description:
// EventRecognizer.h provides macro functionality.
// Essentially, it waits for the VoiceRecognizer to record a new voice command and then activates the corresponding action.

#include "EventRecognizer.h"

// Constructor
EventRecognizer::EventRecognizer(std::string profile_file_path, bool listening) {
	_listening = listening;
	_reload_requested = true;
	_reload_profile_file_path = profile_file_path;
}

int EventRecognizer::run() {
	for (;;) {
		// Handle reload requests
		if (_reload_requested) {
			// Load requested profile
			print_log("\n[INFO] Loading profile " + _reload_profile_file_path + "\n");
			_profile = ensure_profile_compatibility(load_profile(_reload_profile_file_path));

			// Create dictionary for the profile (for better accuracy)
			// Make a list of all words used in commands in this profile, plus the activation keyword from the global config
			std::vector<std::string> commands = {};
			for (int i = 0; i < _profile["events"].size(); i++) {
				for (int j = 0; j < _profile["events"][i]["commands"].size(); j++) {
					// Handle mutli-word commands by seperating them first
					std::istringstream command;
					command.str(_profile["events"][i]["commands"][j]);
					for (std::string word; std::getline(command, word, ' '); ) {
						if (word != "")
							commands.push_back(word);
					}
				}
			}
			if (config["keyword"] != "")
				commands.push_back(config["keyword"]);
			// Load default dictionary from disk
			std::string temp = "";
			std::ifstream f(MODELDIR "/en-us/cmudict-en-us.dict");
			temp.assign( (std::istreambuf_iterator<char>(f) ), (std::istreambuf_iterator<char>()));
			f.close();
			std::istringstream dictionary;
			dictionary.str(temp);
			// Iterate over each line of the dictionary
			std::string line;
			std::string new_dictionary = "";
			for (int i = 0; std::getline(dictionary, line, '\n'); i++) {
				// Check if the line contains any one of the commands
				for (int j = 0; j < commands.size(); j++) {
					const char *line_cstr = line.c_str();
					if (line_cstr==strstr(line_cstr, (commands[j]+" ").c_str()) || line_cstr==strstr(line_cstr, (commands[j]+"(").c_str())) {
						// Add the line to the new dictionary
						new_dictionary += line + "\n";
						// Don't check this line again
						break;
					}
				}
			}
			// Write new dictionary to disk
			std::ofstream ofs;
			ofs.open(VOKEY_TMP_DICT);
			ofs << new_dictionary;
			ofs.close();

			// Make VoiceRecognizer use the new dictionary
			_vr.reload();

			// Mark reload as finished
			_reload_requested = false;
		}

		// Handle listening
		if (_listening) {
			// Voice processing

			// Wait for keyword
			bool activated = true;
			if (config["use_keyword"]) {
				activated = _vr.wait_for_keyword();
				
				if (activated) {
					// TODO: Play listening begin sound
				}
			}

			// If keyword was recognized (or is not required), try to recognize a command
			if (activated) {
				if (_vr.process_microphone(true) != 0) return -1;
				
				if (config["use_keyword"]) {
					// TODO: Play listening end sound
				}

				// Text handling
				print_log("[INFO] Text: " + _vr.get_text() + " (Score: " + std::to_string(_vr.get_score()) + ")\n");

				// Only proceed if pocketsphinx is confident enough that the text is correct
				if (_vr.get_score() > IGNORE_SCORE_TRESHOLD) {
					// Check for all possible events
					bool any_event_activated = false;
					for (int i = 0; i < _profile["events"].size() && !any_event_activated; i++) {
						// Check for all possible commands
						for (int j = 0; j < _profile["events"][i]["commands"].size(); j++) {
							if (strstr(_vr.get_text().c_str(), std::string(_profile["events"][i]["commands"][j]).c_str())) {
								// Text matched one of the commands of this event
								print_log("[EVENT] Triggered " + std::string(_profile["events"][i]["title"]) + "\n");
								
								// Activate all specified actions
								for (int k = 0; k < _profile["events"][i]["actions"].size(); k++) {
									switch (get_action_type(std::string(_profile["events"][i]["actions"][k]["type"])))
									{
									case action_execute:
										_action.execute(std::string(_profile["events"][i]["actions"][k]["parameter"]).c_str());
										break;
									case action_print:
										_action.print(std::string(_profile["events"][i]["actions"][k]["parameter"]));
										break;
									case action_bell:
										_action.bell();
										break;
									case action_play_audio:
										_action.play_audio(std::string(_profile["events"][i]["actions"][k]["parameter"]).c_str());
										break;
									case action_speak:
										_action.speak(std::string(_profile["events"][i]["actions"][k]["parameter"]).c_str());
										break;
									case action_key:
										try
										{
											_action.press_key(stoi(std::string(_profile["events"][i]["actions"][k]["parameter"])));
										}
										catch(const std::exception& e)
										{
											print_log("Invalid parameter for action \"key\": " + std::string(_profile["events"][i]["actions"][k]["parameter"]) + "\n");
											std::cerr << e.what() << '\n';
										}
										break;
									default:
										print_log("[WARNING] Unhandled action type \"" + std::string(_profile["events"][i]["actions"][k]["type"]) + "\"\n");
										break;
									}
								}
								
								// Don't check for any other accepted commands of this event
								any_event_activated = true;
								break;
							}
						}
					}
				}
				else {
					print_log("[INFO] Score too low, ignoring...\n");
				}
			}
		}
		else {
			// Not listening, discard microphone input
			if (_vr.process_microphone(false) != 0) return -1;
		}
	}

	// Unreachable
	return 0;
}

int EventRecognizer::get_action_type(std::string s) {
	if (s == "execute") return action_execute;
	if (s == "print") return action_print;
	if (s == "bell") return action_bell;
	if (s == "play_audio") return action_play_audio;
	if (s == "speak") return action_speak;
	if (s == "key") return action_key;
	return -1;
}

void EventRecognizer::request_reload(std::string file_path) {
	_reload_profile_file_path = file_path;
	request_reload();
}
void EventRecognizer::request_reload() {
	_reload_requested = true;
	_vr.request_finish();
}

void EventRecognizer::set_listening(bool listen) {
	std::string temp = "[INFO] ";
	if (listen)	 {
		temp += "Starting";
	}
	else {
		_vr.request_finish();
		temp += "Stopping";
	}
	temp += " to listen...\n";
	print_log(temp);
	_listening = listen;
}

bool EventRecognizer::get_listening(void) {
	return _listening;
}

std::string EventRecognizer::get_profile(void) {
	return _reload_profile_file_path;
}