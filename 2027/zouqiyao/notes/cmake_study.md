## Docekr 
* 在一个镜像指定的环境里面来运行一个应用或者代码
* 镜像 就是一个模板 里面有运行环境 代码 等等
* 容器 就是一个模板的实例化 
### Docker 运行逻辑
* 用的是 client --  daemon 架构
* 客户端 写命令 发送 给 daemon daemon 通过容器执行 结果返回给客户
* daemon 监听着一个 .sock 文件 这个文件只可以root 和 docker group 读写 所以要将用户加入 docker 组

### Docker pull 从仓库下载镜像 
```
	docker pull docker.io/library/nginx:latest
```
1. docker.io 是DockerHub 注册表地址 
2. library 命名空间 （就是谁上传的）  library 就就是官方上传的
3. nginx 镜像的名字
4. latest 镜像的版本号

### Docker 命令 （如果没有加组 就 在命令前面加 sudo）
	docker images   列出所有下载过的镜像
	docker rmi  name/Id  删除某个镜像

	docker run name/Id 通过镜像创建一个容器 容器在跑的时候会一直刷log

	docker run -d  name/Id  d:detached mode 让容器在后台运行 不要阻碍终端
	
	docker ps 在运行的容器
## 高级语言编译原理
###
* 所有代码和程序最终 都要 变成一个可以执行的二进制文件 
* 以 .cpp 来举例 
```
hello.cpp --->  预处理(替换include<>  define  template<typename T> 等部分) --> hello.i
hello.i --->  编译器(编译成低一级的语言 : 汇编)  ---> hello.s
hello.s  ---> 汇编器 (将汇编文本翻译成 二进制文本)  ---> hello.o
hello.o  world.o(一个项目可能有多个文件来组成)  ---> Linker (将这些二进制文件拼装,有在运行前拼装好的，有在运行时拼装的)  ---> hello.sh(linux) / hello.exe(windows) executable 文件 可执行
```
* 所以你会发现，因为Linker的存在 ，我们只需要每一个程序的.o 文件 就可以拼接成为一个可执行文件 所以可以把每一个源文件分离编译--> 
* 在一个有很多源文件的项目里面 当你只改动了一个源码就可以只编译这个源码生成新的.o 文件 其他没有改动过的源码的.o文件也不需要改动

* 在Linux 上 gcc / g++ 编译器集成了 以上四个步骤 
## 虚拟内存
### 
* 产生原因： 物理内存是很宝贵的，一个需要10g内存的c++ 文件运行在 物理内存 只有 8g 的电脑上 就会崩
* 解决方法: 我物理内存里面只放正在调用的数据  没有调用的数据就先放在磁盘里 如果要用了再放进物理内存里面
* 物理页：一片内存空间
* 虚拟页: 假装这里有一片内存空间 malloc new 给数据分配的都是虚拟页的地址
* 虚拟页 通过页表（虚拟地址与真实物理地址的关系）来映射到 物理页上 虚拟页也可以不用映射到物理页上 这样就会出现“缺页”
* 代码在运行的时候 访问地址是访问 虚拟页的地址 同过MMU（这是一个硬件）查找物理地址 如果物理地址存在 那么虚拟页到物理页的道路就被打通了 代码就可以写入真实的物理地址了
* 当 MMU 通过页表查不到 物理地址的时候 也就是所谓的“缺页” 这时候 OS 就出来了
* 如果内存还有物理页帧 就 直接改页表 让虚拟页指向一个 物理页 
* 如果内存已经 满了 就要回收“脏页”（就是在内存中修改过的页） 将“脏页”写入磁盘（交换空间） 修改 页表 让脏页的虚拟页不能指向 物理页 这样就能腾出物理空间给 "缺页"的页
* 因为代码指向的地址都是 虚拟页上 的 地址 所以 每一个进程数据的地址看起来都是一样的 但是因为页表的不同 他们所映射到的物理地址是完全不同的

### 静态链接
#### 可重定位文件
1. 定义：.o 文件的 函数 和 变量 的地址 还没有确定
比如：有两个 cpp 文件 a ,b
```cpp
void guide()
{
	std :: cout << "guide" : <<Guide;
}
```
```cpp
int Guide;
int main()
{
	guide();
}
```
你会发现 a.cpp 中的 Guide 变量在文件中找不到  a.o中就会把这个 Guide 的地址空出来 等 Linker 填入

b.cpp 中guide() 函数找不到 b.o中就会把guide()地址空出来等待 Linker 填入

2. a.o 和 b.o 就是两个可冲定位文件

#### 符号表
1. 广义上的 符号 一个函数 一个变量名都可以称为一个符号
2. （跟上面定义）符号表记录了 这个符号 是否已经分配了地址 
3. 比如 a.o 的符号表 Guide 符号 就是未分配地址状态

#### 连接器的活动 
1. 将所有文件符号表格汇总成一个总表格 在没有分配地址的符号后分配地址

### 动态链接 
1. 加载: 将程序的代码 和 数据 从磁盘拷贝到内存里 
2. 将俩静态链接好的程序在加载之后链接起来

## Cmake 
###
* cmake 是用来生成 makefile 然后将 makefile 递给 make 
* make 再找到 makefile 然后编译 成一个 可执行文件
### 
	cmake_minimum_required(VERSION 3.11) 指定最低版本

	includ_directories(./includes) 头文件所在的位置 比如 include"hello.hpp" hello.hpp 在那里？"./includes"         ** 所有的地址都是 相对于CMakeLists.txt 所在文件夹的地址
 
	project(demo VERSION 1.0 LANGUAGES CXX) 项目名demo 版本1.0 编程语言c++
	add_executable()  给项目指定一个可执行文件的名字
	add_executable(demo src/cmake_learn.cpp tools/tool.cpp)  生成一个可执行文件叫demo + 它的源文件的地址 

