#!/bin/bash

# This is a simple install script to assist users in installation.
# All it does is copy the compiled program files into /usr/bin.

if !(test -f "vokey_service";) || !(test -f "vokey_manager";) then
	echo "ERROR: vokey_service or vokey_manager are not in this directory."
	echo "To run this script, you need to be in the repository's root folder."
	echo "Make sure you run \"make\" before you run this script."
else
	echo "This install script will copy vokey_service and vokey_manager to /usr/bin."
	echo "You will need to provide root permissions if prompted."
	echo
	read -p "Proceed? [y/n]: " -n 1 -r
	echo
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		echo "Installing..."
		sudo cp -f ./vokey_service /usr/bin/vokey_service
		sudo cp -f ./vokey_manager /usr/bin/vokey_manager
		echo "Done!"
	fi
fi