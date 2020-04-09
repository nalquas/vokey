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

// UI with layout
#include "ui/manager.h"
#include "ui/about.h"

using namespace std;

int main(int argc, char **argv)
{
	cout << "Launching VokeyManager...\n";

	QApplication app(argc, argv);

	QMainWindow manager;

	Ui_MainWindow ui;
	ui.setupUi(&manager);

	manager.show();
	return app.exec();
}
