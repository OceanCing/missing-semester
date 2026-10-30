- .c 文件一次编译的完整过程

以 GCC 工具链为例，一个 `.c` 源文件到可执行文件的完整构建过程分为四个阶段。每个阶段由特定的工具完成，并产生特定格式的中间文件。

## 总览

| 阶段 | 核心任务 | 所用工具（GCC 后台） | 输入文件 | 输出文件格式 | GCC 停止选项 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 预处理 | 展开宏、包含头文件、处理条件编译 | `cpp` | `.c` | `.i` (预处理后的 C 源文件) | `-E` |
| 编译 | 词法/语法/语义分析、优化、生成汇编 | `cc1` (C) / `cc1plus` (C++) | `.i` | `.s` (汇编语言源文件) | `-S` |
| 汇编 | 将汇编指令翻译为机器指令 | `as` | `.s` | `.o` (可重定位目标文件，ELF 格式) | `-c` |
| 链接 | 合并目标文件、解析符号、重定位、链接库 | `ld` (常通过 `collect2` 调用) | `.o` + 库 | 可执行文件 (ELF 格式，如 `a.out`) | 无（默认） |

> **说明**：GCC 驱动程序本身只是外壳，它会根据参数去调用上述后台程序。对于 C 语言，负责编译的核心程序是 `cc1`。



---


## 1.make 与 Makefile

- make 是一个"构建工具"（build tool），本质上是一个可执行程序。
- Makefile 是一个文本文件，用特定语法描述"目标（target）依赖什么、怎么生成"。


想象我们要编译一个由几十个源文件组成的 C/C++ 程序。如果手动敲命令，得先编译每个文件，再链接，一旦改了某个文件，又要重新判断哪些需要重编。make 就是帮我们自动化这件事的命令行工具。

make在正式工作之前，需要读一份叫 Makefile 的说明书。我们必须在 Makefile 里写清楚：哪个目标文件依赖哪个源文件、用什么命令编译。make 就会根据文件的修改时间来判断，只重新编译那些改动过的部分，然后自动链接出最终程序。所以 make + Makefile 是 Unix/Linux 下最经典的构建方式，高效但写起来比较繁琐。

- Makefile核心语法：
```
目标: 依赖
	命令
```

- 例子：

```bash
app: main.o utils.o math.o
	gcc main.o utils.o math.o -o app

main.o: main.c
	gcc -c main.c -o main.o

utils.o: utils.c
	gcc -c utils.c -o utils.o

math.o: math.c
	gcc -c math.c -o math.o
```

注意：命令前面必须是 `Tab` 键，不是`空格`（这是最坑爹的地方）。


## 2.CMake

真实项目的 Makefile 往往会用到变量、通配符、函数等，虽然 make + Makefile 一套组合拳能简化过程，但仍有以下问题：

    语法晦涩，Tab 缩进坑人，跨平台差（Windows 上 make 非常难用）。

    不同平台、不同 IDE（VS，Xcode）各有各的工程文件格式，Makefile 只适合 Unix 风格。

    依赖库、头文件路径管理起来仍然麻烦。

于是 CMake 出现了。

- CMake 不是编译器，也不是 make 的替代品，它是"构建系统生成器"（build system generator）。

- 关键理解：CMake 不直接编译代码，它生成 Makefile（或别的构建文件如project），再由 make 去编译。

CMake 解决的痛点：

    跨平台：同一份 CMakeLists.txt，在 Linux 上生成 Makefile，在 Windows 上生成 VS 工程，在 Mac 上生成 Xcode 工程。不用为每个平台重写构建脚本。

    语法更"高级"：不用手写每个 .o 的规则，声明"我要一个可执行文件，源码是这些"就可以。

    生态好：大量第三方库（OpenCV、Qt 等）都提供 CMake 支持，find_package 一行搞定依赖。

对同一个项目文件我们只需写一个CMakeLists.txt：

```cmake
cmake_minimum_required(VERSION 3.10) #注明最低版本

project(MyApp)                       #注明项目名称

add_executable(myapp main.cpp utils.cpp math.cpp) #指定依赖
```

在任意平台执行即可：

```bash
cmake -B build      # 在 build 目录生成构建文件（一般在"外部构建"）
cmake --build build # 调用底层构建系统完成编译
```

接下来CMake就会自动处理帮我们生成东西，这样就很舒服了。


## 3.Catkin 
- Catkin是 ROS 官方使用的构建系统，它本质上就是 CMake 的一套宏和扩展。（CMake的ROS特供版🤓）

因为 ROS 项目通常由很多个“包”（package）组成，包之间互相依赖。Catkin 在 CMake 的基础上加了一些规则，能让我们在一个工作空间里一次性按正确顺序编译多个互相依赖的包，还能自动处理环境变量，让我们编译完直接就能运行程序。在 Catkin 项目里，同样会看到 CMakeLists.txt，只是里面调用了 Catkin 特有的函数。

- 一个简单流程：

    ↓ 自己写的文件：ROS 包的 CMakeLists.txt 和 package.xml

    ↓ catkin 介入：catkin_make（或 catkin build）扫描工作空间
  
    ↓ 调用 CMake：根据每个包的 CMakeLists.txt，在 build/ 目录生成Makefile

    ↓ 调用 make：执行生成的 Makefile，完成编译链接
  
    ↓ 产物：可执行文件/库放入 devel/（或 install/）


catkin 本质上是“封装器”：
catkin_make 这一个命令，实际上就是调用了 cmake 和 make。如果不用它，也可以自己在工作空间的 build 目录里手动执行 cmake ../src 然后 make，效果是一样的，但是比较繁琐。catkin 的价值在于：它会自动递归查找 src/ 下的所有 ROS 包，按依赖顺序安排编译，并设置好 devel 空间等默认路径。

与普通 CMake 项目的区别：
普通 CMake 项目通常一个 CMakeLists.txt 对应一个项目，在 build/ 里 cmake .. 然后 make。而 catkin 工作空间里有很多个包，每个包都有自己的 CMakeLists.txt。catkin_make 一次性处理整个工作空间，把所有包“合并”配置和构建。

