# 程序编译过程

笔记：[知乎笔记](https://zhuanlan.zhihu.com/p/476697014)

### 完整生命周期

![生命周期](https://i-blog.csdnimg.cn/direct/2fcb249d483249aca1508d17974ab2dc.png)



#### 1. 预处理

```shell
#常见错误，找不到文件
g++ -E main.cpp -o main.i
```

-E expand 扩展，对应预处理的功能，把头文件和宏定义展开，查找并包含文件文件



#### 2. 编译阶段

```shell
#两种方式，可以从预处理完成阶段编译也可以直接利用源代码
#常见错误，语义错误或者语法错误
g++ -S main.i -o main.s
g++ -S main.cpp -o main.s
```

把源代码词码语法语义分析，理解 C++ 的变量、类型、循环、函数等，完成优化，决定用哪些 CPU 指令实现，并且翻译成汇编代码，检查无误后给到汇编

```
C++ → 编译器：决定用哪些机器指令
汇编 → 汇编器：把这些指令编码成二进制
```



#### 3. 汇编

```shell
#这里为c是历史遗留问题，为compile
g++ -c main.s -o main.o
g++ -c main.cpp -o main.o
```

将汇编代码翻译成机器码，二进制执行的指令，接收已经选好的 CPU 指令，主要负责把它们编码成机器码，并生成目标文件。



#### 4.链接

```shell
g++ main.o -lA -lB -o main
g++ main.o -lA -lB             #不写后面的则默认生成a.out文件
```

链接过程负责把各个目标文件和库组合起来，并确定代码之间的引用关系，生成最终可执行文件。

链接静态库过程中，依赖者在被依赖者前面：要先看到“需要什么”，再去后面的静态库里“找什么”。



##### 4.1 静态库

生成静态库过程，静态库需要自己独立完成编译汇编之类的过程，主函数`main.cpp`编译的时候与静态库无关

```shell
g++ -c add1.cpp add2.cpp main.cpp         # 预处理、编译、汇编
ar rcs libA.a add1.o add2.o               # 打包成静态库
g++ main.o libA.a -o program              # 链接
./program
#ar :归档工具，用来创建、修改归档文件。由于历史遗留，不加 `-`。
#r（replace）：把文件加入归档；如果已有同名成员，就替换它。
#c（create）：表示预期可能需要创建归档，创建时不输出提示。即使没有 c，归档不存在时通常也会创建。
#s：创建或更新符号索引，方便链接器查找函数、变量由哪个目标文件提供。
```



##### 4.2 动态库

```shell
#position independent code 位置无关代码,不依赖构建到内存中的绝对地址 
#PIC 会利用相对寻址、间接寻址等方式，让代码适应不同的加载地址。    
g++ -c -fPIC add1.cpp add2.cpp
g++ -shared -o libB.so add1.o add2.o
#macOS为.dylib   Linux为.so  Windows为.dll
g++ -c main.cpp
g++ main.o -L. -lB -o program    #-L.在当前目录搜索库 -l搜索B这个动态库
./program
```

```shell
linux> gcc -shared -fpic -o libvector.so addvec.c multvec.c
linux> gcc -o prog21 main2.c ./libvector.so
```

上面的和下面的等效(这些`.o ` ,`.c ` 位置什么的都不重要，重要的是后面跟着什么，比如说`-o prog21`指的是生成`prog21`，先后顺序不重要)

```shell
gcc -o main2.o -c main2.c
gcc -o prog21 main2.o ./libvector.so 
```



动态链接过程：

![动态链接过程](https://pic4.zhimg.com/v2-d05c07ac0018c398a259161b07854243_1440w.jpg)





# cmake

## 1.基本语法

#### 1.1最低版本

`cmake_minimum_required(VERSION 3.10)`

#### 1.2 声明工程的名称  

 `project(project1)`

#### 1.3 声明生成的目标文件和目标文件所依赖的代码文件  

`add_executable(main main.cpp test1.cpp test2.cpp)`  

或者使用变量替换

```cmake
#搜索当前目录下的源文件，将文件列表保存到 SRC_LIST 变量
aux_source_directory(. SRC_LIST)
add_ececutable(main ${SRC_LIST})
```

#### 1.4 执行cmake命令

```cmake
$mkdir build
$cd build
$cmake ..(CMakeList.txt文件所在的路径)
$make
$./main
```



## 2.cmake的使用

### 2.1 注释

#### 2.1.1 注释行

Cmake用#注释

```cmake
#这是一个cmake文件
cmake_minimum_required(VERSION 4.2.3)
```

#### 2.1.2 注释块

使用`#[[]]`进行块注释

```cmake
#[[这是
1
2
3]]
cmake_minimum_required(VERSION 4.2.3)
```

### 2.2 变量

#### 2.2.1 定义变量

定义变量使用`set`

```cmake
set(SRC_LIST add.c div.c main.c)
#取变量的值
add_executable(app ${SRX_LIST})
```

#### 2.2.2 指定使用的c++标准

1. 通过set

   ```cmake
   set(CMAKE_CXX_STANDARD 11)
   #cmake_cxx_standard(标准)
   ```

2. 在执行cmake命令的时候指定这个宏定义的值

   ```cmake
   cmake CMakeLists.txt文件路径 -DCMAKE_CXX_STANDARD=11
   ```

#### 2.2.3 指定输出的路径

   指定可执行程序输出的路径，对应一个宏，叫做`EXECUTABLE——OUTPUT——PATH`，他的值通过`set`设置

```cmake
set(HOME /home/robin/Linux/Sort)            #定义一个变量存储路径
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)     #设置给宏
```

   这样之后生成的程序就会保存在这个路径里面，如果路径不存在会被自动生成出来，路径推荐使用绝对路径

### 2.3 库

#### 2.3.1 添加库

```cmake
# 语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
# STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件    static
# SHARED：动态库（.so / .dll），运行时加载               shared
# 不指定类型时，由 BUILD_SHARED_LIBS 变量决定
add_library(MyLib STATIC library.cpp)
```

#### 2.3.2 链接库

public库具有传递性

private在链接时不具备

```cmake
#a可以链接b,c，但是b,c给不到d，字母前面不写PRIVATE就默认PUBLIC
target_link_libraries(a PRIVATE b PRIVATE c)
target_link_libraries(d a)
```

基本语法：

```cmake
# 语法：target_link_libraries(<目标> <库>...)
# 将指定的库链接到目标（可执行文件或其他库）
# 可以链接自己项目中的库目标，也可以链接外部库
target_link_libraries(MyApp PRIVATE MyLib)
```

**动态库和静态库的区别**：

1. 静态库：链接时，把需要的库代码放进最终可执行程序。生成程序后，运行时就不需要原来的静态库文件了。  
2. 动态库：库代码保存在独立文件中。程序运行时加载动态库，调用函数时执行动态库中的代码，因此运行时仍需要动态库文件。




#### 2.3.3 为目标指定头文件路径

```cmake
# PUBLIC：当前目标和依赖它的目标都能使用该路径
# PRIVATE：只有当前目标能使用
# INTERFACE：只有依赖当前目标的其他目标能使用
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```

### 2.4 日志

命名为`message:`，打印变量的值的时候可以通过 `${}` 取出

* `(无)`：重要信息
* `STATUS`：非重要信息
* `WARNING`：警告信息，继续处理
* `AUTHOR_WARNING`：面向项目开发者的警告信息
* `SEND_ERROR`：错误信息，继续处理，但不生成构建文件
* `FATAL_ERROR`：致命错误，立即停止处理

### 2.5 安装路径

```cmake
# 语法：install(TARGETS <目标>...
#         [RUNTIME DESTINATION <可执行文件安装路径>]
#         [LIBRARY DESTINATION <动态库安装路径>]
#         [ARCHIVE DESTINATION <静态库安装路径>]
#         [INCLUDES DESTINATION <头文件安装路径>])
# 定义执行 make install 时各类型文件的安装位置
install(TARGETS MyApp RUNTIME DESTINATION bin)
```

```cmake
add_executable(main main.cpp)

# 将 main 安装到安装目录下的 bin 文件夹
install(TARGETS main DESTINATION bin)

# 将头文件安装到安装目录下的 include 文件夹
install(FILES mylib.h DESTINATION include)

#再执行安装：
$cmake --install build --prefix ./output
#[[安装后的结构类似：
output/
├── bin/
│   └── main
└── include/
    └── mylib.h]]
```



# catkin

[笔记](https://www.autolabor.com.cn/book/ROSTutorials/chapter1/15-ben-zhang-xiao-jie/151-roswen-jian-xi-tong.html)

![](https://www.autolabor.com.cn/book/ROSTutorials/assets/%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F.jpg)

1. catkin 工作空间 

   对`cmake`的扩展，组织和管理功能包的文件夹

2. catkin_make 

   建立工作空间

   ```shell
   mkdir -p ~/catkin_ws/src
   cd ~/catkin_ws/
   catkin_make
   ```

   要回到工作空间再catkin_make不然会失败

   ```shell
   cd ~/catkin_ws
   catkin_make
   source ~/catkin_ws/devel/setup.bash   #用source刷新环境
   ```

   * `src` :存放package源代码
   * `build `:`cmake` ,`catkin`缓存和中间文件
   * `devel` ：目标文件

3. package

   `catkin`编译的基本单元

   至少得有`CmakeList.txt`,`package.xml`

   `package.xml`：更改的基本为`build_depend,run_depend`

   `launch及其配置文件`：一次性运行多个可执行文件

4. 常用指令

   * `rospack`

     ```shell
     #查找某个pkg地址
     rospack find package_name
     #列出本地所有pkg
     rospack list	
     ```

   * `roscd`

     ```shell
     #跳转到某一个pkg路径下
     roscd package_name
     ```
     
   * `rosls`
   
     ```shell
     #列举某个pkg下的文件信息
     rosls package_name
     ```
     
   * `rosed`
   
     类似vim
   
     ```shell
     #编辑pkg中的文件
     roscd package_name file_name
     ```
     
   * `catkin_create_pkg`
   
     ```shell
     #创建一个pkg
     catkin_create_pkg<pkg_name>[deps]
     ```
   
   * `rosdep`
   
     ```shell
     #安装某个pkg的依赖
     rosdep install [pkg_name]
     ```
   
     
   
     

# 大模型使用思考

对于大模型的使用，我最深刻的理解就是他作为辅助工具非常优秀，以及他能够给我很多思路，比如说对一个新东西新任务，不知道如何下手的时候，询问他提供一些方向等等，下面是我的一些思考。

1. 学习过程：大模型作为学习的助手可以加深我们对知识的了解，比如说面对全新的知识，看b站的视频往往会有很多自己的思考，以及`为什么不可以这样做`，当自己有创新的思维或者与原文有出入的理解的时候，可以直接跟ai交流非常方便，而且他可以跟你把问题的本质聊清楚，把你问题的出发点聊明白，每次学完一个知识但是我有点莫宁两可的时候，就可以把自己对知识的理解讲述给ai，让他帮我纠错，比如说下图。

   ![6ffb6b2dc078c68f615c06c7f5b2a23a.png](https://picui.ogmua.cn/s1/2026/10/25/6addf625cbb98.webp)

   而且其实在学习完成之后，其实会有遗忘且笔记没有涉及到的地方，这个时候去翻网课无疑是大海捞针，可以直接询问他，或者有些网课没有涉及到的东西，比如说网课没有讲到的。

2. 使用过程：其实使用ai这么长时间我最深刻的感受就是ai始终还是作为辅助工具，我有尝试过让ai全程接管写过两块ti板之间的通讯，也有用过他去通过`HC-05`蓝牙模块连接单片机和小车，实现实时调试过程中把数据传给ai，然后ai来自己调参数，最后确实有还算可以的效果，但是所花费的时间其实也许不如自己来的快，而且后续再次调试，还是要寄托希望给他，过程很枯燥，且完成没有属于自己的成就感，如果是自己全程跟进让他来辅佐，为我提供思路或者优化我的方案的话，以后面对相同问题，后续我的理解也会更加深刻，也会更得心应手。还有就是有些时候ai做的也不一定是最优方案，比如说前段时间遇到`clion`里面`jertbrains`不给使用的问题，跟他拉扯了很久，他能通过日志检测出代理问题，但是解决过程很混乱，且耗费很长时间，其实去`csdn`上面寻找一些攻略，或者根据他所说的代理问题，自己尝试去思考也可以自己推测出其实是账户设置地域可能出现问题，更改地域地址就能解决，如果纯靠他可能需要非常长时间。真正遇到一些问题我还是更喜欢去搞懂并且加上自己的思考而不是纯靠他copy。











































