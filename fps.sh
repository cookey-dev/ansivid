#!/bin/sh
rm -rf frames
mkdir frames

ffprobe -v quiet -select_streams v -of default=noprint_wrappers=1:nokey=1 -show_entries stream=r_frame_rate $* | bc -l | awk '{printf("%d\n", $1 + 0.5)}' &> ./fps.txt
ffmpeg -i $* frames/f-%03d.png
