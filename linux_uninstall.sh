#!/bin/bash

# This is a simple uninstall script to assist users in removing Vokey.
# All it does is remove Vokey's binaries from /usr/bin.
# The config files are not touched, however, letting profiles persist.

if !(test -f "/usr/bin/vokey_service";) && !(test -f "/usr/bin/vokey_manager";) then
	echo "ERROR: Neither vokey_service nor vokey_manager are installed in /usr/bin."
else
	echo "This uninstall script will remove vokey_service and vokey_manager from /usr/bin."
	echo "You will need to provide root permissions if prompted."
	echo
	read -p "Proceed? [y/n]: " -n 1 -r
	echo
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		echo "Uninstalling..."
		if test -f "/usr/bin/vokey_service"; then sudo rm /usr/bin/vokey_service; fi
		if test -f "/usr/bin/vokey_manager"; then sudo rm /usr/bin/vokey_manager; fi
		echo "Done!"
	fi
fi