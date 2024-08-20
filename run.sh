compile(){
    cd shaders
    glslc shader.vert -o vert.spv
    glslc shader.frag -o frag.spv
    cd ..
    cmake -S . -B build
    cmake --build build
}

run(){
    export DISPLAY=:0.0
    ./build/triangle
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
    *)
        echo "this is 程序运行脚本：（1：编译，2：运行，3：编译运行）"
        exit
        ;;
esac