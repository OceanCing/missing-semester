## vim: 应用
    :q 退出 :q!强制退出（不保存）
    :wq 保存后退出
	/name  向下查找name
	?name 向上查找name
	按 n 键上方下一个name所在地
	shift + n 下方下一个 name 所在地
	yy 复制这一行的内容
	3yy复制本行与下两行的内容
	p在光标后换行粘贴
	dd 删除本行
        dw 删除这个当前单词和后面空格
	:1,3s/name1/name2/ 在1到3行找到行内第一个name1 换成 name2
	:1,3s/name1/name2/g 在1到3行找到所有name1 换成 name2
	按esc 退出 插入模式
	.vimrc 配置vim 
	:set number 显示所有行数
	:set nonumber 不显示

## 终端常用命令:
  ###
	ls 列出目录下的内容
	ls -a 列出隐藏的文件 (.name) 文件都是隐藏的
	ls -l 列出详细信息 
	ls -h 显示文件的大小
	ls -t 按照修改时间排序
	ls -r 逆序显示
	ls -lha 详细 显示文件大小的 打印所有文件 (参数可以组合)
   ###
	链接： 软连接(类似于快捷方式)  硬连接（类似于拷贝）
	ln -s name1 name2  生成一个软链接文件name2 指向 name1(name2 指向name1)
	ln name1 name2 有点像引用 name1 和 name2 同时指向同一个i节点(也就是文件真正存在的地方)
	修改name2 的同时name1 也会改 (他们都指向同一个i节点)
	软连接 可以指向目录或者文件  硬连接只能指向文件
   ###
	文件权限 -rwx-rw-rwx  r(read) w(write) x(可执行) 第一个‘-’不看三个一组  分别为user(文件所有者)  group（同组用户） other(其他用户)  的权限
	chmod +x name  给文件name 的other 加一个x权限 
	chmod -x name  给文件name 的other 去除一个x权限
	chmod u+x name 给文件name 的user 加一个x权限 
	chmod g+x
	chmod o+x 一样的
	chmod ou+x  o u g 可以一起用
	数字表示权限 r(4) w(2) x(1) 三个一组组成一个 0～7的数字
	chmod 776 name  name的权限就是   -rwxrwxrw-
	chmod 744 name  name的权限就是   -rwx-w--w- 
   ###
	touch 创建新文件 或 更新文件修改时间
	touch name  文件不保存在 新建
	touch hello.txt 碰一下 hello.txt 更新hello.txt的修改时间为现在
   ###	
	echo 用来输出内容
	echo "hello" 在终端里 打印 hello
	echo "hello" > hello.txt 将文件里的内容换成 "hello" （>重定向符号）
   ### 
	pwd 显示当前所在的位置
	cd bin 跳转到目录bin
	cd ..  跳转到父目录
	cd ../.. 上一个目录的 上一个
   ###	
	目录是树状结构的   根目录为 root -> home -> yao 这里是终端一般的入口
   ###
	cp file2.txt file3.txt 将file2.txt 复制一份 叫 file3.txt
   ###	
	mv file3.txt /home/bin/   移动file3.txt 到bin目录
	mv file3.txt file4.txt  将file3.txt 移动到当前目录 并且重命名为file4.txt 效果就是重名命
	mv file3.txt /home/bin/ file4.txt 将file3.txt 移动到bin目录 并重命名为file4.txt
   ###
	mkdir(make directory) 
	mkdir Folder 创建一个目录
	mkdir -p Folder1/Folder2/Folder3 创建Folder1->Folder2->Folder3 两两为父子关系
   ###
	cp -r 递归复制
	cp -r Folder Folder_copy  将会递归Folder 将它复制给 Folder_copy 
   ###	
	tree Floder 以数状图显示
   ### 	
	rmdir(rm directory)
	rmdir Floder 但是如果这个目录非空 会报错
   ###	
	rm -r Floder_copy 用递归删除 Folder_copy 里的所有文件
	Dangerous!!!  rm 的文件是找不回来的 
   ###
	ps 查看系统的进程
	ps -ef 查看系统的进程树
	UID          PID    PPID  C STIME TTY          TIME CMD  这个是命令输出的表头 
	UID	User ID	运行该进程的用户。显示用户名（如 root）或数字 UID。在容器里常见 root。

	PID	Process ID	进程 ID，系统给每个进程分配的唯一编号。PID 1 通常是容器的主进程（entrypoint/CMD 启动的）

	PPID	Parent Process ID	父进程 ID，即"谁启动了这个进程"。通过 PPID 可以追到父进程，形成进程树

	C	CPU utilization	CPU 占用率（ps -ef 里这一列叫 C，表示 CPU 使用百分比，是粗略值）

	STIME	Start Time	进程的启动时间。可能是时间（10:30）或日期（Oct25）

	TTY	Teletype	进程关联的终端，？表示没有关联终端（后台进程）；pts/0 表示某个伪终端

	TIME	CPU Time	该进程累计消耗的 CPU 时间（注意不是运行时长，是真正占用 CPU 的时间）。

	CMD	Command	启动该进程的命令（命令行）

