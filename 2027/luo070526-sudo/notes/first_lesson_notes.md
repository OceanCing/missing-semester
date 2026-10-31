---
tags:
  - type/lesson
  - topic/linux
  - topic/git
  - topic/robomaster
  - topic/devops
  - status/done
date: 2026-10-29
---

# 第一课：Linux、Shell、Vim 与 Git

本文保留 Linux/Shell 命令和 Bash 脚本作为系统操作内容；涉及机器人节点、算法与控制逻辑的程序实现统一使用 C++。

这节课的主线不是背命令，而是掌握一套开发工作流：

```text
在 Linux 中定位文件
  → 用 Shell 执行操作
  → 用 Vim 修改文本
  → 用 Git 记录版本
  → 通过 GitHub Pull Request 协作
```

## 1. Linux 目录与常用命令

Linux 的所有文件都从根目录 `/` 开始。Ubuntu 是 Linux 的一种发行版，ROS、GCC、CMake 等控制组工具主要运行在 Linux 环境中。

| 符号 | 含义 |
| --- | --- |
| `/` | 根目录 |
| `~` | 当前用户的家目录 |
| `.` | 当前目录 |
| `..` | 上一级目录 |

`cd` 是 **change directory** 的缩写，用来切换当前目录：

```bash
pwd               # 显示当前所在目录
ls -lah           # 查看当前目录内容
cd ~/下载         # 进入家目录下的“下载”
cd ..             # 返回上一级目录
cd -              # 返回上一次所在目录
cd                # 不带参数时回到家目录
```

路径必须与真实名称一致。我的 Ubuntu 使用中文目录 `~/下载`，所以 `cd ~/Downloads` 会提示目录不存在。

常用文件操作：

```bash
mkdir -p demo/src
touch demo/src/main.cpp
cp demo/src/main.cpp demo/src/main_copy.cpp
mv demo/src/main_copy.cpp demo/src/test.cpp
rm demo/src/test.cpp
```

`rm` 删除的文件通常不会进入回收站。执行 `rm -r` 或带 `sudo` 的删除命令前，先用 `pwd` 和 `ls` 确认目标。

### 1.1 文件权限

`ls -l` 显示的 `r`、`w`、`x` 分别表示读取、写入和执行权限。

```bash
ls -l system_report.sh
chmod +x system_report.sh
./system_report.sh
```

没有执行权限时，也可以明确交给 Bash：

```bash
bash system_report.sh
```

## 2. Shell

终端是输入命令的窗口，Shell 负责解释命令。Ubuntu 常用 Bash。

一条命令通常由“命令名＋选项＋参数”组成：

```bash
ls -lah ~/projects
```

- `ls`：命令；
- `-lah`：选项；
- `~/projects`：参数。

### 2.1 重定向、管道与条件执行

```bash
date > output.txt
echo "done" >> output.txt
history | tail -n 10
mkdir build && cd build
```

- `>`：覆盖写入文件；
- `>>`：追加到文件末尾；
- `|`：把前一条命令的输出交给后一条命令；
- `&&`：前一条成功后才执行后一条。

每条命令都有退出状态。`0` 表示成功，非 `0` 通常表示失败：

```bash
echo $?
```

### 2.2 Shell 脚本典型结构

```bash
#!/usr/bin/env bash

note_file="notes/first_lesson_notes.md"

if [ -f "$note_file" ]; then
    wc -w "$note_file"
else
    echo "没有找到笔记"
    exit 1
fi
```

- Shebang 指定解释器；
- `[ -f ... ]` 检查普通文件是否存在；
- 路径变量加双引号，避免空格把路径拆开；
- `exit 1` 主动返回失败状态。

## 3. Vim

Vim 最重要的是分清模式：

| 模式 | 作用 | 进入方法 |
| --- | --- | --- |
| 普通模式 | 移动、复制、删除 | 打开文件后默认进入，或按 `Esc` |
| 插入模式 | 输入文字 | 按 `i`、`a` 或 `o` |
| 命令行模式 | 保存、退出、搜索 | 普通模式下按 `:` 或 `/` |

常用操作：

