#!/bin/bash

# 定义视频名称数组
videos=("WalkingStaticBackground" "WalkingMovingBackground" "SAL" "Stairs" "car" "orange")  # 根据你的需求添加视频名称

# 设置可执行文件的路径
MyEncoder="/Users/gg/Desktop/course/576/CS576_Final_Project/cmake-build-debug/MyEncoder"
MyDecoder="/Users/gg/Desktop/course/576/CS576_Final_Project/cmake-build-debug/MyDecoder"

# 遍历并执行每个视频
for video in "${videos[@]}"
do
    echo "Encoding video from .rgb to .cmp file: $video"
    "$MyEncoder" "$video"
    echo "Decoding video from .cmp to .rgb file: $video"
    "$MyDecoder" "$video"
done