
Debian
====================
This directory contains files used to package atlasd/atlas-qt
for Debian-based Linux systems. If you compile atlasd/atlas-qt yourself, there are some useful files here.

## atlas: URI support ##


atlas-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install atlas-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your atlas-qt binary to `/usr/bin`
and the `../../share/pixmaps/atlas128.png` to `/usr/share/pixmaps`

atlas-qt.protocol (KDE)

