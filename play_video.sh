#!/bin/bash

# 定义视频名称数组
videos=("WalkingStaticBackground" "WalkingMovingBackground" "SAL" "Stairs" "car" "orange")

# 设置可执行文件的路径
MyPlayer="/Users/gg/Desktop/course/576/CS576_Finaƒl_Project/cmake-build-debug/MyImageApplication"

# 设置基础路径
BASE_PATH="/Users/gg/Desktop/course/576/CS576_Final_Project/assets"

# 遍历并执行每个视频
for video in "${videos[@]}"
do
    echo "Playing video: $video"
    "$MyPlayer" "$BASE_PATH/outputs/PROCESSED_$video.rgb" "$BASE_PATH/rgbs/$video.rgb" "$BASE_PATH/wavs/$video.wav" 960 540 30
done