## What's this repo?
Prove where I'm learning `Vulkan` step by step. 

## How to play?
Make sure you have installed these pkgs:
* Vulkan SDK
* GLFW

Clone the repo with the following command:

~~~ shell
git clone git@github.com:ILoveU3D/vkLearning.git
cd vkLearning
git submodule update --init --recursive
~~~

Then run this command to build & start this demo on Linux:
~~~ shell
sh install.sh 3
~~~

(Optional) If you want to debug with `GDB`, run this command:
~~~ shell
sh install.sh 4
~~~

(Optional) If you want to run this demo on other GPU(etc. NVIDIA, AMD...), run this command:
~~~ shell
sh install.sh ${YOUR_CHOICE} NVIDIA/AMD
~~~

(Optional) If you want to run this demo on Windows with `MSVC`, run those command:
~~~ shell
sh install.sh 1 ${GPU}
.\build\Debug\Triangle.exe
~~~

## You can see...
if everything goes rightly, you will see the rotated *Vking's room*:
![](./resources/viking_room.gif)

## What's more
* Now this demo is so simple and only be tested on Ubuntu platform.
* It seems to have some bugs of the `swap chain extent`...