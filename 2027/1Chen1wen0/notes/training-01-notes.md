# 第一课笔记

## 1.Linux
- Linux通常指内核，Ubuntu是其发行版之一。
- 发行版通常包括：
	1. Linux 内核
	2. 系统工具和基础命令
	3. Shell，例如 Bash
	4. 软件包管理器
	5. 初始化系统，例如 systemd
	6. 图形桌面环境，例如 GNOME、KDE
	7. 安装程序、配置工具和默认应用
	8. 软件仓库及维护团队

## 2.vim
- Vim 是 Linux/Unix 中常见的终端文本编辑器，功能强大，适合编辑代码、配置文件和脚本。
- 模式：
	1. 普通模式：移动光标、复制、删除（hjkl：上下左右 ，dd：删除当前行 , yy/p：复制当前行/粘贴 ，u：撤销 ， /word：搜索word）
	2. 插入模式：输入文本
	3. 命令模式：保存、退出（：w ， ：q ， ：wq ， ：q！）

## 3.shell
- Shell 是一种命令解释器，负责接收用户输入的命令，并让操作系统执行。它通常运行在终端中，是用户与 Linux/Unix 系统交互的重要方式。
- 终端是窗口，Shell 是窗口里的命令解释程序。
- 常见指令：
	1. mkdir -p	创建目录，必要时补齐父目录	mkdir -p ~/linux_demo
	2. touch	创建空文件或更新时间	touch hello.txt
	3. cp	复制文件	cp hello.txt hello-copy.txt
	4. mv	移动或重命名文件	mv hello-copy.txt backup.txt
	5. rm	删除文件	rm backup.txt
	6. cat / less	查看文件内容，less 可分页查看	cat hello.txt

## 4.Git&&Github
- Git:
	1. 作用：记录每次提交，比较修改、查看历史。
	2. 位置：工作区 --git add--> 暂存区 --git commit--> 本地仓库 --git push--> GitHub 远程仓库。
	3. 常用指令：
		git switch -c feature/hello	创建并切换到功能分支
		git add hello.txt	暂存指定文件
		git commit -m "Add greeting"	记录一次本地提交
		git push -u origin feature/hello	推送分支，并建立跟踪关系
		git switch main	切回主分支
		git pull	获取并整合当前分支的远程更新
- Github:
	1. 作用：托管 Git 仓库，通过 PR（Pull Request）进行讨论和评审，合并团队认可的修改。 

