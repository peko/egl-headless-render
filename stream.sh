#!/bin/bash
./main | \
    ffmpeg \
        -f rawvideo \
        -pix_fmt rgb32 \
        -s:v 854x480\
        -r 60 \
        -i - \
        -threads 8 \
        -c:v libx264 \
        -f flv \
        rtmp://${SERVER}/${APP}/render
