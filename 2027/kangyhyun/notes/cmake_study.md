#DynamicX课堂笔记
#日期2026.10.22
#智能感知工程 康宸斌

#Cmake笔记

**注意：project为可执行程序名称，my_libs为我创建的目录，该笔记中会记录g++如何工作，Makefile、Cmake、Catkin是什么和怎么工作的**

#**这些东西是什么**
在代码程序编写中，g++、Makefile和Cmake是三种编写工具。

1.若将编写比作盖房子，g++是一块一块砖头盖即用g++编译每一个文件，当文件数量多起来时，g++则会显得繁琐且容易出错。
2.Makefile则相当于包工头，根据提供的图纸（Makefile这个纯文本文件）进行编写。
3.Cmake：Cmake相当于总设计师，根据提供的需求说明书（CmakeList.txt)，自动根据当前使用的系统，帮忙画出Makefile能执行的图纸。本质上就是一个自动化的生成图纸的工具，省去了制定符合使用系统格式的图纸的时间和工作。
4.Catkin是ROS（机器人操作系统）里面的构建系统，是rosbuild的替代者，是基于Cmake进行拓展，为了方便管理ROS的众多依赖包和复杂工程结构的系统。

#一、g++怎么编写文件

**单个文件编写**：`g++ main.cpp -o project` 
`-o`代表输出（output)，`project`贼是生成的可执行程序的名称。`-o`如果缺失，则会默认生成一个a.out的文件

**多个文件编写**:`g++ main.cpp test.cpp -o project`

**头文件搜索**：`g++ main.cpp test.cpp -I./include -o project`
`-I`(include)即去`./include`这个目录中找寻头文件。

**链接外部库**：
1.系统自带的库：`g++ main.cpp -lm -o project`
-l(link)表示链接，对于系统自带的库如`lib`则去/user/lib等标准目录中去找，-lm则是链接`libm.so`（数学库）。
2.自己下载的非系统自带的库`g++ main.cpp -L./my_libs -limage -o project`
-L则是去指定目录即我创建的`my_libs`目录中去找`libimage.so`文件。
**补充**：在Linux系统中，库文件通常以lib开头，以.so（动态库）或.a(静态库）结尾。`libm.so`中实际是lib+m+.so组成。

**调试与优化**：`g++ main.cpp -g -o project`
`-g`表示在可执行文件中加入调试信息，方便用gdb调试时查看源码。
##补充gdb相关信息：
###1.gdb是什么
gdb（GNU Debugger即GNU调试器）可以称之为程序运行是的“X光机”用于随时暂停程序来查看程序中变量的变化。
###2.gdb常用指令
启动调试：gdb ./main
设置断点：（gdb) break 10
运行到第10行时让它停止
运行程序：（gdb) run或者r
单步执行：
执行下一行代码，当遇到函数时不进去：（gdb) next或n
会进入函数：(gdb) step或s
打印变量的值：（gdb） print 变量名或写为（gdb） p 变量名
退出：（gdb） quit或q

#二、Makefile如何工作
**注意：g++这行代码前面一定用tab缩进**
如：
`makefile`
```
my_project:main.cpp
    g++ main.cpp -o my_project 
 clean:
    rm -f my_project
 install:
    cp my_project /usr/local/bin/
```
##1.在目录中找Makefile或makefile这个图纸并默认访问里面的第一个文件。
##2.若只想要默认则输入`make`，若要指定则输入`make clean`这样跟文件名。
##3.查找依赖 如`my_project`后面冒号接的便是该文件的依赖`main.cpp`。
##4.比对修改时间 ：跳过未修改文件，直接执行修改文件节约时间（编译大型项目快速的主要原因）。

##工作特性
###1.可使用变量替换
如‵ａ = g++`后面就可以使用$(a)来代替g++这一个编译名称。
###2.具有模式化规则
如：

```
#最终执行目标：生成依赖main.o和math.o生成my_project这一可执行文件
my_project:main.o math.o
    g++ main.o math.o -o my_projrct
