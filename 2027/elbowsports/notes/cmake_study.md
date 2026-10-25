

### Makefile
```makefile
main: main.o utils.o
	g++ main.o utils.o -o main

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -f *.o main
```

写了 Makefile 之后，改完代码只要敲一次 `make`。

### CMake

```
CMakeLists.txt  --cmake-->  Makefile  --make-->  可执行文件
```


```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)
add_executable(MyApp main.cpp)
```



### 常用指令

| 指令 | 作用 |
| --- | --- |
| `cmake_minimum_required(VERSION x.y)` | 最低版本要求 |
| `project(名字 CXX)` | 项目名和语言 |
| `add_executable(目标 源文件...)` | 生成可执行文件 |
| `add_library(目标 STATIC/SHARED 源文件...)` | 生成库 |
| `target_link_libraries(目标 库...)` | 链接库 |
| `target_include_directories(目标 作用域 路径)` | 头文件搜索路径 |
| `set(变量 值...)` | 定义变量 |
| `find_package(包 REQUIRED)` | 查找外部依赖 |
| `add_dependencies(目标 依赖)` | 显式声明构建顺序 |

### 构建流程

```bash
mkdir build && cd build     # 外部构建，把产物和源码分开（推荐）
cmake ..                    # 生成 Makefile
make                        # 实际编译
# 或者用 cmake --build . 代替 make（跨平台写法）
```

---

### Catkin工作空间结构

```
catkin_ws/
├── src/          # 源代码空间，放各个功能包
├── build/        # 构建空间，中间文件（一般不用管）
├── devel/        # 开发空间，编译产物 + setup.bash
└── install/      # 安装空间，make install 之后才有
```

### 常用命令

```bash
# 初始化工作空间
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace       # 在 src 下创建指向 toplevel.cmake 的软链接

# 编译
cd ~/catkin_ws
catkin_make
source devel/setup.bash     # 让环境找到刚编译的包

# 创建功能包
cd ~/catkin_ws/src
catkin_create_pkg 包名 依赖1 依赖2
```


---

## 六、用大模型辅助学习的过程记录

### 6.1 我实际问过的问题
我上个月用AI做过一个项目，用Chatgpt提供思路以及提示词，Workbuddy进行代码的编写，Deepseek进行检查，Muse生成图文和视频。对于AI的使用还是感触很深的。

### 6.2 被误导或者没用的经历
我在调试ESP8266时，AI就忘记了写中断和中断向量，导致我无法查看调试的状态，浪费了很多时间；
在最后生成策划书的时候AI没有按照我给的模板进行生成导致答辩的时候很难堪。

### 6.3 哪些地方 AI 帮到了我
在代码的编写上，AI确实很高效，省去了我大量的时间。

---

## 七、对「用大模型帮助学习」的思考
AI只能作为辅助，是帮助我们学习的工具，最后的检查和方案的敲定还得靠我们自己。
