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

// Libraries
#include <iostream>
#include <QApplication>
#include <QAction>
#include <QObject>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>

// UI with layout
#include "ui/manager.h"
#include "ui/about.h"

using namespace std;

QMainWindow *manager;
Ui_VokeyManager *ui_manager;
QDialog *about;
Ui_VokeyAbout *ui_about;

// Function headers
void about_open(void);
void about_close(void);

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