#将所有.cpp文件生成.o文件
%o: %.cpp
    g++ -c $< -o $@
#清理规则:删除所有.o文件和最终的可执行程序（可用于清理编译过程中编译失败后产生的.o垃圾文件）
clean：
    rm-f *.o my_projrct
```
第一行表示任意名字即如果需要生成.o文件则去寻找.c文件生成。`%`表示任意名字，如果make发现需要main.o时，`%`就是main。
第二行表示不管遇到哪个.cpp都用g++编译成.o文件。`$<`和`$@`叫做自动变量（专属于Makefile）。`$<`代表第一个依赖文件（如上文的main.cpp)，`$@`代表目标文件（如main.o）。

#三Cmake如何工作
##1.建立CmakeLists.txt文件
```
# 1. 声明要求的 CMake 最低版本3.10
cmake_minimum_required(VERSION 3.10)

# 2. 声明项目名称
project(MyProject)

# 3. 添加可执行文件（目标名 源文件）
add_executable(my_program main.cpp math.cpp)
```
**注解**：`project（Myproject）`中Myproject会被存入一个Cmake的名为PROJECT_NAME内置变量中。
如：
```
project(MyProject) 
add_executable(${PROJECT_NAME} main.cpp) 
# 这里的 ${PROJECT_NAME} 就自动替换成了 MyProject
```
**好处**：如果想更改项目名称只用更改第一行，大大节约了时间和降低了出错的可能性。
##2.建立文件夹build
```
mkdir build
cd build
```
**注意**：在源码目录下建立文件夹————外部构建，使源码目录保持干净。
##3.在文件夹里执行命令
```
# 第一步：让 CMake 根据上一级目录的 CMakeLists.txt，生成 Makefile
cmake ..

