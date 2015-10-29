#!/bin/bash
### BEGIN INIT INFO
# Provides:          r-duo
# Required-Start:    networking
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start r-duo
# Description:       Start r-duo
### END INIT INFO

/usr/bin/python __DIR__/www/remote.py &> __TMPDIR__/r-duo.log &

exit 0
