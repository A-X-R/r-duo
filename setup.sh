#!/bin/bash

if [ $UID -ne 0 ]; then
	echo "Must be root!"
	exit 1
fi


# Install required cherrypy python library
if ! dpkg --get-selections | grep -q python-cherrypy3; then
	apt-get install python-cherrypy3
fi

# Disable getty on ttyAMA0 (builtin UART)
if [ ! -e /etc/inittab.bak ]; then
	sed -i.bak '/ttyAMA0/s/^/#/' /etc/inittab
fi

# Install our startup script and set it for runlevel 2
DIRECTORY=$(cd $(dirname $(which $0)) && pwd)
TMPDIR=$(grep tmpfs /etc/fstab | grep -v log | awk '{print $2}')
sed s\|__DIR__\|$DIRECTORY\|\;s\|__TMPDIR__\|$TMPDIR\| etc/r-duo.sh > /etc/init.d/r-duo
chmod +x /etc/init.d/r-duo
update-rc.d -f r-duo remove
update-rc.d r-duo defaults