# 第二步：调用 make 包工头，执行真正的编译工作
make
```
###Cmake的核心语法
1.set(）：定义变量
2.include_directories():告诉编译器去哪里找头文件（.h) -> g++ -I。
3.target_link_libraries():告诉链接器链接什么库 -> g++ -l。
4.find_package():自动补全所需的头文件和库文件路径，大大节约了时间和事情的繁琐性。

#资料补充

##一. https://zhuanlan.zhihu.com/p/476697014
**程序编译的整体流程（预处理、编译、汇编、链接）**
1.该篇文章中提到GCC编译器驱动程序的四个阶段的顺序是预处理 → 编译 → 汇编 → 链接。中文里“编译”经常被用来指代整条流水线，但严格说它只是第二步：把 `.i` 变成汇编 `.s`。汇编再把`.s`变成机器码目标文件`.o`，链接最后把多个`.o`和库拼成可执行文件。可以认为预处理是**文本展开**，编译是**翻译成汇编**，汇编是**翻译成机器码**，链接是**拼装和地址解析**。

###预处理阶段：
1.删除define，展开宏定义。
2.处理预编译指令。
如：
```
#define PI 3.14
area = PI*r*r；
```
`PI`会被替换成3.14
3.删除注释。
4.保留#pragma编译器指令。

###编译阶段：
1.词法分析 ->即认词，将代码拆解做好记号，哪些是变量，哪些是常量。
2.语法分析 ->检查句子结构，有可能会因为漏写分号和括号未闭合导致报错`Syntax Error`。
3.语义分析 ->理解代码含义，检查是否有调用了的函数未定义等等，报错`Type mismatch`。
4.源代码优化与生成 ->分析优化减少代码运行时间，使代码运行更顺畅，最后生成可执行文件。

`gcc -E main.c` 默认不输出 `.i `文件，而是把预处理结果直接打到终端。要输出 `.i`文件（预处理阶段后产生的文件），必须显式写 `gcc -E main.c -o main.i`。这里很多人把“预处理”想象成一个自动落盘的过程，实际上 gcc 中加入`-o `才能把中间产物固化下来。有没有 `-o`文件，决定了你是仍在观察还是在产出可执行文件 。

###汇编阶段
1.将代码（`.s文件`）翻译成机器语言指令，将这些指令放入（`.o文件`)。
2.相较于编译阶段的代码多对一优化，汇编只会一对一的死板翻译。
3.此时的`.o文件`只是半成品没法直接运行。

###链接阶段
1.符号解析：去系统库里找需要调用的函数。
2.重定位：对于每个.o文件进行座位分配，使最终文件能够正常运行。
3.链接有静态链接和动态链接。
（1）静态链接：将代码复制，优点：不需要考虑电脑是否具有对应的库，哪里都能运行。缺点：但文件数据庞大，占用内存多。
（2）动态链接：不拷贝代码，记录运行需要的库。优点：数据小，执行快。缺点：只记录了程序要用的库，如果未安装则直接报错，移植性差。

##二.https://www.runoob.com/cmake/cmake-basic.html
###1.开篇点出Cmake的本质是构建系统生成器 。
###2.文章里提到了最基本的三个指令：

cmake_minimum_required：必须放第一行，声明最低版本。
project(MyProject)：定义项目名，顺便把项目名塞进全局变量。
add_executable(main main.c)：告诉 CMake，我要把 main.c 变成一个叫 main 的可执行文件。这本质上就是把命令行里的 gcc main.c -o main 给“声明化”了。

###3.作用域与依赖传递（PUBLIC / PRIVATE / INTERFACE）

**文章对比了两种写法**：
老写法：include_directories(...) —— 这是全局的。只要写了，所有子目录、所有目标都能看到。很容易造成“头文件互相污染”。
新写法：target_include_directories(...) —— 只作用于特定目标。更关键的是它带了三个修饰词：
**假设我开了公司售卖产品：
PRIVATE（私有）：这头文件是我自己用的，谁依赖我，我都不给他看。即特殊的工具生产我的产品。
PUBLIC（公开）：我自己要用，依赖我的人，我也允许他们用。即使用产品配套设备。   
INTERFACE（接口）：我自己不用，但谁依赖我，我就把这路径传给他（典型场景：纯头文件库）。（**询问ai得知，可以将其理解为我自己不用它，但是买我的产品的人必须用**）
  
###4.缓存变量机制
文章提到了普通变量和缓存变量（CACHE）。
普通变量每次运行 cmake 都会重新算；但缓存变量会被写进 CMakeCache.txt，跨运行持久化（把数据写入硬盘，跨越程序运行的生命周期）。
这就是为什么很多人在 GitHub 上抄了别人的项目，改了 CMakeLists.txt 里的默认配置，结果死活不生效。因为之前跑过一次，缓存文件里已经存了旧值。遇到这种问题，删掉 build 目录。

###5.依赖
通过调用find_package找寻对应脚本，然后将库的头文件路径和库文件路径赋给一群变量。

示例（来源于菜鸟教程）
```
# 文件路径：项目根目录/CMakeLists.txt

# 1. 指定 CMake 最低版本（必须放在最前面）
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称、版本和语言
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# 3. 设置 C++ 标准为 C++17，并强制要求使用该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 添加静态库目标
add_library(MyLib STATIC
    src/lib/core.cpp      # 核心功能实现
    src/lib/helper.cpp    # 辅助函数实现
)

# 5. 添加可执行文件目标
add_executable(MyApp
    src/main.cpp           # 程序入口
    src/utils.cpp          # 工具函数
)

# 6. 为库目标指定公开头文件路径
#    PUBLIC 表示使用 MyLib 的目标也能访问该路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 7. 为可执行文件指定私有头文件路径
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 8. 将 MyLib 链接到 MyApp
target_link_libraries(MyApp PRIVATE MyLib)

