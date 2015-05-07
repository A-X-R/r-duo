#!/bin/bash

WIDTH=320
HEIGHT=240
FPS=30

# The file 'twitch.key' shall have the contents 'TWITCH_KEY="[key]"'
# with the approrpriate key substituted
source twitch.key

LOCATION="rtmp://live.twitch.tv/app/$TWITCH_KEY live=1 flashver=FME/3.0%20(compatible;%20FMSc%201.0)"

gst-launch-1.0 \
    -v \
    \
    v4l2src device=/dev/video0 \
    ! video/x-raw,width=$WIDTH,height=$HEIGHT,framerate=$FPS/1 \
    ! queue \
    ! x264enc bitrate=500 key-int-max=30 tune="zerolatency" \
    ! h264parse \
    ! flvmux streamable=true \
    \
    alsasrc device=hw:1 \
    ! audio/x-raw \
    ! queue \
    ! voaacenc bitrate=16000 \
    ! aacparse \
    ! flvmux0. flvmux0. \
    \
    ! rtmpsink location="$LOCATION"
