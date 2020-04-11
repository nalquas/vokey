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
#include <QScrollBar>
#include <QString>
#include <QTimer>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

// Vokey includes
#include "../common/Config.h"
#include "../common/Communication.h"
#include "ui/manager.h"
#include "ui/about.h"

using namespace std;

QApplication *application;
QMainWindow *manager;
Ui_VokeyManager *ui_manager;
QDialog *about;
Ui_VokeyAbout *ui_about;

// Timers
QTimer *timer_monitor;

// Function headers

void about_open(void);
void about_close(void);
void discard_global_settings(void);
void refresh_log(void);
void refresh_monitor(void);
void reset_global_settings(void);
void save_global_settings(void);
void service_reload_profile(void);
void service_set_listening(void);
void quit(void);

// Function implementations

int main(int argc, char **argv) {
	ensure_config_exists();
	ensure_log_exists();
	ensure_tmp_exists();
	load_config();

	cout << "Launching VokeyManager...\n";

	application = new QApplication(argc, argv);

	// VokeyManager
	manager = new QMainWindow;
	ui_manager = new Ui_VokeyManager;
	ui_manager->setupUi(manager);

	// About Vokey
	about = new QDialog;
	ui_about = new Ui_VokeyAbout;
	ui_about->setupUi(about);

	// Connections: VokeyManager
	QObject::connect(ui_manager->actionQuit, &QAction::triggered, quit);
	QObject::connect(ui_manager->actionAbout, &QAction::triggered, about_open);
	QObject::connect(ui_manager->pushButton_restart, &QPushButton::clicked, service_reload_profile);
	QObject::connect(ui_manager->checkBox_listening, &QCheckBox::stateChanged, service_set_listening);
	QObject::connect(ui_manager->pushButton_monitor_refresh, &QPushButton::clicked, refresh_monitor);
	QObject::connect(ui_manager->buttonBox_global->button(QDialogButtonBox::Save), &QPushButton::clicked, save_global_settings);
	QObject::connect(ui_manager->buttonBox_global->button(QDialogButtonBox::Discard), &QPushButton::clicked, discard_global_settings);
	QObject::connect(ui_manager->buttonBox_global->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, reset_global_settings);

	// Connections: About Vokey
	QObject::connect(ui_about->buttonBox, &QDialogButtonBox::rejected, about_close);

	// Global settings tab: Load current settings from config
	discard_global_settings();

	// Start monitor-tab refresh timer
	timer_monitor = new QTimer();
	QObject::connect(timer_monitor, &QTimer::timeout, refresh_monitor);
	refresh_monitor();

	// Show application to user
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

void discard_global_settings() {
	ui_manager->lineEdit_default_profile->setText(QString::fromStdString(config["default_profile"]));
	ui_manager->checkBox_global_listening->setChecked(config["listening_on_startup"]);
}

void refresh_log() {
	// Load log from file
	string temp = "";
	ifstream ifs;
	ifs.open(VOKEY_TMP_LOG);
	temp.assign( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
	ifs.close();

	// Remember scroll position to restore after new text has loaded in
	QScrollBar *sb = ui_manager->textBrowser_monitor->verticalScrollBar();
	int scroll_position = sb->value();

	// Show log in GUI
	ui_manager->textBrowser_monitor->setText(QString::fromStdString(temp));

	// If the autoscroll-checkbox is checked, scroll to the bottom; Otherwise, restore previous scroll position
	if (ui_manager->checkBox_monitor_autoscroll->isChecked()) {
		sb->setValue(sb->maximum());
	}
	else {
		// Check whether the previous scroll position is still within scroll range; Otherwise, scroll to bottom
		if (scroll_position < sb->maximum()) {
			sb->setValue(scroll_position);
		}
		else {
			sb->setValue(sb->maximum());
		}
	}
}

void refresh_monitor() {
	// Stop the timer (just in case this was triggered by the manual refresh button)
	timer_monitor->stop();

	// Check if the service is running
	bool service_running = (get_service_pid() >= 0);

	// Only allow the listening checkbox to be interacted with if the service is running
	ui_manager->checkBox_listening->setEnabled(service_running);

	if (service_running) {
		// If the tmp-file for listening exists, get the current listening status and show it in the listening checkbox
		if (stat(VOKEY_TMP_LISTENING, &st) != -1) {
			bool listening = true;
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
		else {
			// As the listening file doesn't exist yet, assume default settings
			ui_manager->checkBox_listening->setChecked(config["listening_on_startup"]);
		}

		// Refresh the log view of the monitor tab
		refresh_log();
	}

	// Restart the timer
	timer_monitor->start(1500);
}

void reset_global_settings() {
	reset_config_to_default();
	discard_global_settings();
}

void save_global_settings() {
	config["default_profile"] = ui_manager->lineEdit_default_profile->text().toStdString();
	config["listening_on_startup"] = ui_manager->checkBox_global_listening->isChecked();
	save_config();
}

void service_reload_profile() {
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

void service_set_listening() {
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