## GIT
  ###
	git config --global user.name ""  初始化用户名
	git config --global user.email "" 初始化email
	git config --list 查看配置  
###
	git init  将当前文件夹目录初始化为仓库
	注意！：.git文件保存着所有版本快照..不要删除
###
	git 组成

* git 仓库有三个区 工作区 暂存区 仓库
* 工作区： 修改文件内容
* 暂存区： 暂存已经修改好的文件
* 仓库： 给管理的文件拍快照 生成一个新的版本
###
	git add. 将所有文件上传至暂存区
	git add name1.txt 将name1.txt上传到暂存区
###
	git commit -m "Discribe" 提交一次版本（快照），并描述修改的内容
	git commit 也可以 弹出vim 在第一行写描述 再:wq就好
###
	git status 查看仓库状态（三个区分别状态）
	git log 查看提交历史
	git log --online 用简短的方式查看历史提交
	git ls-files -c
	git ls-files --catched 列出所有被git追踪的文件
						在一次提交后，也可以理解为，这个版本快照里有什么内容
	git diff 检查工作区 和 暂存区文件的区别 (如果是第二次提交以后，版本一开始的暂存区存放着上一个版本年的所有“追踪文件”) 
###
	git reset --soft 退回到上一个版本，保留上一个版本的暂存区和工作区 (比如你在上一个版本新建了一个文件并且add了，reset --soft之后 你的工作区还有这个文件，暂存区也有)

	git reset --mixed  退回到上一个版本，保留上一个版本的工作区 (比如你在上一个版本新建了一个文件并且add了，reset --soft之后 你的工作区还有这个文件,但是暂存区被清,不知道那些工作区文件被修改，暂存区也没有新建的文件，也就没有被追踪)

	git reset --hard 退回到上一个版本,都不保留(上一个版本作的无论是工作区还是暂存区的修改都没有了) 完全是变成的裸体的上一个版本
###
	分支的意义:
* 将master里面的东西“拷贝一份”出来，成为一个分支
* 对分支进行修改不会对master 有任何影响 ，完全独立，master 不知道它干了啥
* 比如一个游戏 有人要写移动的程序 有人写环境的渲染 就分别创建一个分支 在master的基础上两人分别添加内容 最后合并到master 分支里 master的环境渲染 和 人物的移动 就搞定了

###
	git branch branch_name  新建一个名字为branch_name 的分支
	git branch 查看当前仓库所有分支
	git switch branch_name 跳转到branch_name 分支上
###
	合并分支
* 将分支上的修改内容 和 master 合并 产生一个新版本
* 会出现一个问题， 如果两个分支都修改了master的同一个文件而且修改的内容不同 就产生了冲突
### 
	git merge branch_name  branch_name 是要被合并的分支
	注意： 你要在将合并 branch_name 的分支下写这一行代码
	一般是 master....
###
	分支在合并之后不会消失
###
	git branch -d branch_name 删除已经合并的分支
	注意：你也要在合并了的分支下写这一行代码
	git branch -D branch_name 删除未合并分支
###
	git clone <> 将远程仓库复制一份到本地
	git push 将本地的新增的commit 推送到远程仓库
	git push -u origin branch_name 将本地分支的上游设置为origin
