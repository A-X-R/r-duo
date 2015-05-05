#!/bin/bash

WIDTH=640
HEIGHT=480
FPS=30

TWITCH_KEY="live_89916911_6FSHP4XbA7b7lE4ne1SPAkxZNuVKNr"
LOCATION="rtmp://live.twitch.tv/app/$TWITCH_KEY live=1 flashver=FME/3.0%20(compatible;%20FMSc%201.0)"

gst-launch-1.0 \
    -v \
	rtpbin name=rtpbin v4l2src device=/dev/video0 \
    ! video/x-raw,width=$WIDTH,height=$HEIGHT,framerate=$FPS/1 \
	! queue \
    \
	! omxh264enc target-bitrate=1000000 control-rate=3 interval-intraframes=1000 \
    ! video/x-h264,profile=high \
	! h264parse \
	! queue max-size-bytes=10000 \
    \
    ! flvmux streamable=true name=mux \
    ! queue \
    \
    ! rtmpsink location="$LOCATION"
