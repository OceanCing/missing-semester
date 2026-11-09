-|  普通文件                           |                目录         |
 | 你写的实际内容（文本、二进制）      |一张表：文件名 → inode 编号  |
 |能（cat、vim）                       | 不能，不能 cat 一个目录     |
-目录与文件之间的联系：
 -|  普通文件                           |                目录         |
 | 你写的实际内容（文本、二进制）      |一张表：文件名 → inode 编号  |
 |能（cat、vim）                       | 不能，不能 cat 一个目录     |
-目录与文件之间的联系：
 - 相似点：都是"文件系统对象"，目录本质上也是一种文件——一种特殊的文件。所有东西都通过 inode（索引节点）
 - inode  →  元数据（类型、权限、大小、时间、数据块位置）
 -  数据块 →  实际内容
 - ***目录是"文件名的容器"，文件必须挂在某个目录的映射表里才有名字。没有目录，文件只能靠 inode 号访问（几乎没法用）。***



#创建文件夹的指令对照
 -  创建单个文件夹	                   mkdir myfolder
    创建多个平级文件夹                 mkdir dir1 dir2 dir3
    创建多层嵌套（父目录可能不存在）   mkdir -p ~/catkin_ws/src
    创建三层	                       mkdir -p a/b/c
    创建时显示过程	                   mkdir -pv a/b/c（-v = verbose）
    创建带指定权限的目录	           mkdir -m 755 myfolder
    图形界面方式	文件管理器里右键 →「新建文件夹」


#为何用cmake?
1.手工时代（g++）：
直接在终端敲：g++ talker.cpp -o talker -I/opt/ros/noetic/include -L/opt/ros/noetic/lib -lroscpp ...
这太痛苦了，依赖库一旦多了，这行命令长得像面条，根本记不住
2.Makefile 时代（make）：
有人发明了 Makefile 文件，里面写清楚怎么编译。只要敲一个 make 命令，它就自动照做。
3.Make 时代（cmake）：
但是，不同电脑的路径不一样，手写 Makefile 依然很麻烦。于是诞生了 CMake。
只需要写一个极其简单的配置文件，叫做 CMakeLists.txt，里面写上：“我要编译 talker.cpp，我需要用到 roscpp 库”。
CMake 会读取这个文件，自动生成复杂的 Makefile，然后再调用 make 去编译。


##CMake 和 ROS（Catkin）的关系
***Catkin 是什么？ 它本质上就是一套包含大量 ROS 专属宏和函数的 CMake 脚本集合。***


###catkin_make 和它们的关系
-在 catkin_ws 目录下敲下的那句 catkin_make，到底干了什么？
***这个词是 catkin + cmake + make 的组合。它是一个 Python 脚本执行以下任务：
    1.建目录：创建 build 和 devel 文件夹（保持源码干净）。

    2.调用 CMake：进入 build 目录，读取src 里所有包的 CMakeLists.txt，生成 Makefile。

    3.调用 Make：执行 make，让编译器（g++）开始真正地去一行行编C++ 代码。***
 **如果没有 catkin_make，需要自己建目录、手动 cd 到 build 里、手动敲 cmake ..、再手动敲 make，极其繁琐。catkin_make 把这三步打包成了一键式操作。**





#catkin
-创建catkin workspace

 ```mkdir -p ~/catkin_ws/src
    cd ~/catkin_ws/
    catkin_make    
<!--catkin_make 是 ROS 1 (Catkin) 体系里的一个核心命令行构建工具。你可以把它理解为一道“自动化流水线”，它的核心任务是将标准 CMake 的多步构建流程（mkdir、cd、cmake、make）打包成一个简单的命令，从而简化整个工作空间的编译过程。它的作用就是把写在 src 目录里的各种代码，翻译成计算机能懂的机器语言，并配置好运行环境。
原理：-->
    source devel/setup.bash
<!--source 是 Bash 的内置命令（也有等价写法 .，即一个点）。它的作用是：

    在当前 Shell 进程中读取并执行指定脚本里的命令，而不是新开一个子 Shell 去执行。-->
<!--devel/setup.bash是 catkin_make 编译成功后，自动生成在 devel/ 目录下的一个环境配置脚本。--> ```


