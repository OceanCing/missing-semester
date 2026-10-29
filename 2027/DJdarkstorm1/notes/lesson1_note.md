# 第一课作业

## Linux

+ 一种系统内核

+ 发行版 = 基于系统内核 + 系统工具 + 软件包 + 默认配置
  + ubuntu
  + debian
+ 应用：机器人开发

## 终端与shell

+ 终端是系统/程序的“终端”

+ shell把终端的输入翻译成命令，调度程序

  ![zsh终端与bash终端](/home/dj/图片/zsh终端与bash终端.png)

+ 常用命令
  1. `date`：打印当前的日期和时间。
  2. `echo`：参数打印。   例：`echo`  “hello world”
  3. `pwd`：打印当前工作目录的路径。
  4. `cd`：切换目录，后加路径。 例：`cd`  ./home
  5. `ls`：查看指定目录下包含哪些文件
  6. `--help`：打印帮助信息，以便了解有哪些可用的标记或选项。格式为命令 +  `--help`。例：`ls` --help
  7. `mv`：用于重命名或移动文件。
  8. `cp`：拷贝文件。
  9. `mkdir`：新建文件夹。
  10. `chmod`：改变文件的权限。修改（`w`），例如，添加或删除文件夹中的文件。可执行（`x`），为了进入某个文件夹或执行某个文件。读权限（`r`），列出文件或文件夹所包含的内容。
  11. `touch`：新建文件。格式为touch + 文件名。
  12. `grep`：筛选工具。例：`grep`  ‘hello’，筛选所有包含”hello“的行。
  13. `^Z`：暂停进程。`fg`：在前台继续进程。`bg`：在后台继续进程。
  14. `journalctl`：打印系统日志。

## Vim

+ 参考vim_note.txt(用vim打开即可)

  ```vim
  i:插入模式
  esc:退出插入模式返回普通模式
  ‘:’:普通模式输入英文冒号进入命令模式
  
  :/word xxx  :find word
  :wq         :save and exit
  dd          :delete a line
  yy/p        :copy/paste
  yy/p        :copy/paste
  u           :撤销操作
  hjkl        :move left/down/up/right
  
  this is a line to be copied.
  
  this is a line pasted:
  this is a line to be copied.
  
  this is some words to find.
  a word 
  another word
  
  more operation can be find in:https://quickref.me/vim
  一个B站视频：【【Vim命令全解】新人如何入坑？教你自定义快捷键快速编译运行程序，并定位编译出错点-哔哩哔哩】 https://b23.tv/7BcYKIo
  ```

  

## Git&&Github命令

### 基础

1. `git help <command>`: 获取 git 命令的帮助信息。
2. `git init`: 创建一个新的 git 仓库，其数据会存放在一个名为 .git 的目录下。
3. `git status`: 显示当前的仓库状态。
4. `git add <filename>`: 添加文件到暂存区。
5. `git commit`: 创建一个新的提交。
6. `git log`: 显示历史日志。
7. `git log --all --graph --decorate`: 可视化历史记录（有向无环图）。
8. `git diff <filename>`: 显示与暂存区文件的差异。
9. `git diff <revision> <filename>`: 显示某个文件两个版本之间的差异。
10. `git checkout <revision>`: 更新 HEAD 和目前的分支。

### 分支和合并

1. `git branch`: 显示分支。

2. `git branch <name>`: 创建分支。

3. `git checkout -b <name>`: 创建分支并切换到该分支。

   相当于` git branch <name>`;

   ​		 `git checkout <name>`

4. `git merge <revision>`: 合并到当前分支。

5. `git mergetool`: 使用工具来处理合并冲突。

### 远程操作

1. `git remote`: 列出远端
2. `git remote add <name> <url>`: 添加一个远端
3. `git push <remote> <local branch>:<remote branch>`: 将对象传送至远端并更新远端引用
4. `git branch --set-upstream-to=<remote>/<remote branch>`: 创建本地和远端分支的关联关系
5. `git fetch`: 从远端获取对象/索引
6. `git pull`: 相当于 `git fetch`; `git merge`
7. `git clone`: 从远端下载仓库

### 撤销

1. `git commit --amend`: 编辑提交的内容或信息
2. `git reset HEAD <file>`: 恢复暂存的文件
3. `git checkout -- <file>`: 丢弃修改
4. `git restore --staged <file>...`: 撤销暂存操作





