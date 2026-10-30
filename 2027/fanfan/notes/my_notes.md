#张佳凡控制视觉第1课笔记
今天的培训课主要讲了怎么用linux把代码传到github上
##1linux
以前zhi知道windows系统，直到学习学长学姐要求的课前准备，并尝试跟着教程装后才知道有ubuntu系统（linux的其中一种），然后讲到了路径：linux的目录从/开始，～（当前用户的家目录），.（当前目录），..（上一级目录）
##2.终端、shell与命令
###shell命令的结构：命令（做什么）+选项（怎样执行）+参数（操作那个对象，这里是目标目录
###当前位置与目录切换：pwd:显示当前目录，ls:列出目录内容，cd目录：进入指定目录，cd..:返回上一级，cd~：返回家目录
###文件和目录操作：mkdir-p：创建目录，允许补齐父目录；touch：创建空文件/更新时间；cp:复制文件；mv:移动或重命名；rm:删除文件（！！注意删了就找不回来了！！）；cat/less：直接查看/分页查看
###shell的四种常见组合：变量：保存一个值再通过变量引用它；重定向：>覆盖写入，>>追加到文件末尾；管道：将输出交给下一个程序；条件连接：前面一条成功才执行下一条
###陌生命令的帮助入口：--help（快速查看），man（打开手册，按q推出），tab(补全），上箭头/下箭头（调出历史命令）
##3.vim编辑其
h j k l:上 下 左 右；dd:删除现在这一行；yy/p:复制当前行/粘贴；u：撤销；/word：搜索word。：w(保存)。：q(推出）。：wq（保存退出）。:q!（放弃修改直接退出）
esc：返回普通模式
##4.git与github
###git与hithub的分工：git:本地版本控制，github：远程托管协作，commit：之记录本地仓库，push：提交发送到远程
###公钥（.pub）与私钥:复制到github的shh设置（公），不上传、截图、分享（私）
###把ssh公钥添加到github：1.settings  2.ssh and GPG keys  3.new ssh key
###git常用命令：
获取与查看：git clone<仓库地质>(下载仓库及提交历史）git status(查看当前分支和文件状态）git diff(查看还没暂存的修改）git log--oneline（用简短格式查看提交历史）
分支与提交：git switch-c feature/hello（创建并切换到功能分支）git add  hello.txt(暂存指定文件）git commit-m"add greeting"(记录一次本地提交）git pull（获取并整合当前分支的远程更新）git switch main(切回主分支）git push-u origin feature/hello（推送并建立跟踪关系）