-explian
 ***mkdir	make directory，              创建目录
    -p	parents，                         递归创建：如果父目录 ~/catkin_ws 不存在也会一并创建；若目录已存在则不报错
    ~	                                  当前用户主目录，如 /home/username
    catkin_ws	                          工作空间根目录（ws = workspace）
    src	                                  存放 ROS 功能包源码的目录***


##知识串连
  -基础概念回顾：
   -shell:人与内核沟通的桥梁，既是命令解释器，也是编程语言
┌─────────────────────────────┐
│  你（键盘输入 / 鼠标点击）      │
├─────────────────────────────┤
│  Shell（命令解释器）           │  ← 你直接打交道的这一层
├─────────────────────────────┤
│  内核 Kernel（管理CPU/内存/磁盘）│
├─────────────────────────────┤
│  硬件                         │
└─────────────────────────────┘
---
   进而引入脚本的概念
---
  -脚本：用 Shell 语言写的、一堆命令的文本文件

###
   |	              |     脚本                        |                 编译型程序（如 C/C++）
   |  形式	          |     纯文本                 	    |                 二进制机器码
   |  执行方式	      |                             |     解释器逐行读、逐行执行	先整体编译成可执行文件，再运行
   |  需要编译器吗	  |   不需要	                    |    需要（gcc、g++…）
   |  运行速度	      |     较慢（边翻译边执行）	    |     快（已翻译好）
   |  修改后	      |直接改文本即可	                |      必须重新编译
   |  典型例子	      |  .sh、.py、.js	                |      .exe、.out          
*** catkin_make 本身其实也是脚本（一个 Python 脚本），内部自动调用 cmake、make 等命令***
*** devel/setup.bash    catkin_make 运行后生成的 Shell 脚本*** 

####conclude:Shell 是“能读懂命令并执行”的翻译官兼编程语言；脚本是用这门语言写好的“命令台词本”；source setup.bash 就是让翻译官照着台词本，在当前这个“现场”把环境配置好。
   

#创建catmake的前置条件
**  既然 ROS 1 Noetic 是安装在 Docker 容器里面的，那么宿主机（也就是Ubuntu 系统）里是绝对没有 ROS 环境的。宿主机终端里敲 catkin_make，它会报 command not found，因为宿主机根本没装。因此，必须先进入 Docker 容器，在容器内部执行编译操作。**
##进入ros环境
1检查Docker 容器运行状态
```docker ps <!--检查容器是正在运行还是停止，列出你所有的容器。找到那个安装了 ROS 的容器，记下它的 CONTAINER ID 或者 NAMES。如果它的 STATUS 显示是 Exited（已停止），需要先启动它-->```
2.启动容器
```docker start <你的容器ID或名字>```
    <!--在终端命令里，< > 只是用来表示“这里要填入你自己的内容”的占位符，实际输入时绝对不能带上这两个符号（在 Linux 中，< 和 > 有特殊含义，代表输入/输出重定向，直接敲会引发语法错误）。eg.docker start ros1-noetic-->
3.进入这个正在运行的容器（打开一个交互式终端）：
```docker exec -it <你的容器ID或名字> /bin/bash```
<!--注意：如果你容器的默认 shell 是 zsh，就把 /bin/bash 换成 /bin/zsh。敲完后按回车，命令行提示符前缀会发生变化，这就代表已经“进入” Docker 容器内部了。）-->
4.加载 ROS 环境
```source /opt/ros/noetic/setup.bash```
5.创建工作区
```mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/
catkin_make```



##执行以上成功后，进而激活新生成的环境变量
```source devel/setup.bash```
<!--source 是 Linux Bash 的一个内置命令（也可以简写为一个点 .）。
它的作用是：在当前 Shell 环境中读取并执行指定的脚本文件。-->
<!--
1.为什么要用 source？
 如果直接运行 ./devel/setup.bash，系统会开启一个“子 Shell”来执行它，执行完子 Shell 就关闭了，里面设置的环境变量也就随之消失了。而 source 命令会在当前终端里执行，这样脚本里导出的环境变量就能立刻在当前终端生效。
2.为什么不 source 就不行？

如果不执行这条命令，虽然你catkin_make 编译成功了，但当前的终端根本不知道你刚刚编译了东西。去运行自己的包时，终端会报错说找不到这个包（[rospack] Error: package 'xxx' not found）。
3.用 source 会发生什么？

