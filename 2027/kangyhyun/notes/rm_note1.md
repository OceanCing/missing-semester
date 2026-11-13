#DynamicX课堂笔记
#日期2026.10.17
#智能感知工程 康宸斌

#================================Linux系统笔记====================================

##Linux系统核心目录结构
- `/bin`:存放最常用的基础命令（如ls，cp，mkdir等）
- `/etc`:存放系统的配置文件。
- `/home`:用户家目录
- `/root`:管理员的家目录
- `/var`:存放变化的文件，如：日志文件。

##Linux系统终端常用指令

上下键可以翻阅历史命令

- `sudo`:临时获取最高权限，安装软件，改系统文件
sudo apt类
sudo apt install 软件名（安装软件）
sudo apt remove 软件名（卸载软件）

###一、查看类

####1.关于系统
- `pwd`:直接查看当前位置
- `stat -f`:查看本机块大小
- `df -h`:查看磁盘剩余空间
- `tree`:以树形展示文件夹结构
- `htop`:查看电脑进程
- `top`:查看占用cpu内存
按q退出（可用于排查机器人程序卡死，cpu占用过高，后台进程排查必备）

- `whoami`:查看当前登录用户名
kang@kang:~/rm_preview$ whoami
kang

- `uname -a`:查看当前系统内核版本
kang@kang:~/rm_preview$ uname -a
Linux kang 7.0.0-38-generic #38-Ubuntu SMP PREEMPT_DYNAMIC Fri Sep  4 09:10:14 UTC 2026 x86_64 GNU/Linux


####2.查看文件
- `cat`:一次性查看整个文件,将整个文件打在终端上(要加文件名，不加会读取并打印你的输入）
（Ctrl+D结束输入）（Ctrl+c强制终止）

- `bat`——————交互式查看文件，对语法有高亮

- `head/tail`——看开头、看结尾
tail ~ n 10 ~/.bash_history>>"要写入的文件"读取最后十行写入

- `less`:分页查看文件内容（适合长文件，按`q`退出）。

- `grep`:在文本中搜索特定字符串（如`grep"error" log.txt`）。

- `wc`：统计文件的字数和行数（`wc -w`统计单词数，`wc -l`统计行数，`wc -m`多适用于字符，汉字）

- `ls`———————查看当前文件夹里的文件
- `ls -l`————查看详细信息（权限、大小、修改时间)
- `ls -al`———显示所有文件（含隐藏文件。Linux中以.开头的是隐藏文件）
例：
kang@kang:~/rm_preview$ ls -l
total 8（代表所有文件占用的磁盘块总和即总共占用4kb磁盘）
-rw-rw-r-- 1 kang kang    0 Oct 16 12:58 log.txt
drwxrwxr-x 2 kang kang 4096 Oct 16 13:03 test_folder
drwxrwxr-x 2 kang kang 4096 Oct 16 20:02 终端指令

kang@kang:~/rm_preview$ ls -al
total 16
drwxrwxr-x  4 kang kang 4096 Oct 16 13:16 .
drwxr-x--- 17 kang kang 4096 Oct 16 13:15 ..
-rw-rw-r--  1 kang kang    0 Oct 16 12:58 log.txt
drwxrwxr-x  2 kang kang 4096 Oct 16 13:03 test_folder
drwxrwxr-x  2 kang kang 4096 Oct 16 20:02 终端指令


###二、创建类
- `mkdir/mkdir -p`:创建文件夹
kang@kang:~$ mkdir rm_preview（mkdir是创建空文件夹）

- `touch`：创建文件(如:md,txt)
kang@kang:~/rm_preview$ touch log.txt（touch是创建空文件，没办法cd进入）
kang@kang:~/rm_preview$ touch note.md

- `cp`:制作副本
cp 原文件名    副本文件名
kang@kang:~/rm_preview$ cp log.txt log_backup.txt（制作log.txt的副本）

###三、索引类
- `cd`:cd 要去的地方
kang@kang:~$ cd rm_preview（cd是去哪里）
（如cd ~是去家目录，注意～和~的区别。cd ..是退一级）


###四、修改移动类
- `mv`:mv 旧文件名 新文件名或移动到的文件夹
kang@kang:~/rm_preview$ mv note.md readme.md（重命名note.md为read.md)
kang@kang:~/rm_preview$ mv readme.md test_folder（移动到test_folder)