```text
i       进入插入模式
Esc     回到普通模式
dd      删除当前行
yy      复制当前行
p       粘贴
u       撤销
/word   搜索 word
:w      保存
:q      退出
:wq     保存并退出
:q!     放弃修改并退出
```

如果不知道当前在哪个模式，先按 `Esc`，再输入保存或退出命令。

## 4. Git 与 GitHub

Git 负责本地版本管理，GitHub 提供远程仓库、代码审查和 Pull Request。

```text
工作区 --git add--> 暂存区 --git commit--> 本地仓库 --git push--> 远程仓库
```

提交前的检查顺序：

```bash
git status
git diff
git add 2027/luo070526-sudo
git diff --cached
git commit -m "Complete first lesson homework."
git push
```

- `git status`：查看分支和文件状态；
- `git diff`：查看尚未暂存的修改；
- `git diff --cached`：查看即将提交的内容；
- `git commit`：保存一次有意义的版本；
- `git push`：把本地提交发送到远程分支。

### 4.1 本次 Fork 与 PR 工作流

```bash
git switch -c feature/luo070526-sudo-first-lesson
git push -u origin feature/luo070526-sudo-first-lesson
```

完整流程是：

1. Fork 原仓库；
2. Clone 自己的 Fork；
3. 在开发分支修改；
4. Commit 并 Push；
5. 向原仓库发起 Pull Request；
6. 后续继续 Push 到原分支，原 PR 会自动更新。

PR 已经建立后不应为了更新作业而关闭重开。

### 4.2 同步上游与处理冲突

```bash
git remote add upstream https://github.com/OceanCing/missing-semester.git
git fetch upstream
git switch master
git merge upstream/master
```

当两个分支修改同一文件的同一位置时，Git 可能留下：

```text
<<<<<<<
自己的内容
=======
上游内容
>>>>>>>
```

需要判断两边内容，手动合并并删除标记，再执行 `git add` 和 `git commit`。不能为了消除报错随便删除一边。

## 5. 本课实践与验证

```text
2027/luo070526-sudo/
├── notes/
│   ├── first_lesson_notes.md
│   └── cpp_basics_notes.md
└── homework/
    ├── system_report.sh
    └── output.txt
```

`system_report.sh` 把运行时间、最近 10 条命令和笔记字数写入同目录的 `output.txt`。这个练习对应真实工程中的“采集现场信息并留下可复查记录”：脚本是否成功，不能只看终端有没有报错，还要检查退出状态与输出文件内容。

提交前验证：

```bash
bash -n 2027/luo070526-sudo/homework/system_report.sh
chmod +x 2027/luo070526-sudo/homework/system_report.sh
./2027/luo070526-sudo/homework/system_report.sh
git diff --check
```

- `bash -n`：只检查脚本语法；
- `git diff --check`：检查多余的行尾空格等格式问题。

## 6. 本课形成的习惯

- 不确定路径时先用 `pwd`、`ls` 和 `find`；
- 不复制自己无法解释的高风险命令；
- 修改后先看 `git diff`，提交前再看 `git diff --cached`；
- 一次 Commit 只表达一件清楚的事情；
- PR 收到意见后继续修改原分支，不关闭重开。

## 7. 上车以后，Linux 命令解决什么问题

比赛机器人上的上位机通常没有一直连接显示器。代码部署、启动和排错经常通过 SSH 完成，因此真正需要形成的是“确认状态再操作”的习惯。

### 7.1 远程登录与保持任务

```bash
ssh dynamicx@192.168.10.10
hostname
pwd
whoami
```

假设车载电脑地址是 `192.168.10.10`。登录后先确认主机名、用户和目录，避免把命令敲到自己的电脑或另一台机器人上。

网络不稳定时，直接在 SSH 前台运行节点可能因断线而退出。可以使用 `tmux` 保存终端会话：

```bash
tmux new -s robot
# 在 tmux 中启动程序
# 按 Ctrl+B，再按 D，退出但不结束程序
tmux attach -t robot
```

`tmux` 只解决会话保持，不等于进程健康。重新连接后仍要检查节点、日志和数据是否正常。

### 7.2 进程、资源与日志

```bash
pgrep -af ros
ps -eo pid,stat,%cpu,%mem,cmd --sort=-%cpu | head
free -h
df -h
dmesg --level=err,warn | tail -n 30
```

