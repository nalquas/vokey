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
// The main source file of Vokey's management application.

// Includes
#include <csignal>
#include <fstream>
#include <iostream>
#include <QApplication>
#include <QAction>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

// Vokey includes
#include "../common/config.h"
#include "../common/communication.h"
#include "ui/manager.h"
#include "ui/about.h"

using namespace std;

QApplication *application;
QMainWindow *manager;
Ui_VokeyManager *ui_manager;
QDialog *about;
Ui_VokeyAbout *ui_about;

// Internal function headers
pid_t get_service_pid(void);
void refresh_status(void);

// GUI-function headers
void about_open(void);
void about_close(void);
void reload_profile(void);
void set_listening(void);
void quit(void);

// Function implementations

int main(int argc, char **argv) {
	ensure_config_exists();
	load_config();

	cout << "Launching VokeyManager...\n";

	application = new QApplication(argc, argv);

	// VokeyManager
	manager = new QMainWindow;
	ui_manager = new Ui_VokeyManager;
	ui_manager->setupUi(manager);
	refresh_status();

	// About Vokey
	about = new QDialog;
	ui_about = new Ui_VokeyAbout;
	ui_about->setupUi(about);

	// Connections: VokeyManager
	QObject::connect(ui_manager->actionQuit, &QAction::triggered, quit);
	QObject::connect(ui_manager->actionAbout, &QAction::triggered, about_open);
	QObject::connect(ui_manager->pushButton_restart, &QPushButton::clicked, reload_profile);
	QObject::connect(ui_manager->checkBox_listening, &QCheckBox::stateChanged, set_listening);
	QObject::connect(ui_manager->pushButton_monitor_refresh, &QPushButton::clicked, refresh_status);

	// Connections: About Vokey
	QObject::connect(ui_about->buttonBox, &QDialogButtonBox::rejected, about_close);

	manager->show();
	return application->exec();
}

// GUI-functions

void about_open() {
	about->show();
}

void about_close() {
	about->close();
}

void reload_profile() {
	pid_t pid = get_service_pid();
	if (pid >= 0) {
		// Get data
		string selected_profile = ui_manager->comboBox_monitor_profile->currentText().toStdString();
		cout << "Sending signal to change profile to " << selected_profile << "\n";

		// Store data
		ensure_tmp_exists();
		ofstream ofs;
		ofs.open(VOKEY_TMP_PROFILE);
		ofs << selected_profile;
		ofs.close();

		// Send signal for service to fetch data
		kill(pid, SIGUSR1);
	}
	// TODO If the service does not exist, start it
}

void set_listening() {
	pid_t pid = get_service_pid();
	if (pid >= 0) {
		// Get data
		bool listening = ui_manager->checkBox_listening->isChecked();
		cout << "Sending signal to change listening state to " << (int)listening << "\n";
		
		// Store data
		ensure_tmp_exists();
		ofstream ofs;
		ofs.open(VOKEY_TMP_LISTENING);
		ofs << listening;
		ofs.close();

		// Send signal for service to fetch data
		kill(pid, SIGUSR2);
	}
	// TODO If the service does not exist, start it
}

void quit() {
	application->closeAllWindows();
}

// Internal functions

pid_t get_service_pid() {
	// Check if directory and the PID-file exist
	if (stat(VOKEY_TMP, &st) != -1 && stat(VOKEY_TMP_PID, &st) != -1) {
		string temp = "";
		pid_t pid = 0;
		
		ifstream ifs;
		ifs.open(VOKEY_TMP_PID);
		ifs >> temp;
		ifs.close();

		// Is the PID given in the file still running?
		try {
			pid = stoi(temp);
			if (0 == kill(pid, 0)) return pid;
		}
		catch(const exception& e) {
			cerr << e.what() << '\n';
			cout << "As the temporary PID file caused an error, we'll just assume there is no instance running...\n";
		}
	}

	// Process does not exist
	return -1;
}

void refresh_status() {
	ensure_tmp_exists();

	// Monitor tab

	bool service_running = (get_service_pid() >= 0);

	ui_manager->checkBox_listening->setEnabled(service_running);

	if (service_running) {
		// Listening
		bool listening = true;
		if (stat(VOKEY_TMP_LISTENING, &st) != -1) {
			string temp = "";
			ifstream ifs;
			ifs.open(VOKEY_TMP_LISTENING);
			ifs >> temp;
			ifs.close();

			try
			{
				listening = (stoi(temp) > 0);
			}
			catch(const exception& e)
			{
				cerr << e.what() << '\n';
			}

			ui_manager->checkBox_listening->setChecked(listening);
		}
	}
	else {
		// Get defaults from config
		ui_manager->checkBox_listening->setChecked(config["listening_on_startup"]);
	}

	// Global settings tab
	ui_manager->checkBox_global_listening->setChecked(config["listening_on_startup"]);
	ui_manager->lineEdit_default_profile->setText(QString::fromStdString(string(config["default_profile"])));
}