###五、删除类
- `rm`：删除文件
kang@kang:~/rm_preview$ rm iog_backup.txt(删除空文件）
rm后面接多个文件名，会同时删除
rm -i（建议使用）（删除前会询问，避免误删）
rm -I(多个文件才提示，少量不提示）
rm -f(强制删除）

- `rmdir`:删除文件夹
kang@kang:~/rm_preview$ rmdir test_folder(删除空文件夹）

###六、重定向与管道符
- `>`:**覆盖**重定向（把左边的命令的输出，写入到右边的文件中，完全覆盖原文件内容）
- `|`:**管道符**（把左边的命令输出，作为右边命令的输入。如：`ls -l | grep ".md`只显示.md文件)

#================================Git笔记=========================================

##Git指令
###一、设置类
- `git config --global user.name`:设置用户名。
- `git config --global user.email`：设置邮箱地址
- `git config --list`:查看当前所有git的配置
###二、仓库工作类
- `git init`：在当前目录初始化一个新的本地Git仓库
- `git clone `<仓库地址>：把远程仓库完整下到本地。
- `git status`:查看当前工作区和暂存区的状态。可以查看当前哪些文件被修改了，哪些文件已经准备提交。
- `git add`:把当前目录下的所有修改、新建的文件添加到暂存区。
- `git commit -m`:提交暂存区的内容到本地仓库，形成记录。
- `git remote -v`:查看当前关联的远程仓库地址
- `git push`:把本地仓库的提交推送到远程仓库
- `git pull`:把远程仓库的最新代码拉取下来
- `git fetch`:只拉取信息，但不自动合并
###三、分支管理类
- `git branch`:查看本地所有分支（*号跟着当前分支）
- `git branch <分支名>`:创建新分支
- `git checkout <分支名>:`与`git switch <分支>`：切换到指定分路
- `git switch -c <分支>`:创建并切换
- `git checkout -b <分支名>`:创建并切换
- `git merge <分支>`:把指定的分支合并到当前所在分支
###四、查看历史与差异
- `git log`:查看历史提交记录（按q退出）
- `git diff`:查看工作区和暂存区文件的具体代码差异
###五、撤销与回退
- `git restore<文件名>`:放弃工作区中某个文件的修改。
- `git restore --staged<文件名>`:从暂存区退回到工作区。
- `git reset --hard <commit-id>`:把本地仓库回退到某个历史版本。
###Git工作流
先创建一个分支，再切换到新分支上，写完再提交到主路并删除支路

#=================================Vim笔记=========================================
##  Vim 文本编辑器基础
Vim 是 Linux 下极其强大的命令行文本编辑器。它的特点是“纯键盘操作”，一旦熟练，双手无需离开键盘即可完成高效编辑。

### 1. Vim 的三种核心模式
- **普通模式（Normal Mode）**：打开 Vim 时的默认模式，主要用于移动光标、复制、粘贴、删除等操作。此时敲击键盘不会输入文字。
- **插入模式（Insert Mode）**：用于正常打字输入。在普通模式下按 `i` 键（insert）进入。
- **底行模式（Command-line Mode）**：用于保存、退出、查找替换等。在普通模式下按 `:` 键进入。

*模式切换逻辑：普通模式按 `i` 进入插入模式；插入模式按 `Esc` 退回普通模式；普通模式按 `:` 进入底行模式。*

### 2. 常用操作指令
- **退出与保存（底行模式）**：
  - `:w`：保存文件 (write)。
  - `:q`：退出 Vim (quit)。
  - `:wq` 或 `:x`：保存并退出。
  - `:q!`：强制退出，不保存修改（常用于误操作后想放弃修改）。
- **光标移动（普通模式）**：
  - `h` `j` `k` `l`：分别对应向左、下、上、右移动光标。
  - `gg`：跳转到文件第一行。
  - `G`：跳转到文件最后一行。
- **编辑操作（普通模式）**：
  - `dd`：删除（剪切）当前行。
  - `yy`：复制当前行。
  - `p`：粘贴到光标下一行。
  - `u`：撤销上一步操作。

---

##  Shell 脚本编程基础
Shell 脚本（Shell Script）是将一系列 Linux 命令按逻辑组合，保存到一个文本文件中，由 Shell 解释器自动执行。

### 1.脚本的开头：Shebang
每个 Shell 脚本的第一行通常都是 `#!/bin/bash`。
这叫做 Shebang，它告诉操作系统：“请使用 `/bin/bash` 这个解释器来运行我下面的代码”。

### 2. 变量定义与使用
- **定义变量**：`NAME="Kang"` （注意等号两边**不能有空格**）。
- **使用变量**：使用 `$` 符号读取变量的值，如 `echo "Hello, $NAME"`。

### 3. 基本控制结构
- **条件判断 (if)**：
  ```bash
  if [ "$NAME" = "Kang" ]; then
      echo "Welcome back!"
  else
      echo "Who are you?"
  fi
