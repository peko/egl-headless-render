#!/bin/bash
../build/egl/test02 | \
    ffmpeg \
        -f rawvideo \
        -pix_fmt rgb32 \
        -s:v 854x480 \
        -r 60 \
        -i - \
        -threads 8 \
        -c:v libx264 \
        -y \
        ../test02.mp4
ffplay ../test02.mp4
