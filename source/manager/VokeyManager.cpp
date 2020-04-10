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

#define VOKEY_CONFIG_VERSION 1

#define VOKEY_TMP "/tmp/vokey"
#define VOKEY_TMP_PID "/tmp/vokey/vokey.pid"
#define VOKEY_TMP_LOG "/tmp/vokey/vokey.log"

// Libraries
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
#include <string>
#include <sys/stat.h>
#include <unistd.h>

// UI with layout
#include "ui/manager.h"
#include "ui/about.h"

using namespace std;
struct stat st = {0};

QMainWindow *manager;
Ui_VokeyManager *ui_manager;
QDialog *about;
Ui_VokeyAbout *ui_about;

// Function headers
void about_open(void);
void about_close(void);
pid_t get_service_pid(void);
void reload_profile(void);
void set_listening(void);

// Function implementations

int main(int argc, char **argv)
{
	cout << "Launching VokeyManager...\n";
	QApplication app(argc, argv);

	// VokeyManager
	manager = new QMainWindow;
	ui_manager = new Ui_VokeyManager;
	ui_manager->setupUi(manager);

	// About Vokey
	about = new QDialog;
	ui_about = new Ui_VokeyAbout;
	ui_about->setupUi(about);

	// Connections: VokeyManager
	QObject::connect(ui_manager->actionQuit, &QAction::triggered, app.closeAllWindows);
	QObject::connect(ui_manager->actionAbout, &QAction::triggered, about_open);
	QObject::connect(ui_manager->pushButton_restart, &QPushButton::clicked, reload_profile);
	QObject::connect(ui_manager->checkBox_listening, &QCheckBox::stateChanged, set_listening);

	// Connections: About Vokey
	QObject::connect(ui_about->buttonBox, &QDialogButtonBox::rejected, about_close);

	manager->show();
	return app.exec();
}

void about_open() {
	about->show();
}

void about_close() {
	about->close();
}

pid_t get_service_pid() {
	// Make sure directory and the PID-file exist
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

void reload_profile() {
	pid_t pid = get_service_pid();
	if (pid >= 0) {
		// TODO put the selected profile path in a file
		cout << "Sending signal to change profile to TODO\n";
		kill(pid, SIGUSR1);
	}
	// TODO If the service does not exist, start it
}

void set_listening() {
	pid_t pid = get_service_pid();
	if (pid >= 0) {
		// TODO put the new listening state in a file
		cout << "Sending signal to change listening state to TODO\n";
		kill(pid, SIGUSR2);
	}
	// TODO If the service does not exist, start it
}