# 9. 设置安装规则
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```

##三.https://www.mihaileric.com/The-Emperor-Has-No-Clothes/
该篇文章未完全理解，等完全理解后进行补充。

##四.https://wiki.ros.org/catkin/Tutorials






## 附录：我在实操与大模型交互中的“真实提问记录”


**感悟**：AI 可以在一秒钟内给我正确的代码和原理，但它不能代替我手敲终端。正是这些少空格、大小写错误、引号不闭合的“坑”，才让我真正记住了 Linux 的命令行规则。记录下这些真实的踩坑过程，远远比直接复制一段完美的代码更有价值。

在完成这两次作业的过程中，我遇到了非常多的疑惑和报错。我没有直接让 AI 替我写作业，而是把 AI 当作“导师”，遇到不懂的概念和终端报错就去提问。以下是我与 AI 交互的真实思考脉络：

### 一、 基础语法与工具的启蒙
1. **关于脚本头**：我问 AI `#!/bin/bash` 是什么意思？AI 告诉我这叫做 Shebang，是告诉系统用哪个解释器来运行脚本，这是写 Shell 脚本的“身份证”。
2. **文本处理**：不懂 `wc` 和 `tail` 怎么用。AI 解释后我明白了，`wc -w` 可以统计字数，而 `tail -n 10` 可以读取历史记录的最后10行，完美解决了作业输出“最近10条命令”的需求。
3. **编辑器的区别**：我问 AI，刚才打开的 `nano` 和课上讲的 `vim` 是一样的吗？它们都是终端里的文本编辑器，但 `vim` 有普通、插入、底行三种模式，操作更硬核也更高效。

### 二、 构建工具（Makefile & CMake）的底层逻辑
1. **为什么需要 CMake**：一开始我完全不懂 `cmake` 是什么。通过 AI 的“盖房子”比喻，我理解了 `g++` 是泥瓦匠，`Makefile` 是施工图纸，而 `CMake` 是自动画图纸的设计师。这就解释了为什么我们要用 CMake 来管理大项目。
2. **编译的细节**：
   - 我问了 `g++` 的常见编译例子，特别是在链接外部库时，`-l`（指定库名）和 `-L`（指定库路径）的具体用法。
   - 搞懂了 `g++ -c` 参数的意思是“只编译不链接”，它是实现 Makefile 增量编译的核心。
3. **Makefile 工作原理**：
   - 我很好奇 Makefile 的“图纸”到底怎么工作？原来它是通过比对**源文件和目标文件的“时间戳”**，来实现聪明的增量编译（只编译改过的文件）。
   - 询问 `%.o: %.cpp` 这种模式规则，它是自动处理大量源文件的流水线模板。（详情请看上文）

### 三、 真实的终端“翻车”与 Git 避坑实录
在实操中，我遇到了很多报错，都是 AI 帮我分析和排查的（这也是我收获最大的地方）：
1. **脚本没有显示时间**：排查后发现是 Linux 严格区分大小写，我把重定向写成了全大写的 `OUTPUT_FILE`，导致内容写错文件了。
2. **nano 编辑器的卡死**：在合并代码时弹出了 `.git/MERGE_MSG` 界面，我不知道怎么退出。AI 提醒我按 `Ctrl+O` 保存，回车确认，然后按 `Ctrl+X` 退出。
3. **Git 同步与冲突**：当团队更新了 GitHub 的 `TASK.md` 时，我学会了用 `git fetch upstream` 拉取，用 `git merge upstream/master` 合并。并且学会用 `git stash` 暂存我还没写完的笔记，合并完再用 `git stash pop` 拿出来。
4. **低级失误手滑**：
   - 把 `git add .` 敲成了 `git add.`（少了个空格），终端直接报错。
   - 因为少敲了一个双引号，终端一直卡在 `>` 提示符出不来，最后按了 `Ctrl+C` 强行中断。

**注意**：该附录使用ai帮我梳理行文思路，使逻辑更加清晰，用词更简洁。

