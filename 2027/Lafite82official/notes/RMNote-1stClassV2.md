# 第一节课笔记

## 学习目标

1. Linux 基础
2. CLI 文本编辑器
3. Git 与 GitHub PR

---

## 1. Linux

### 1.1 Linux 简介

区别于日常办公、游戏常用的 Windows，以及擅长多媒体开发的 macOS，Linux 是一种在服务器、嵌入式控制领域常见的专业开发平台。其特点是性能友好、进程透明、开发者可操作性极强，因此常用于机电与机器人控制、视觉等领域。发行版中，Ubuntu 与 ROS 生态兼容性最佳。

DX 团队的机器人使用 Linux 开发板（上位机）作为唯一主控计算单元，较少使用单片机做简单控制。开发板安装 Ubuntu 20.04 Headless Server 版操作系统和 ROS1 Noetic 控制程序，调试时通过 SSH 内网连接 CLI，全部程序使用 C++ 语言编写。

### 1.2 Linux 终端 CLI 基础

命令行中：

- `~`：当前用户家目录
- `.`：当前目录
- `..`：上一级目录
- 目录可视为文件夹

示例：

- `/home/user/example`：稳定的绝对目录
- `./hello.txt`：从当前目录开始向后索引
- `~/example`：直接索引家目录下的文件夹

`/` 是系统级根目录，以其开头的绝对路径含义如下：

- `/home/user`：用户文件
- `/etc`：系统配置文件
- `/usr`：程序与库文件
- `/var`：日志等变量/可变数据文件
- `/dev`：内外部设备读取和交互用的文件，例如 CSI 镜头和 DSI 屏幕

终端是 Linux Headless 的唯一交互界面，负责计算机系统内核和用户之间的输入输出可视化。Shell 是命令解释器：Windows 用 CMD 和 PowerShell，Linux-Ubuntu 用 Bash，负责解释脚本命令并交给系统内核执行。通常在终端 CLI 输入 Bash 脚本指令和程序/库自定义指令，以启动程序并交互。

### 1.3 常见指令与 Bash 基础

常见指令：

- `pwd`
- `cd xxx`
- `ls -la`
- `mv`
- `rm`
- `mkdir`
- `cp xxx xxx`
- `code .`：用 VS Code 连接远程 SSH，并打开当前目录为工作空间
- `git clone`

Bash 指令结构包括：声明变量于本终端、重定向、管道、逻辑关系式。常用快捷键：`Tab` 补全、`↑`/`↓` 复用指令。

---

## 2. CLI 文本编辑器

可使用 Vim，或直接用更友好的 `code .` 命令一键呼叫 PC 端 VS Code，提供 Remote-SSH 实现远程交互连接。

---

## 3. Git 与 GitHub PR

### 3.1 Git 与 GitHub 简介

Git 提供本地版本管理与控制，可记录每次提交、比较修改、查看历史、为新功能创建代码库分支。

GitHub 提供远程在线代码托管与团队协作，在线托管 repository 代码仓库，通过 PR 向上级提交新的代码修改与添加并审核。

### 3.2 首次 PR 配置

```bash
git config --global user.name "Your Name"
git config --global user.email "you@example.com"
git config --get user.name   # 查询客户端名字
```

### 3.3 基本流程与常用命令

基本流程：

```text
工作区 → add → 暂存区 → commit → 本地仓库 → push → GitHub 服务器
```

常用命令：

- `git clone`
- `git status`
- `git diff`
- `git log --oneline`

### 3.4 分支相关概念

- Fork：仓库副本
- Branch：仓库内的开发线
- feature：Git 中的功能开发分支，叫什么名字都行。这里以 `git switch -c feature_zhangzhiyuan1020` 为测试功能分支

### 3.5 执行脚本

```bash
# cd 进 homework 后可直接执行本文件夹下的文件
# 如果用 bash ShellScript1020-1.sh 的话就不需要授权脚本执行权限
# 类似于 python3 xxx.py
chmod +x ShellScript1020-1.sh
./ShellScript1020-1.sh
```

### 3.6 常见问题：为什么输出的是“前 10 条”而不是“最近 10 条”？

这是个非常经典的问题！

之前用的是：

```bash
tail -n 10 ~/.bash_history
```

`~/.bash_history` 这个文件是在终端关闭时（或者定期）才会把你敲过的命令存进去。所以当前终端里刚刚敲的命令（比如 `bash`、`tree`、`mkdir`），其实还在内存里，还没有被写入 `.bash_history` 文件。读取这个文件时，自然读到的是“老早以前”（还没通过 `code .` 跳进 VS Code Remote 交互环境并新建终端）的旧命令。

over