当输入 source devel/setup.bash 时：

    系统不会新建进程。

    系统会直接把当前的“当前终端”（ID 1000）的记事本拿过来，把 setup.bash 里的代码一行一行直接在当前进程里执行。

    遇到 export ROS_PACKAGE_PATH=...，直接写在当前记事本上。

    脚本执行完毕，你的“当前终端”（ID 1000）脑子里已经记住了所有新的环境变量。一敲 rosrun，它立刻就知道去哪里找你的包。
4.devel/setup.bash 是什么？

当刚才运行 catkin_make 编译成功后，ROS 会自动在catkin_ws 目录下生成一个 devel 文件夹。setup.bash 是 ROS 自动生成的环境配置脚本。

在这个脚本里，ROS 把大量的系统环境变量配置好了，主要包括：

    ROS_PACKAGE_PATH：告诉 ROS 去哪里找你刚才写的包（指向了 catkin_ws/src）。

    PATH：把你自己编译出来的可执行文件路径（catkin_ws/devel/lib）加进去，这样在终端里直接敲你写的节点名字（比如 rosrun my_pkg my_node），系统就能找到它。

    PYTHONPATH：配置 Python 脚本的搜索路径。

    动态链接库路径（LD_LIBRARY_PATH）：让程序能编译出来的 C++ 库。
5.环境叠加（Overlay）
 当执行 source devel/setup.bash 时，它不仅配置了当前工作空间的路径，还会自动把底层（Underlay）的环境变量也包含进来。
在Docker 容器里，底层就是 /opt/ros/noetic/setup.bash。所以它其实做了一件“承上启下”的事情：既保留了系统自带的 ROS 环境，又加上了你自己编译的包。


###conclude:
1.每次 catkin_make 编译完，只要在 src 里新增了包或者修改了 CMakeLists.txt，记得立刻 source devel/setup.bash 刷新一下当前终端。
2.新开一个终端（比如想开两个终端，一个跑 roscore，一个跑 rosrun），如果要用到你自己的包，新终端里也要先 source devel/setup.bash。
3.  快捷配置（可选）：可以把 source /root/catkin_ws/devel/setup.bash 写到容器的 ~/.bashrc 里（和之前配置 /opt/ros/noetic/setup.bash 一样），这样每次进终端就不用敲了。不过有些开发者喜欢手动敲，避免不同工作空间互相干扰。


####执行代码
-在容器内写代码
 -src/：写代码、建包的地方
  - 1.进入源码目录：cd ~/catkin_ws/src

    2.创建一个 ROS 包：catkin_create_pkg my_pkg roscpp rospy

    3.进入包：cd my_pkg

    4.在这里面的 src 目录下新建 .cpp 或 .py 文件开始写代码。

    5.如果是 C++，还要去包里的 CMakeLists.txt 配置编译规则。


***warning:
虽然写代码是在 src 里面，但是准备执行 catkin_make 编译时，必须退回到 catkin_ws 根目录！

如果现在处于 src 目录下敲 catkin_make，它会报错。正确的流程是：

    cd ~/catkin_ws/src （进入源码目录写代码）

    写完代码后，cd .. （退回到上一级，也就是 catkin_ws）

    确认提示符是 ~/catkin_ws# 时，再敲 catkin_make。***


-在 Docker 容器外（宿主机）编辑
 -目录挂载
  1.退出并删除当前的容器（因为里面没代码，直接删了不可惜）：
```# 在容器内
    exit
    # 回到宿主机
    docker rm -f ros1-noetic```
  2.在宿主机（Ubuntu）建好文件夹：
```cd ~
mkdir -p catkin_ws/src```
  3.用带有 -v（挂载）参数的命令重新启动一个容器：
这条命令的意思是把宿主机的 /home/guo-tongyan/catkin_ws 和容器里的 /root/catkin_ws 绑定在一起（两边同步
(注意：把 /home/guo-tongyan/ 换成你实际的宿主机用户名路径)）：
```docker run -it --name ros1-noetic \
  -v /home/guo-tongyan/catkin_ws:/root/catkin_ws \
  osrf/ros:noetic-desktop-full bash```



***解析以上代码：1. docker run