| 现象 | 需要思考的原因 |
| --- | --- |
| 节点还在，但数据越来越慢 | CPU 被占满、回调阻塞或内存交换 |
| rosbag 突然停止 | 磁盘空间不足 |
| 相机或串口偶尔消失 | USB 断连、供电不稳或设备名改变 |
| 程序反复退出 | 启动参数错误、依赖缺失或硬件未就绪 |

现场日志应同时保留在屏幕和文件中：

```bash
mkdir -p ~/robot_logs
roslaunch dx_bringup robot.launch 2>&1 | tee ~/robot_logs/robot_launch.log
```

`2>&1` 把错误输出合并到标准输出，`tee` 让信息既显示在终端，也写入日志。复盘时应保留第一条错误和发生时间，而不是只截最后一屏。

### 7.3 设备文件与权限

相机、串口和 CAN 适配器接入 Linux 后通常会对应 `/dev` 下的设备文件：

```bash
lsusb
ls -l /dev/video*
ls -l /dev/ttyUSB* /dev/ttyACM*
dmesg | tail -n 30
```

常见问题不是“代码突然坏了”，而是设备没有枚举、权限不足或编号变化。例如同一串口可能从 `/dev/ttyUSB0` 变成 `/dev/ttyUSB1`。临时执行 `chmod 777` 会扩大权限且重启后失效，更可靠的做法是用用户组和 udev 规则绑定稳定名称。

## 8. RoboMaster 项目的 Git 工作流

课堂作业只有一个人修改，但战队工程会同时修改视觉、控制、通信和配置。Git 的目标不是“把代码传上去”，而是让每一次改动可审查、可回退、可定位。

### 8.1 不应提交的产物

```gitignore
build/
devel/
logs/
*.bag
*.core
*.o
```

`build/`、`devel/` 和日志都可以重新生成；bag 文件体积大；崩溃转储可能包含现场数据。真正应提交的是源码、必要配置、launch 文件和能复现实验的说明。

相机标定、PID 等配置可以纳入版本管理，但账号令牌、密码和私人密钥不能提交。若不同机器人参数不同，可以按机器人编号拆分配置文件，而不是每次上车手改同一个文件。

### 8.2 一次可审查的修改

```bash
git switch -c fix/chassis-command-timeout
git status
git diff
git add src/chassis_node.cpp config/chassis.yaml
git diff --cached
git commit -m "Add chassis command timeout protection."
git push -u origin fix/chassis-command-timeout
```

分支名说明问题，Commit 说明做了什么，暂存区只放本次相关文件。不要把“调 PID、改通信协议、整理格式”混成一个提交，否则出问题时很难只回退其中一项。

### 8.3 上场版本必须可回退

稳定版本完成整车测试后可以打标签：

```bash
git tag -a field-test-2026-10-31 -m "Stable field-test build."
git push origin field-test-2026-10-31
```

如果赛前新改动导致机器人异常，可以从稳定标签建立修复分支，而不是在未知状态下继续叠加修改：

```bash
git switch -c hotfix/field-test field-test-2026-10-31
```

标签只表示代码版本。要真正复现，还要记录系统版本、依赖、参数文件、固件或通信协议版本。

## 9. 真实故障的思考顺序

| 问题 | 先检查 | 不应立即做 |
| --- | --- | --- |
| `cd` 提示目录不存在 | `pwd`、`ls`、目录语言和大小写 | 反复复制同一条命令 |
| 节点启动后立刻退出 | 第一条报错、当前用户、环境变量 | 直接重装 ROS |
| 设备无法打开 | `/dev`、用户组、`dmesg`、线缆供电 | 长期使用 `chmod 777` |
| 上车后版本不一致 | `git status`、`git log -1`、标签 | 凭记忆判断版本 |
| SSH 断开后程序消失 | 前台任务、`tmux`、进程状态 | 只检查网络通不通 |
| rosbag 把电脑写满 | `df -h`、录制范围和分包策略 | 录制所有话题后不监控 |

真正的排错记录至少包含：现象、复现条件、第一条错误、根因、修改内容和复测结果。这样下次遇到同类问题时，笔记才会变成可重复使用的经验。