## Docekr 
* 在一个镜像指定的环境里面来运行一个应用或者代码
* 镜像 就是一个模板 里面有运行环境 代码 等等
* 容器 就是一个模板的实例化 
### Docker 运行逻辑
* 用的是 client --  daemon 架构
* 客户端 写命令 发送 给 daemon daemon 通过容器执行 结果返回给客户
* daemon 监听着一个 .sock 文件 这个文件只可以root 和 docker group 读写 所以要将用户加入 docker 组
```bash
	docker start ros_noetic
	#client 给docker 的 deamon 发送start 命令 docker 将ros_noetic 跑起来
	#docker exec -it ros_noetic bash client 给docker 的 deamon 发送exec命令 也就是excute 执行的意思的意思 

	docker exec -it ros_noetic bash 含义是：

    docker exec：在已运行的容器里执行命令

    -it：交互式 + 分配伪终端

    ros_noetic：目标容器名

    bash：要在容器里执行的命令
```
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

 	docker exec -it ros_noetic bash 在容器里面启动一个进程bash 将当前终端作为虚拟终端（假装是container 里面的终端）与容器交互
	
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


### 节点通讯
#### 话题通讯 Topic
* 单向传输 
* Publisher 将数据不断的 传输到 topic 上 
* Subscriber 订阅这个topic 不断拿到数据 
* Message 也就是传输的内容 有一定的类型和数据结构 包括ROS提供的标准类型和用户自定义类型  用.msg文件定义
* Message Type 也就是数据的结构
* Subscriber 有缓冲区 也就是一个队列 如果Publisher发得太快了 就将数据push in queue 等待回调函数处理(用来处理数据的函数)

#### 服务通讯 Service
* 使用客户端/服务器 (C/S)模型 ，客户端发送请求数据 ，服务器完成处理之后返回应答数据
* 比如： 图像处理节点 发送了一个 request （摄像机把分辨率调节至720p）  摄像机节点 将分辨率调节至720p 发送一个 Reply （调节成功）
* .srv文件定义请求和应答的数据结构
* 一个服务端可以被多个客户端 来进行request
* 一个节点作为服务端就会到 Master 那里注册一个Service名称 这个变量包括了 "那个节点发出的Service" "Request/Reply(如何调用这个Service)"
* 说人话就是：一个节点告诉Master 说 :"这里是我的电话号码（Service名称），你要以这样的格式给我打电话(Service名称里面包含的内容)" 


### 参数 Parameter
* ROS Master 拥有一个参数 服务器 共享的 多变量字典
* 节点可以通过网络来访问这个服务器 
* Talker --> 就是定义字典的节点  Talker: setParam("foo",1)   将“foo” 设置成为 int 1
* Listener --> 就是查阅字典的节点 Listener ： getParam("foo")  拿到"foo"这个变量 的值
* 节点用这个服务器来存储和检索 运行时的参数
* 比较适合静态的，非二进制的配置参数 ，动态的不太行
* 当Talker 将foo改成 2 Listener 不 getParam 就不知道 foo = 2   所以..你看动态参数不太行吧...

### 功能包
* ROS软件中基本的单元  包含节点源码 配置文件 数据定义 功能包清单(Package manifest)--> 记录功能包作者信息，许可信息，依赖选项，编译标志等
* 功能包里可以有很多不同的节点，小乌龟turtleism 功能包里 就有telepo_key键盘节点 turtlesim节点 他们被一起打包进入一个功能包
* 元功能包：包括多个用于统一目的的功能包 比如都是做视觉控制的： 有Camera 功能包 有数据处理功能包 有Camera驱动功能包 大家打包到一起就是 视觉元功能包

