# linux 基础命令
##文件操作：
###寻找文件路径
1.pwd
2.ls
3.cd
###建，删，复，移
1.touch
2.vim/gedit/vi
3.mkdir
4,rm
5.cp
6.mv
###查看文件
1.cat
2.less
3.head
4.tail
###搜索
1.find
2.grep
###解压与压缩
####gz 文件
1.gzip ..
2.gzip -d ...
3.gzip -r ..
4.gzip -rd ...
####bzip2文件
1.bzip2 ...
2.bzip2 -d ..
####tgz
1.tar -zcvf ... [goal]
2.tar -zxvf ... 
-z=gzip,c=create, v=visit,f=point to file x=

##查看系统信息
###系统
1.top/ntop
2.ps -aux | grep name 
3.kill
4.df -h
5.free -h
6.uname
###安装
1.sudo apt update/install/remove/search
###网络
1.ping
2.ip a
3.curl
###系统文件
1./根目录
2./boot内核
3./dev设备驱动文件
4./home用户主文件夹
5./root用户目录
6./usr存放于系统用户有关的文件
###磁盘管理
1.fdisk查看分区
##用户
###用户管理
1.chmod权限修改
2.gcc 编译
3.chown用户/用户组归属调整
# git和github
##开始
1.git init
2.git config --global
3.git add code.txt
4.git commit -m "version"
5.git log
6.git reflog
##回退 
1.git reset
##撤销
1.git checkout --file
2.git reset HEAD file
##对比
1.git diff HEAD --FILE
2.git diff HEAD HEAD~n
##分支管理
1.git branch
2.git checkout -b branch-name
3.git checkout branch
4.git merge
5.git branch -d branch-name
6.git stash(list/pop/ blank/)
##github
###ssh
1.ssh -keygen -t ed25519 -C "email"
2.cat ~/.ssh/id-ed25519.pub
3.git remote set-url origin git @github.com:username/name.git
###clone
1.git clone git@github.com:username/name
2.git clone -b branch git@github.com:username/name.git
###push&pull
1.git push origin branch
2.git branch -u origin/main
3.git branch --unset-upstream
4.git pull origin name

# vim 的用法
##mode
1.insert
2.command
3.visual
###command
1.w/q/wq/q!
###visual
1.x
2.y
3.p
##搜索
1.f{char}
2./{}
3.?{}
4.n+ 命令

# shell脚本
1.#!/bin/bash
2.bash
3.echo ""
4.&&和||

