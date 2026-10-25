第一节课笔记。
本节课学习目标：1.Linux基础、2.CLI文本编辑器、3.Git与Github PR

1.Linux
区别于日常办公游戏用的Windows和擅长多媒体开发的MacOS，Linux是一种在服务器、嵌入式控制领域常见的专业开发平台。特点是性能友好、进程透明、开发者可操作性极强，故常用于机电与机器人控制与视觉领域。发行版中尤其Ubuntu与ROS生态兼容性最佳。

DX团队的机器人使用Linux开发板（上位机）作为唯一主控计算单元，较少使用单片机做简单控制。开发板安装Ubuntu20.04 Headless Server版操作系统和ROS1 Noetic控制程序，调试通过SSH内网连接CLI，全部程序使用C++语言编写。

1.1 Linux 终端CLI基础
命令行中，~ 是当前用户家目录，.是当前目录，.. 是上一级目录。目录可视为文件夹。
例子：/home/user/example是稳定的绝对目录，./hello.txt是从当前目录开始向后索引，~/example直接索引家目录下的文件夹。
/是系统级根目录，以其开头的以下绝对路径含义：/home/user用户文件，/etc系统配置文件，/usr程序与库文件，var日式等变量/可变数据文件，/dev为内外部设备读取和交互用的文件，比如CSI镜头和DSI屏幕。

终端是Linux Headless的唯一交互界面，其负责计算机系统内核和用户之间的输入输出可视化。Shell壳体是命令解释器，Windows用CMD和PowerShell，Linux-Ubuntu用Bash，负责解释脚本命令并交给系统内核执行。通常在终端CLI输入Bash脚本指令和程序/库自定义指令以启动程序并交互，常见指令有pwd、cd xxx、ls -la、mv、rm、mkdir、cp xxx xxx、VS Code/git等程序定义的code .（用VS Code连接远程SSH并打开当前目录为工作空间）、git clone等。

Bash指令结构：声明变量于本终端、重定向、管道、逻辑关系式。常用快捷键Tab补全和↑↓复用指令。

2.CLI文本编辑器
Vim或者直接用更友好的 code . 命令一键呼叫PC端VS Code提供Remote远程交互连接。

3.Git与Github PR
git提供本地版本管理与控制。记录每次提交、比较修改、查看历史、为新功能创建代码库分支。
github提供远程在线代码托管与团队协作。在线托管repository代码仓库，通过PR向上级提交新的代码修改与添加并审核。

首次PR：
git config --global user.name "Your Name"
git config --global user.email "you@example.com"
git config --get user.name #查询客户端名字

工作区→add→暂存区→commit→本地仓库→push→Github服务器

常用命令git clone、status、diff、log --oneline

Fork 是仓库副本，Branch 是仓库内的开发线。

**feature是git中的虚拟开发分支，叫什么名字都行，这里以git switch -c feature_zhangzhiyuan1020为测试功能分支**

执行脚本：
'''bash
# cd进homework后可直接执行本文件夹下的文件
# 如果用 bash ShellScript1020-1.sh 的话就不需要授权脚本执行权限
# 类似于python3 xxx.py
chmod +x ShellScript1020-1.sh
./myscript.sh
'''

为什么输出的是“前 10 条”而不是“最近 10 条”？
这是个非常经典的问题！
你之前用的是 tail -n 10 ~/.bash_history。

~/.bash_history 这个文件是在终端关闭时（或者定期）才会把你敲过的命令存进去的。所以你当前终端里刚刚敲的命令（比如 bash、tree、mkdir），其实还在内存里，还没有被写入到 .bash_history 文件里。你读取这个文件，自然读到的是“老早以前”（还没通过 code . 跳进VS Code Remote交互环境并新建终端）的旧命令。