### cmake 的变量
	aux_source_directory(./src SRC) 将src 这个文件夹 里 的所有 .c .cpp 等source 文件路径打包成 一个 SRC 变量
	add_executable(demo ${SRC} ./tools/tool.cpp) 这样我们就不用一个一个打 源文件的路径了

### cmake library 
###	cmake 终端 代码
	cmake -B build cmake 生成一个名为 build 的文件夹 将makefile 放入这个文件夹里
	make -C build make转到build 这个文件夹里  找到makefile 编译生成可执行文件
### 静态库 
* 静态库 里面放的是 ： .o 文件
* 静态库的含义是：将编译出来的.o 集中放进入 一个仓库里面 当要生成可执行文件的时 Linker 就会在 library 里面找需要的.o文件 l连接成一个可执行文件
* 动态库的含义是：将将编译出来的.o 集中放进入 一个仓库里面 当要执行可执行文件的时 Linker 就会在 library 里面找需要的.o文件 一边执行一边连接
* 静态库当 library 被删除的时候 可执行文件依然可以执行 因为已经链接完毕 所有的文件都处于可执行文件中
* 动态库当 library 被删除 可执行文件爱你不可执行 因为运行需要链接时 找不到文件

```
	#新建一个静态库
	add_library(mlib STATIC tools/tool.cpp)
	#告诉linker 这个静态库 可以在其中找到 需要的.o 文件
	target_link_libraries(可执行文件名 PRIVATE mlib)
```

## ROS noetic
* 它是一个以节点为基础 的 分散式的系统  就是不同的功能代码分别在不同的节点下运行 通过 建立通讯来进行数据交互
* 好处：不同功能的代码可以在不同的硬件环境下跑  不需要一个硬件跑完  大大提高效率
* 每个节点都会完成一个 事情 比如导航
### ros 环境
	source /opt/ros/noetic/setup.bash 执行setup.bash 脚本
	这个脚本会干什么呢？

	setup.bash 主要设置这些环境变量：
	变量	作用
	PATH	把 /opt/ros/noetic/bin 加进路径，这样你才能直接敲 roscore、rosrun、roslaunch、rosnode 等命令
	ROS_ROOT	指向 /opt/ros/noetic/share/ros
	ROS_PACKAGE_PATH	ROS 找包的路径
	PYTHONPATH	让 Python 能找到 ROS 的 Python 模块
	LD_LIBRARY_PATH	让程序能找到 ROS 的动态库
	ROS_MASTER_URI、ROS_HOSTNAME 等	网络相关（有的脚本会设默认值）

	注意奥， 这些变量都处于 当前bash 进程的内存里   (也就是说 你新开一个 bash 也就是新开一个终端 这些变量都会被清0重置  这时候就有要再跑一次setup.bash 脚本)

	但是 我写了自己的 包(可以理解为一个函数) 或者 我修改了ros本身的包  我想让ros 运行我的包该怎么办呢？
	这时候catkin就上场了
		
	你的包放在 /root/catkin_ws/src/my_pkg/（含 package.xml）
			│
			│ catkin_make 编译
			▼
	devel/setup.bash 生成，设置 ROS_PACKAGE_PATH 包含 /root/catkin_ws/src   (catkin 会自动更新 setup.bash 这个脚本哦！ 跟我说：好聪明的catkin)
			│
			│ 你 source devel/setup.bash
			▼
	当前终端 ROS_PACKAGE_PATH = /root/catkin_ws/src:/opt/ros/noetic/share
			│
			│ 你敲 rosrun my_pkg xxx
			▼
	rosrun 去 ROS_PACKAGE_PATH 列出的目录里递归找名为 my_pkg 的 package.xml
			│
			▼
	找到 /root/catkin_ws/src/my_pkg，运行它

	所以！ ros系统自带了一个 setup.bash 它在/opt/ros/noetic/setup.bash 这里
		  你的工作空间由catkin帮你管理了一个 setup.basn 它在  root/ros_ws/devel/setup.bash (如果你的工作文件夹叫做ros_ws) 这里

	你要干的事情： 
		source /opt/ros/noetic/setup.bash 将一些 ros包和命令路径 放进当前的 bash 内存里 当你对bash 说 rosrun 的时候 它就会找到rosrun 这个命令 指向的可执行程序
		source /root/ros_ws/devel/setup.bash  将你的包 路径 放到 bash 的内存里面

	注意奥! 一定要先 "source ros" 再 "source 你的工作空间"

	原因：setup.bash 在写入bash 内存里的变量的时候 是在头部插入 比如: ROS_PACKAGE_PATH=新路径:$ROS_PACKAGE_PATH   # 把新路径放在前面
	bash 在查找包的时候 会找到 第一个路径 （第一个路径找到了也就不再找了）
	如果你改过了某一个 package 或者 你的 package 和 ros 的package 同名  你想让ros运行你工作空间的包的...那么你就得让 "source 工作空间" 在 "source ros" 后面
### 节点管理器 ROS Master
* 为节点提供命名和注册服务
* 跟踪和记录话题/服务通信，辅助节点互相连接，查找
* 提供参数服务器  一个全局的对象字典  变量名：变量值 可供每一个节点读写
