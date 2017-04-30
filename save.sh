#!/bin/bash
./build/main | \
    ffmpeg \
        -f rawvideo \
        -pix_fmt rgb32 \
        -s:v 854x480 \
        -r 60 \
        -i - \
        -threads 8 \
        -c:v libx264 \
        -y \
        /tmp/video.mp4
ffplay /tmp/video.mp4