### ros常用命令
   #### roscore
	roscore 启动 ROS Master 可以提供“节点管理器 ROS Master” 以上服务
   #### rosrun
	rosrun <包名> <节点名>  rosrun turtlesim turtlesim_node  运行一个节点
	！！注意： 每一次运行rosrun 必须确保 ROS Master在 也就是 roscore --> rosrun 这个顺序不能变
	原因：启动一个节点就得 到 Master 那里注册自己 还有启动topic 如果没有 Master在 那么无法注册也就无法通讯了
	rosrun <包名> <节点名> 这个节点的运行会占用这个终端 你想运行其他节点得再开一个终端

   #### rosnode 
	  rosnode list 将当前ros 运行的节点 都列出来s
	  rosnode info <节点名称> 查看莫个节点的信息  Publish 了那些 topic Subscribe 了那些 topic 等信息
   #### rostopic 
	  rostopic list 打印出ros 里所有的话题 
	  rostopic pub 参数 <topic名称> <Message 结构> "具体Message"   给某个topic 发送message
	  rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist "linear:  (这里-r 是rate 的意思 -r 10 就是10Hz 一秒发布10次)
		x: 1.0
		y: 0.0
		z: 0.0
		angular:
		x: 0.0
		y: 0.0
		z: 1.0" 
	  Ctrl + C 就可以停下来
   #### rosmsg 
	  rosmsg <Message结构>  打印出这个数据结构
	  rosmsg geometry_msgs/Twist 查看 geometry_msgs/Twist 这个Message 的模板
   #### rosservice
	  rosservice list 打印出当前 Master 已注册的 service 名称
	  rosservice call <服务名> <Message 结构>  调用<服务名>service 并获得 reply
	  rosservice call /swamp "x: 2.0 y: 2.0 theta:0.0 name 'turrle2' " 给turtlesim 发一个Request 然后让它 reply
   ####	rqt_graph  
		rqt_graph 图形化查看节点 和节点之间的联系 （比如两节点之间有一个topic）
   #### rosbag (!!!!!)
		rosbag record -a -O cmd_record   记录系统的所有数据 -a的意思是all -O cmd_record 的意思是 将系统的数据都放在 cmd_record 里 （注意，如果没有写路径就默认放在终端所在的文件夹里）
		rosbag record -a -O /root/catkin_ws/cmd_record.bag 最好把cmd_record.bag 放在挂载点上 (宿主看得见) 注意写绝对路径

		工作原理: subscribe 所有的topic 加上时间戳 写入 cmd_record.bag 里面  在play 开始的时候 将 cmd_record.bag 里面的message pub 到各个topic 上
		还有： play 的时候 如果有节点在向 topic 同时发消息 topic 遵从先到先发的原则 虽然只有一条信息通道 会按顺序将俩方的message 发出去

### ROS 中的工作空间
   #### 工作空间的 结构
		src : 放置所有的源码 
		install : 安装空间
		devel: 开发中放可执行文件和 库
		build:	中间文件
   #### 创建一个功能包
		在 /root/catkin_ws/src 里面
		catkin_creat_pkg <功能包名字> <depend1> <depend2> <depend3>
		catkin_creat_pkg test_pkg std_msgs rospy roscpp   我的功能包名字叫作 test_pkg 它依赖 std_msgs
   #### 知识补充！ 什么是“依赖”
	1. 包想要正常运行 或者测试 必须要从 系统库或者别的包里面 拿东西! 比如： include<roscpp> 你在编译的时候需要这个库  需要某个包的.launch文件
	2. .launch 文件就是 : 一个XML 格式的自动脚本 比如:你在运行这个包的的时候 要连接 另外一个包的动态库 这个库由5个节点 生成 如果你需要..就要手动打开5个终端..这太让人难过了        .launch 就可以帮你一键启动5个节点！（.launch文件里面写了启动命令） 
	3. 什么是 XML格式文件? eXtensible Markup Language 可扩展的 标记 语言 一种用自定义标签来描述结构化数据的文本格式

	4. 在那里写 depend? 在生成 package 的时候 catkin_creat_pkg test_pkg std_msgs rospy roscpp 

	5. 它是如何工作的？ 在catkin_creat_pkg 的时候 你写的<depend1> <depend2> ... 会被写入package.xml 这个文件里面 
	6.package.xml 只是一个 声明 别人在调用这个包的时候可以看到你有那些依赖
	7. 一般的依赖: <build_depend>roscpp<build_depend/> 构建依赖 ，在我代码编译的时候需要的东西  <exec_depend>std_msgs<exec_depend/> 运行时依赖 在我代码运行时需要用到的东西  <buildtool_depend>catkin<buildtool_depend/> 在我构建系统本身时候的依赖
	8. 这些依赖都被写入package.xml 里面 不过 这只是一个声明 ... 告诉别人我要用到这些
	9.真正 让代码能用上这些个依赖的 是 CMakeLists！ 在它编译代码的时候 它会 find_package(catkin REQUIRED COMPONENTS roscpp) 就是去查找这个依赖的真实路径
    10. 那它是如何查找的呢？ >> 它会去找 CMAKE_PREFIX_PATH 
	11. CMAKE_PREFIX_PATH 是什么呢? 是一个环境变量！！！！ 通过 source 来配置的环境变量 它记录了系统库的root层级的地址(说人话就是 定位到东莞但是定位不到那个街区) catkin 会自己在这个地址下找这个“系统库”的 
	12. 如果在我自己的工作空间 里面 source 那么我工作空间的 root层级地址 会插入CMAKE_PREFIX_PATH 的头部优先查找 
	13. 人话就是 如果 一个代码要依赖 roscpp 刚好我在工作空间里改了 roscpp 我不想用系统的 roscpp  source 系统的setup.bash 再 source我自己工作空间 的setup.bash 那么 我工作空间的root层级地址就会插入 CMAKE_PREFIX_PATH 的头部 被优先查找
	14. 那么 这个代码用到的就是 我自己改的 roscpp 了
