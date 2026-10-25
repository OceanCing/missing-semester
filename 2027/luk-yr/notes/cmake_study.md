# CMake与catkin

## 1.引入

C++程序怎么变成可执行文件

比如main.cpp本身只是文本文件，存着自己写的字符，要经过以下步骤才能运行程序

- 预处理	展开头文件里面的代码

- 编译            转换成汇编代码（理解为一种比C++更接近机器语言的表示）

- 汇编             转换成二进制编码

- 链接              例如main.cpp 跟add.cpp有关系，需要将他们的.o联系起来

## 2.为什么需要CMake与catkin

 ### Makefile

当文件越来越多时，改了一个文件却需要把全部文件重新编译，费时久。

因此引入 `Makefile`，把规则写进 `Makefile`，`make`就会读取，判断哪些步骤需要执行，再替你调用编译器。

**以便减少重复工作**

### CMake

项目很大时，编写和维护 Makefile 本身也麻烦。这时，你可以在 `CMakeLists.txt` 中告诉 CMake生成`Makefile`

**总的来说**

- **CMake**：根据你写的项目要求，生成构建规则。

- **make**：读取 Makefile 中的规则，调用编译工具去执行。

- **编译器等工具**：实际完成代码转换、汇编和链接。

## 3.CMake的使用

怎么告诉CMake项目要做什么

比如要用main.cpp生成demo，在项目根目录的`CMakeLists.txt`里写：

```cmake
cmake_minimum_required(VERSION 3.16)
project(Demo)
add_executable(demo main.cpp)
```

- cmake_minimum_required：要求的最低版本。
- project：项目叫什么。
- add_executable：用哪些源文件生成什么程序。

Demo是项目名，demo是程序名（可以不一样）。

写好后还要执行命令：

```bash
cmake -S . -B build    # 生成构建文件，放到build里
cmake --build build   # 根据规则，调用工具编译和链接
./build/demo          # 运行程序
```

### 多个文件

文件多了，可以把.cpp放在src，.h放在include，方便整理。

比如main.cpp要用add函数，add.h告诉它怎么调用，add.cpp里写具体怎么计算。所以头文件要找得到，实现也要加进程序。

- `target_include_directories`：告诉编译器去哪里找头文件。
- `target_link_libraries`：告诉构建工具要链接哪些库。
- `find_package`：查找项目需要的外部依赖，通常不会自动安装。

### 变量

比如`set(SOURCES main.cpp add.cpp)`，把两个文件名放到SOURCES里，后面用`${SOURCES}`取出来，就不用重复写。

## 4.catkin的使用

### 工作空间

可以理解为把多个功能包放在一起的地方，构建结果也放在这里。

- src：放各个包。
- build：放构建过程中产生的文件。
- devel：放开发时使用的构建结果和环境脚本。

功能包里面也可以有src，放这个包自己的代码（和外面的src不是同一层）。

### 功能包

包里面有两个重要文件：

- CMakeLists.txt：这个包怎么构建。
- package.xml：这个包叫什么，依赖哪些包等信息。

在工作空间的src里创建包（需要已经配置好ROS 1和catkin_tools）：

```bash
catkin_create_pkg demo1 std_msgs rospy roscpp
```

demo1是包名，后面是依赖。生成的是模板，具体功能还需要补代码和配置。

回到工作空间根目录，用`catkin build`构建。再执行`source devel/setup.bash`，让当前终端知道去哪里找这些资源（加载环境，不是重新编译）。

## 额外补充

### 链接

比如main.cpp里调用add，链接时先找到add的实现，把调用和实现对上，叫符号解析。各部分位置安排好后，再调整需要修改的地址或偏移，叫重定位。

- 静态链接：把需要的库代码放进链接结果。
- 动态链接：保留对共享库的依赖，在加载或运行时完成相关处理，所以运行时也要找得到库。

### ELF与虚拟地址

程序文件里面不只有代码，还有数据、符号表等。ELF就是组织这些内容的一种格式。

比如.text放机器指令，.bss记录需要清零的数据所需的空间等信息（文件里不用真的存一大串零）。

程序运行时用虚拟地址，可以理解为每个进程自己看到的一套地址。系统和硬件再管理它与实际存储的关系，所以地址数字相同，不一定用的是同一块物理内存。
