#!/bin/bash

# This is a simple uninstall script to assist users in removing Vokey.
# All it does is remove Vokey's binaries from /usr/bin.
# The config files are not touched, however, letting profiles persist.

if !(test -f "/usr/bin/vokey_service";) && !(test -f "/usr/bin/vokey_manager";) && !(test -f "/usr/share/applications/Vokey Manager.desktop";) && !(test -f "/usr/share/icons/hicolor/scalable/apps/vokey.svg";) then
	echo "ERROR: Nothing to uninstall."
else
	echo "This uninstall script will remove Vokey from your system."
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
		if test -f "/usr/share/icons/hicolor/scalable/apps/vokey.svg"; then sudo rm /usr/share/icons/hicolor/scalable/apps/vokey.svg; fi
		echo "Done!"
	fi
fi
