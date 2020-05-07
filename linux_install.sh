#!/bin/bash

# This is a simple install script to assist users in installation.
# All it does is copy the compiled program files into /usr/bin.

if !(test -f "vokey_service";) || !(test -f "vokey_manager";) then
	echo "ERROR: vokey_service or vokey_manager are not in this directory."
	echo "To run this script, you need to be in the repository's root folder."
	echo "Make sure you run \"make\" before you run this script."
else
	echo "This installer will install Vokey to your system."
	echo "You will need to provide root permissions if prompted."
	echo
	read -p "Proceed? [y/n]: " -n 1 -r
	echo
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		echo "Installing..."
		sudo cp -f ./vokey_service /usr/bin/vokey_service
		sudo cp -f ./vokey_manager /usr/bin/vokey_manager
		sudo cp -f "./desktop/Vokey Manager.desktop" "/usr/share/applications/Vokey Manager.desktop"
		sudo cp -f ./desktop/vokey.svg /usr/share/icons/hicolor/scalable/apps/vokey.svg
		echo "Done!"
	fi
fi
