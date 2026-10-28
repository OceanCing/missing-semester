#  第一节培训课笔记

## window与linux的区别
- window的兼容性更强，生态更丰富
  linux的运行更透明，架构更精简，效率更高
  机器人开发中使用微电脑控制机器人
  linux系统的设计理念是一切皆文件

## linux系统的基本使用
- linux系统的目录从/开始，操作文件主要通过终端来进行，而不是像window使用图形界面

- shell是一个解释命令的程序

- *shell命令的结构*
  命令（ls） 选项（-la）参数（～/projects）

- *基本的语法有*
  进入指定目录     cd 目录名/
  显示当前工作目录 pwd
  列出目录内容     ls
  创建目录         mkdir 目录名/
  新建文件         touch 文件名
  移动或重命名     mv 原文件 名字或目标目录
  删除文件（直接删除，不进回收站） rm 文件名
  
- *常见组合*
  变量 `name="NAME"`
       `echo "$name"`
                     
  重定向 `echo "hello" > hello.txt` //改变输出方向
  
  管道 `ls -la | grep.txt` //前一个命令的输出当后一个命令的输入
  
  条件连接 `mkdir -p demo && cd demo` //类似if else
  
  //不记得指令的可以输入`ls --help`来查阅指令
  `tab`可快速补全命令
  `Ctrl-C`中断当前命令
  `Ctrl-L`清屏

- 使用指令 vim 文件名/ 打开文件并进入普通模式
  普通模式，可以移动删除和复制
  在普通模式通过hjkl来控制左下上右
  dd 删除当前行 
  u 撤销
  /word 搜索word 

- 按 i 进入插入模式，然后可以输入文本

- 按住shift+:可以进入命令模式
  `wq` #保存并退出
  `w`  #保存
  `q!` #强制退出

## git 与 github
- git是一个安装在本地的程序，功能是记录记录文件修改的状态，以便多人开发，完成后可把git修改后的代码远程同步到远程仓库，然后在github接受团队审查，如果通过审查就把你的分支合流到主分支

- github主要存储的是团队的数据和代码

- 要使用git和github，要先配置ssh密钥，然后添加ssh公钥到github

- *常见的指令有*
  ```bash
  git clone <仓库名> #把仓库的内容克隆过自己的仓库
  git status #查看当前分支和文件的状态
  git diff #查看当前分支和文件状态
  git log --oneline #查看提交历史
  git add text.txt #暂存本地文件
  git branch #查看本地分支
  git switch -c feature/notes# 新建并切换到新分支
  git commit -m "Add greeting" #记录一次本地提交
  git push -u origin 用户名/仓库名 #推送并建立跟踪关系  
  git switch main #切换为主分支
  git pull #拉取远程更新
```
  
- 分支就是让同一个仓库中的成员开发互不干扰，等开发完了再把分支和主分支合并

- *团队pr工作流*
  先从团队仓库fork文件到个人仓库，然后在本地电脑进行分支的修改和开发，完成后push到个人fork，然后进行pr，经过团队审核后，merge到团队的仓库
  然后一次完整的团队贡献大致如上面的过程所示




 
    






