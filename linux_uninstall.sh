#!/bin/bash

# This is a simple uninstall script to assist users in removing Vokey.
# All it does is remove Vokey's binaries from /usr/bin.
# The config files are not touched, however, letting profiles persist.

if !(test -f "/usr/bin/vokey_service";) && !(test -f "/usr/bin/vokey_manager";) && !(test -f "/usr/share/applications/Vokey Manager.desktop";) then
	echo "ERROR: Neither vokey_service nor vokey_manager are installed in /usr/bin and there's no .desktop file."
else
	echo "This uninstall script will remove Vokey from /usr/bin and remove the .desktop file."
	echo "You will need to provide root permissions if prompted."
	echo
	read -p "Proceed? [y/n]: " -n 1 -r
	echo
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		echo "Uninstalling..."
		if test -f "/usr/bin/vokey_service"; then sudo rm /usr/bin/vokey_service; fi
		if test -f "/usr/bin/vokey_manager"; then sudo rm /usr/bin/vokey_manager; fi
		if test -f "/usr/share/applications/Vokey Manager.desktop"; then sudo rm "/usr/share/applications/Vokey Manager.desktop"; fi
		echo "Done!"
	fi
fi