### 撰写一个 Publiser
```cpp
# include <ros/ros.h>  // 包含了ros命名空间  还有里面的自由 函数 如 ros:: init()
# include <geometry_msg/Twist.h> 其实.h 文件才是c++语言 Twist 是 geometry包里面的一个 Twist.msg 文件格式 只不过catkin 在编译时 将 Twist.msg 写成了 c++的结构体 Twist 放在了 Twist.h 这个文件里面  这里个代码里的 Twist 都是一个结构体

int main(int argc, char **argv)  //atgc 这个是什么呢 这个记录了 终端在调用这个节点时的 字符个数
                                 //atgv 这个是一个字符串数组 记录了终端在调用这个节点时的 字符串内容
                                 // 比如 终端输入 rosrun learning_topic turtle_publisher 这个命令时 argc 就是3  
                                 //argv[0] 就是 “rosrun“ argv[1] 就是 “learning_topic“ argv[2] 就是 “turtle_publisher“

{

    ros_init(argc,argv,"turtle_publisher"); //首先roscpp 这个库是一个整个ros系统都可以调用的全局函数库
                                            //"运行状态" 就是 在节点运行起来以后 roscpp里面的函数在内存里 维护的 一堆全局变量 
                                            // 这些全局变量 对于当前进程是可见的 对于其他进程（跑起来的节点）是不可见的

                                            //package.xml <build_depend>roscpp<buile_depend/> 还有 CMakeLists.txt 里面的 find_package(catkin REQUIRED COMPONENTS roscpp ) target_link_libraries()
                                            //它们是在告诉 catkin 这个包在编译的时候需要roscpp 这个库
                                            //include<ros/ros.h> 就是将roscpp 这个库里的函数 加载进来
                                            //当我们在代码里调用 roscpp 里面的函数比如 ros::init() 就会用到roscpp里面写好的init()
                                            //ros::init() 干了点什么？ 在这个进程里面 维护了 一堆全局变量           
                                            // 这些全局变量 有什么呢： ros Master的地址 "turtle_publisher" 这个节点名 还有这个节点的IP地址 端口号(tcp监听用)
 

    ros:: NodeHandle n;  // NodeHandle 是一个类 它用来干什么呢？
                         // 首先 在这个代码跑起来的时候 我们已经 创建了一个节点！ 我们还想将这个节点 作为publisher 注册到ros Master 
                         // 我们可以如何做呢? 我们可以自己 向ros Master 注册... 自己写函数 ..自己传参数... 
                         // 晕死...老大每次我们都要自己写函数注册节点吗？ 这太麻烦..
                         // 所以我们就需要一个roscpp 里的"奴隶" 一个NodeHandle 类对象 
                         // NodeHandle 里面封装了向rosMaster 注册节点 声明成为publiser 或者 声明成为 subscriber 的函数
                         // 所以每次我们想注册节点到 rosMaster 就可以用 NodeHandle 的对象... 让它去干
    ros::Publisher turtle_vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
                         // 首先n.advertise() 是NodeHandle封装的一个 函数 用于 在Master 上注册一个 topic
                         //n.advertise<geometry_msgs::Twist> 这是一个模板函数 <geometry_msgs::Twist> 是这个topic 需要发布的数据类型
                         //("/turtle1/cmd_vel/",10) 传入参数告诉Master我这个topic 的名字叫做 /turtle1/cmd_vel/ 发布的队列缓冲区是10个数据
                         //ros::Publiser 类 跟 NodeHandle类一样 也是一个奴隶...
                         //n.advertisr() 只是告诉在Master 上注册了 一个 topic 
                         //当要向这个 topic 发送mssage 的时候我又得找到这个topic 的地址 ，找到这个topic 的数据类型..等等 再将它们以参数的形式写入mssage 发送函数
                         //上帝，这实在是太麻烦了 ... 我用一个类来管理这个 topic 不就好了？
                         // 聪明！ Publisher 这个类就是用来管理 这个 topic 的 ，它里面有这个topic 的名字 数据类型 发送端口等等. 还封装了发送函数
                         //所以我们每次要在这个topic 上发消息就叫 一个 Publisher 类来帮忙!
    ros::Rate loop_rate(10);
                         //有参构造了一个 Rate 的实例化对象 loop_rate "10"代表10Hz
                         //Rate 对象里面装着什么 ? :上一次sleep()的时刻 还有一个sleep 函数
                         //如果调用loop_rate.sleep() 这个函数就会查看当前时刻和上一次sleep时刻的时间差距 如果时间差距小于100ms 就在当前代码行等到时间差距到达100ms
                         // 如果超过了 就马上运行下一行
                         // 整体效果就是 每一次 .sleep() 之间的时间间隔必须大于等于100ms 也就是 .sleep()之间调用频率不超过10Hz
    while(ros::ok())     // 调用 ok()函数 会返回当前节点的状态(是否还在运行)(是否被退出）
                         // 当在这个节点的终端 按下 Ctrl ^C 的时候，  或者在代码里调用 ros::shutdown() 的时候 这个节点的状态就会变成 false
                         // 节点状态为 false 再次调用 ros::ok() 就会输出 false
                         //ros::ok()有什么作用呢？ 
                         // 你可以在节点状态为 false 之后 调用一些析构函数 之后再main()return 0 可以优雅的释放内存
    {
        geometry_msgs::Twist vel_msg; //实例化一个消息对象
        vel_msg.linear.x = 1.0;
        vel_msg.angular.z = 1.0;  //设置运动参数
        turtle_ver_pub.publish(vel_msg);//叫 /turtle/cmd_vel 这个话题的管理员turtle_ver_pub过来  让它去发布vel_msg 这个message 到这个topic 上

        ROS_INFO(“Linear: %f  Angular %f”,vel_msg.linear.x,vel_msg.angular.z) //这个函数就是将某些数据打印到终端上
                                                                              // 就是c 语言风格的 printf一样的写法 用 %d %f %c 作为占位符
        POS_INFO_STREAM("Linear:" << vel_msg.linear.x << "Angular:" <<vel_msg_angular.z) //这个也是将数据打印到终端上
                                                                                         // 就是 c++ 的 cout <<" " << 一样
        loop_rate.sleep();// 这里就调用了 loop_rate.sleep() loop_rate 里记录了上一次 sleep()的时刻 和当前时刻的差如果 小于 100ms (含参构造的时候传进去的10 代表10Hz 也就是1s 10次 每次间隔100ms)
                          // 那么代码就停在这里 等到 100ms 再继续
                          // 如果时间大于 100ms 就直接放行
    }
    return 0; //main()退出
}
```
### 撰写一个subscriber
```cpp
# include<ros/ros.h>
# include<turtlesim/Pose.h> //Pose 是turtlesim 包里面的一个 pose.msg 文件 它记录着 ”海龟位置“的信息结构
                            //我在CMakeLists.txt 里就已经写好了 我要依赖的包了 在这个包里面就有 Pose.h这个文件 所以编译会过..
void callback(const turtlesim::Pose::ConstPtr &msg) //回调函数 这个函数的参数是一个指针 指向 Pose 结构体
                                                    // ConstPtr 就是 shared_ptr<const turtlesim::Pose> 
                                                    //&msg 是一个引用 
                                                    // 所以callback 函数传入的是一个 Pose 结构体的智能指针
{
    ROS_INFO("x:%f,y:%f,theta:%f",msg->x,msg->y,msg->theta); //打印出海龟的位置
}

int main(int argc,char **argv) //依旧读取终端 输入命令
{
    ros::init(argc,argv,"my_subscriber"); //初始化ros节点 跟 Master 报道 将自己的 ip 地址和端口号告诉 Master 让 Master 记录下来
    ros::NodeHandle n; //创建 奴隶
    ros::Subscriber sub = n.subscribe("turtle1/pose",1000,callback); //订阅topic turtle/pose 1000个队列长度 
	// Subscriber sub 就是 一个管理通路这个通路的对象 里面主要包括了 一个 impl_指指向一个 SubscriberImpl类 
    // 这个SubscruberImpl 类里面记录了 这个通路的名字 用来解析数据的helper_ 还有一个nodehandle
    // 过程是 subscriptionQueue 收到数据
	//向 CallbackQueue 写入任务 （注意 CallbackQueue 也是一个队列）
	// 然后 ros::spin() 这个函数会去 CallbackQueue 里面取出任务 执行
	// 利用ros::Subscriber sub 这个对象里 impl -> Impl 里面的 helper_ 解析subscriptionQueue 里的数据 
	// 解析后的数据 放在一个 堆上的临时 Pose 结构体里 然后把这个临时的 Pose 结构体的地址传给 callback 函数
    ros::spin();
    return 0;
}
```
### 撰写一个Client
```cpp
#include<ros/ros.h>
#include<turtlesim/Spawn.h>//发送请求的格式封装在turtlesim包 的 Spawn.msg里面

int main(int argc ,char ** argv)
{
    ros::init(argc,argv,"turtle_spawn");
    ros::NodeHandle n;

    ros::service::waitForService("/spawn"); //这是堵塞型函数 如果没有找到 /spawn service 存在 就会一直等待

    ros::ServiceClient add_turtle = n.serviceClient<turtlesim::Spawn>("/spawn");
    //n.serviceClient() 函数是向 Master 查询一个 名为 /spawn 服务地址 然后返回给 add_turtle
    // 我们就可以通过 add_turtle 来调用 spawn 这个service 了
    //这里的ServiceClient 是一个本地客户端 和 NodeHandle 一样 通过它来发送 call 通过它来受到 resopnd
    turtlesim::Spawn srv; //turtlesim 库里面 封装了 Spawn 这个类 是Api 标准格式
                          // srv 里存两东西 request 和 response 
                          // request 是 发送给 servicce 的消息
                          // service 执行完毕以后将 完成消息 塞进 response 里面
    srv.request.x = 2.0;
    srv.request.y = 2.0;
    srv.request.name = "turtle2";

    ROS_INFO("ready to call spawn");

    if( add_turtle.call(srv) ) //这是一个堵塞型的函数 一直等到 有回答数据 才继续下一行
    {                     // 如果call成功 add_turtle.call() 会返回1;
 
    ROS_INFO("Spawn turtle successfully [name:%s]",srv.response.name.c_str()); //这里name就是一个string 但是ROS_INFO 看不懂string
                                                                               //所以调用 c_str() 转换成char* 让ROS_INFO看懂
    }
    else{
        ROS_ERROR("Fail to call spawn");
    }
    return 0;
}
```

