compile(){
    cd shaders
    glslc shader.vert -o vert.spv
    glslc shader.frag -o frag.spv
    cd ../build
    cmake ..
    make
}

run(){
    export DISPLAY=:0.0
    ./triangle
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
        echo "this is 程序运行脚本：（1：编译，2：运行，3：编译运行）"
        exit
        ;;
esac