创建并启动一个新的容器。这是 Docker 最核心的命令之一。
2. -it

由两个短选项组合而成：

    -i（--interactive）：保持容器的标准输入（STDIN）打开，允许你向容器内输入命令。

    -t（--tty）：为容器分配一个伪终端（pseudo-TTY），让你看到类似本地终端的效果。

    -it 几乎是交互式使用容器的标配组合，缺少任何一个都会导致 shell 体验异常。

3. --name ros1-noetic

为容器指定一个固定名称 ros1-noetic。

    不指定时 Docker 会随机生成名字（如 hungry_hopper）。

    指定名称后可以用 docker start ros1-noetic、docker exec -it ros1-noetic bash、docker stop ros1-noetic 等命令直接引用。

4. -v /home/guo-tongyan/catkin_ws:/root/catkin_ws

挂载卷（bind mount），语法为 -v <宿主机路径>:<容器内路径>。
部分	含义
/home/guo-tongyan/catkin_ws	宿主机上的 ROS 工作空间目录
/root/catkin_ws	容器内的目标路径（ROS 镜像默认用户是 root，所以放在 /root 下）

作用：宿主机上的代码与容器内的代码实时同步。

    在宿主机用编辑器改代码 → 容器内立即可见。

    容器内编译产生的 build/、devel/ 也会保留在宿主机，容器删除后不丢失。

    避免每次重建容器都丢失工作空间。

5. osrf/ros:noetic-desktop-full

镜像名称，格式为 仓库名:标签：

    osrf/ros：Open Source Robotics Foundation 官方维护的 ROS Docker 镜像仓库。

    noetic：ROS 1 的 Noetic Ninjemys 版本（对应 Ubuntu 20.04，是 ROS 1 的最后一个长期支持版本，EOL 为 2025 年 5 月）。

    desktop-full：镜像变体，包含 ROS、RViz、Gazebo、以及所有感知/仿真相关的包，体积最大、功能最全（约 3–4 GB）。

    ROS 官方镜像变体常见有：ros-core < ros-base < desktop < desktop-full。***

4.容器退出后仍保留：如果只是临时实验，可以加 --rm 让容器退出时自动删除：
```docker run -it --rm --name ros1-noetic ...```
**注意：已用 -v 挂载了工作空间，代码不会丢，但容器内 apt install 的软件会丢失。**



####conclude:
1.在 Docker 容器里（ros1-noetic 镜像），里面已经预装了 CMake 和 Catkin

2.进入 ~/catkin_ws/src，准备建包。

3.修改包里的 CMakeLists.txt（这就是你学 cmake_study.md 的用武之地）。

4.退回 ~/catkin_ws，敲下 catkin_make。

5.catkin_make 在后台默默调用 cmake 解析你写的 CMakeLists.txt，然后调用 make 执行编译。

6.最终，生成了可在 devel/lib 里运行的二进制程序。 

####思疑：对于conclude中的第3点：为什么还要去改那个配置文件CMakeLists.txt？
答：
    1.在 src 目录下新建并编写了 talker.cpp。但在 Linux 系统眼里，它就是一个普通的文本文件，和 readme.txt 没有任何区别。计算机是无法直接运行 C++ 代码的，必须经过编译（把 C++ 翻译成机器码），变成可执行文件才能跑。
    2.CMakeLists.txt 就是写给系统看的“施工指令”。
如果不修改它，你就算在 src 里把 talker.cpp 写得再完美，当你敲 catkin_make 时，系统扫一眼 CMakeLists.txt，发现里面什么都没写，它会直接略过你的代码，什么都不会编译。
    3.具体配置(在CMakeLists.txt包中)：
```add_executable(talker src/talker.cpp)```
<!--告诉系统：我要把 src/talker.cpp 编译成一个叫 talker 的可执行文件-->
```target_link_libraries(talker ${catkin_LIBRARIES})```
<!--告诉系统：这个程序用到了 ROS 的库（比如 roscpp），请帮我把这些库链接进去，否则运行会报错找不到符号-->



#####The relationship between code and configuration.
1.Writing .cpp files in src is writing business logic.
2.Modifying CMakeLists.txt is writing build rules.
3.The source code determines what the program can do.
4.CMakeLists.txt determines how the program is built.
5.Both are indispensable!
