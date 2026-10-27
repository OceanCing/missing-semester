# Lesson 1 从Linux到第一次Pull Request
## 1. Linux介绍
### 1.1 与 windows 和 macos 的区别
- **windows**-常用于个人桌面 软件和游戏生态丰富

- **mac os**-集成Apple生态，日常使用和开发

- **Linux(Ubuntu)**-常用于服务器、机器人和嵌入式

### 1.2 Linux与发行版
> Linux是**内核**；**发行版**提供可以安装和使用的代码

- **发行版**

| Linux发行版 | 系统工具与安装包 | 使用场景|
|---|---|---|
|Ubuntu|`apt`(基于Debian)|桌面、服务器、机器人开发|
|Fedora|`dnf`(RPM系)|使用较新软件|
|Arch Linux|`pacman`,滚动更新|用于学习|

### 1.3 终端、shell与指令
> 在终端输入命令，由Shell解释，再交给程序执行

- **Terminal终端→ Shell(Bash/Zsh)→ 程序/系统**
> 输入输出界面→ 解释命令、展开路径、组织执行→ ls查看文件、git管理版本
### 1.4 贯穿演示
- 创建目录→ 新建文件→ 编辑内容→ 记录修改
> `~/example` →  `hello.txt` →  `Vim` →  `Git`
### 1.5 Shell命令的结构
> 命令『选项』『参数』

|`ls`|`-la`|`~/project`|
|---|---|---|
|做什么|怎样执行|操作哪个目录|

### 1.6 Shell指令一览
> 当前位置与目录切换

|命令|作用|
|---|---|
|`pwd`|显示当前目录|
|`ls`|列出目录内容|
|`cd 目录`|进入指定目录|
|`cd ..`|返回上一级|
|`cd ~`|返回家目录|

> 文件和目录操作

|命令|作用|
|---|---|
|`mkdir -p`|创建目录、自动创建父目录|
|`touch`|创建空文件/更新时间|
|`cp`|复制文件|
|`mv`|移动/重命名|
|`rm`|删除文件*不进入回收站*|
|`cat/less`|直接查看/分页查看|
|`ls --help`|帮助指令|
|`man ls`|打开手册|

## 2.Vim介绍
- 使用手册

|`指令`/『按键』|执行|
|---|---|
|『h』『j』『k』『l』|左-下-上-右|
|『dd』|删除当前行|
|`:w`|保存|
|`:wq`|保存并退出|
|`:q`|直接退出|
|`:q!`|放弃修改并退出|

## 3.Git与Github
### 3.1 Git与Github的分工
> 多人修改同一项目，用版本记录和评审代替互传压缩包
- Git：本地版本控制

*记录每次提交，比较修改、查看历史，为新功能创造分支*
- Github：远程托管和协作

*托管Git仓库,通过PR讨论和评审，合并团队认可的修改*

### 3.2 Git的四个位置

|工作区→ `git add`→ |暂存区→ `git commit`→ |本地仓库→ `git push`→ |Github远程仓库|
|---|---|---|---|
|正在编辑的文件|本次提交内容清单|已记录的提交历史|远程保存的提交历史|

> 注:commit只记录到本地仓库；push才把提交发送到远程
### 3.3 Git常用指令
- 获取与查看

`git clone <仓库地址>` *下载仓库及提交历史*

`git status` *查看当前分支和文件状态*

`git diff` *查看尚未暂存的修改*

`git log --online` *用简短格式查看提交历史*

- 分支与提交

`git switch -c feature/hello` *创建并切换到功能分支*

`git add hello.txt` *暂存指定文件*

`git commit -m "Add greeting"` *记录一次本地提交*

`git push -u origin feature/hello` *推送并建立跟踪关系*

`git switch main` *切回主分支*

`git pull` *获取并整合当前分支的远程更新*
## 4.贡献的完整流程
- `Linux` → `Shell` → `Vim` → `Git` → `Github`
---
> `created by Sklygit`






## 5. 命令演示参考

```bash
# 目录与文件：创建、进入、查看
mkdir -p ~/example
cd ~/example
touch hello.txt
ls -la
```

```vim
" vim 最常用的几个
i        进入插入模式
Esc      回到普通模式
:wq      保存并退出
```

```bash
# git 一次完整提交
git status
git add hello.txt
git commit -m "Add greeting."
git push -u origin feature/hello
```


