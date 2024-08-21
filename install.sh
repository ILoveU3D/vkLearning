#!/bin/bash

# GPU型号
if [ -z "$2" ]; then
    # 如果没有提供 $2，将 GPU 设置为默认值 MT
    GPU="MooreThread"
else
    # 如果提供了 $2，将 GPU 设置为 $2 的值
    GPU=$2
fi

compile(){
    cmake -S . -B build -DGPU=${GPU}
    cmake --build build
    echo "compile shaders ..."
    cd shaders
    glslc shader.vert -o vert.spv
    glslc shader.frag -o frag.spv
    cd ..
}

run(){
    export DISPLAY=:0.0
    ./build/triangle
}

debug(){
    export DISPLAY=:0.0
    gdb ./build/triangle
}

case $1 in
    1)
        compile
        ;;
    2)
        run
        ;;
    3)
        compile
        run
        ;;
    4)
        compile
        debug
        ;;    
    *)
        echo "this is 程序运行脚本：（1：编译，2：运行，3：编译运行，4：编译后debug）"
        exit
        ;;
esac