### 撰写一个Server
```cpp
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_srvs/Trigger.h>

bool pubcommand = false;

bool commandCallBack(std_srvs::Trigger::Request &req,std_srvs::Trigger::Response &res) 
                    //Client .call() 发过来的是一个标准的API(你也可以自己写)
                    //这个API是一个结构体 Trigger
                    //Trigger 包含了一个 request 结构体 和一个 response 结构体
                    //request 结构体是 Client 填好的请求的数据 比如说 Turtlesim::Spawn srv  srv.request.x = 1.0; srv.request.y = 0.2; 这里就写好了request 再通过 .call() 传过来
                    //resopnse 就是要传回去Client 的数据 比如说: response.name = "turtle2" Client 就可以受到一个 string 意思是turtle创建成功名字是 "turtle2"
                    
        
{
    pubcommand = !pubcommand; //受到一次call()就变换一次运动年状态
    res.success = true;
    res.message = pubcommand ? "start" : "stop";
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "turtle_command_server");
    ros::NodeHandle n;

    ros::ServiceServer command_service =
        n.advertiseService("/turtle_command", commandCallBack); //在这里会向Master 注册一个Service 
        // 分别告诉Master 这个Service 的 名字 还有发布这个 Service 的节点地址
        // 这个 Service 是这样运转的 :: 
        // 首先 有一个Client .serviceClient() 跟Master声明 "我要向这个Service 发call" Master 就会将Server 的地址返回给Client
         //Client 利用句柄 .call() 就将请求的 Api 发送给了 Server
         //Server 底层维护了一个 队列 CallBackQueue 
        //这个队列就会受到这个API 将它打包成一个等待 丢入 回调函数的数据
        //在 调用 ros::spinOnce()的时候 ros就会检查 CallBackQueue 还有多少个 没有丢进Callback 的数据 
        // 让程序中断 调用 回调函数 知道在这一刻之前的所有数据被运用完毕
    ros::Publisher Turtle_vel = 
        n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

    ROS_INFO("Ready to receive turtle command");

    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        ros::spinOnce();

        if (pubcommand)
        {  
            geometry_msgs::Twist vel_cmd;
            vel_cmd.linear.x = 0.5;
            vel_cmd.angular.z = 0.2;
            Turtle_vel.publish(vel_cmd);
            //这里讲一下小海龟的运动机制
            //当没有收到订阅的Twist 的 时候 计时开始
            //小海龟从/turtle1/cmd_vel 里面得到Twist 数据 然后将自身的运动状态和Twist同步 计时清0
            //当计时达到1秒 小海龟不动
        }

        loop_rate.sleep(); //这里限制了turtle/cmd_vel 的发布速度最快是 100ms
                           //也限制了Server 接收数据的最快速度是 100ms 
                           //如果你再 100ms内 call 了 多次 那么这些call 都会被压缩
                           //同样的 如果你在1s内变化了两次 turtle的运动状态 turtle 就不会察觉你两次变化中的停顿
    }
    return 0;
}
```





   #### 定制你的个人的消息库!
   1. 定义msg 文件 Person.msg
   ```
	 string name 
	 uint8 age
	 uint8 sex

	 uint8 unknown = 0
	 uint8 male = 1
	 uint8 female = 2
   ``` 
   2. 这是 msg 的原始数据 ros 会将它编译成 c++ 的 .h 文件
   ```cpp  Person.h
	struct
	{
		string name;
		unsigned int age;
		unsigned int sex;

		enum{
			unknown = 0,
			male,
			female
		}
	}
   ```
   3. 在package.xml 里面声明功能包依赖
   <build_depend>message_generation</build_depend>
   <exec_depend>message_runtime</exec_depend>

   4. 在CMakeLists.txt添加编译选项
   find_package( .... message_generation) //拿到生成工具
   add_msg_files(FILES Person.msg)  //登记 Person.msg 告诉generation 我这个文件要转化成 .h
   generate_messages(DEPENDENCIES std_msgs) // 开始转化命令 DEPENDENCIES std_msgs
										    // 的意思是 如果我的msg 里用到了 std_msgs 库里面的类型 确保这个类型已经被转化好了

   catkin_package(.... message_runtime)  //对外声明 依赖我这个包的其他包 也要依赖message_runtime 这个库链接


 #### Parameter
	1. Master 会 有一个全局的参数 列表
	2. 这个列表所有节点都可以看到

	```cpp
	应用ros 命令
	rosparam list //列出当前参数
	rosparam get /param_key               //列出名为param_key 的参数的值
	rosparam set /param_ket n             //修改这个param_key 的值
	rosparam dump param.yaml              // 将当前的参数列表 打包成一个 param.yaml 放在主文件目录下   .yaml 文件格式是 /param_key + parameter value
	rosparam delet /param_key             //删除某个参数
	rosparam load param.yaml              //加载这个文件进入 Master
    ```

 #### 在程序里面调用Parameter 
 ```cpp
 #include<ros/ros.h>
#include<std_srvs/Empty.h>
int main(int argc ,char** argv)
{
    ros::init(argc,argv,"Change_param");

    ros::NodeHandle n;

    int red,green,blue;
    ros::param::get("/turtlesim/background_r",red);
    ros::param::get("/turtlesim/background_g",green);
    ros::param::get("/turtlesim/background_b",blue);

    ROS_INFO_STREAM("r:"<<red<<" g:"<<green<<" b:"<<blue);

    enum{
        r = 255,
        g = 255,
        b = 255
    };
    ros::param::set("/turtlesim/background_r",r);
    ros::param::set("/turtlesim/background_g",g);
    ros::param::set("/turtlesim/background_b",b);

    ROS_INFO("SET R:%d  G:%d  B:%d",r,g,b );

    ros::param::get("/turtlesim/background_r",red);
    ros::param::get("/turtlesim/background_g",green);
    ros::param::get("/turtlesim/background_b",blue);

    ROS_INFO_STREAM("r:"<<red<<" g:"<<green<<" b:"<<blue);

    ros::service::waitForService("/clear");
    
    ros::ServiceClient pub_change = n.serviceClient<std_srvs::Empty>("/clear");

    std_srvs::Empty srv;

    pub_change.call(srv);

    ROS_INFO("OK");
}
```

