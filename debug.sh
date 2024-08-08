compile(){
    cd shaders
    glslc shader.vert -o vert.spv
    glslc shader.frag -o frag.spv
    cd ../build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make
}

run(){
    export DISPLAY=:0.0
    gdb ./triangle
}

case $1 in
    1)
        compile
        cd ..
        ;;
    2)
        cd build
        run
        cd ..
        ;;
    3)
        compile
        run
        cd ..
        ;;
    *)
        echo "this is 三角形程序运行脚本：（1：编译，2：运行，3：编译运行）"
        exit
        ;;
esac