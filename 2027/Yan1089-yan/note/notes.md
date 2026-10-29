# 10.20日第一次笔记
## 一·linux系统
1.具有ros兼容性，部署一致性和现有生态开发，常用于机器人和嵌入式开发
2.团队多使用小电脑通过ubuntu系统，用ssh，密钥与github链接完成远程开发和控制。
3.linux终端根目录从/开始
---
## 二·shell和vim
终端：输入输出界面
Shell：解释命令、执行命令

### 常见Shell命令一般形式：命令 [选项] [参数]
1.ls：做什么，列目录内容
2.-la：怎样执行
3.~/projects：操作哪个对象，这里是目标目录
4.Pwd:展开当前目录
5.cd 目录（名）：进入指定目录
6.cd ..：返回上一级
7.cd ~：返回家目录
目录操作：
mkdir -p ~/linux_demo：创建目录
touch：创建空文件，或更新文件时间
cp：复制文件
mv：移动或重命名
rm ：删除文件（不进回收站，删除前先确认路径和文件名）
cat：查看内容
Less：分页查看

### Shell 的四种常用组合
1.变量：保存一个值，再通过变量引用（等号两边不加空格）
2.重定向：把输出写进文件  >覆盖写入，>>追加到文件末尾
3.管道：把前一个命令的输出交给下一个程序
4.条件连接：前一条成功后，才执行下一条

### 陌生命令的帮助入口（要先查用途与参数，再自己练）
1.ls --help：快速查看选项和用法
2.man ls：打开手册，按 q 退出（若系统未安装 man 页，用 --help 替代）
3.Tab：补全命令或路径
4.↑ 和 ↓：调出历史命令

### Vim（终端输入vin进入）有三种模式：
1.普通模式：移动、删除、复制（按esc返回）
2.插入模式：输入文本（普通按i切换）
3.命令行模式：保存、退出（wq保存退出,q退出,q!不保存强制退出，w保存）都要加冒号

### Vim常用按键
h、j、k、l：左、下、上、右移动（方向键也行）
dd：删除当前行
yy：复制当前行
p：粘贴
u：撤销
/word：搜索

---
## 三·git 与 gitHub
### ·git：本地控制（没有图形界面，在终端和github上操作）
提交记录
比较修改、查看历史
为新功能创建分支
###·gitHub：远程托管与协作平台
托管 git 仓库
通过 PR 讨论和评审（远程合并修改）
合并团队认可的修改

### git 的四个位置
1. 工作区：正在编辑的文件  git add
2. 暂存区：本次提交的内容清单 git commit
3. 本地仓库：已记录的提交历史 git push
4. gitHub 远程仓库：远程保存的提交历史
注意：commit 只记录到本地仓库，push 才把提交发送到远程

### 使用 git：
1.设置提交作者
git config --global user.name ""（name和双引号要有空格，同email）
git config --global user.email ""
看是否生效：git config --list即可
2.配置SSH 密钥：公钥与私钥
用密钥完成远程认证，私钥自己保存，保密，不可以泄露
生成：ssh-keygen -t ed25519 -c "邮箱"
查看公钥：cat ~/.ssh/id_ed25519.pub（公钥复制到 gitHub中，右上角设置，点SSH）

### git 常用命令：
1.获取与查看
git clone <仓库地址>：下载仓库及其提交历史
git status：看目前分支和文件状态
git diff：看没存的修改
git log --oneline：用简短格式查看提交历史
2.分支与提交（切根子前先看status？）
git switch -c feature/hello：创建并切换到功能分支
git add hello.txt：暂存指定文件
git commit -m "Add greeting"：记录一次本地提交
git push -u origin feature/hello：推送并建立跟踪关系
git switch main：切回主分支
git pull：获取并整合当前分支的远程更新


### 分支是为新功能保留的独立开发线：main 保持稳定，feature/hello 承载本次修改，评审后再合并，方便其他人修改开发与合作，并不干扰原来的主分支。（不是fork仓库副本，分支是开发线）

### 团队 PR 工作流：团队默认使用 fork 协作：远程复制仓库，本地开发，再向团队发起合并请求。

1.先gitHub 上 fork 团队仓库，得到个人 fork（仓库副本）
2. clone到本地
3.开feature 分支，在新分支上修改
4. push 到自己的fork
5.向团队仓库发起 PR
6.review评审通过后merger到团队仓库
有团队仓库推送权限，可以省略 fork，直接在同一仓库内分支协作

### 完整参与流程：
linux系统终端：进入开发环境
shell：定位与操作文件
vim：编辑文件内容
git：分支、暂存与提交
gitHub：PR、评审与合并
git 记录版本，gitHub 支持协作。