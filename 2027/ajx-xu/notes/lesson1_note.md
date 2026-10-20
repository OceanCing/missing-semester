# 第一节课堂笔记
## 1.Linux的特性以及基本操作方式
### 目录
-进入目录
-目录推荐语言设置
## 2.vim与shell
### shell
#### shell指令生效过程
-终端——shell——程序
#### shell命令
##### shell命令结构
-`ls`命令
-`-la`选项
-`~/projects`参数
#### 文件与目录
##### 位置切换
-`pwd`显示当前目录
-`ls`列出目录内容
-`cd `“目录名”进入目录
-`cd ..`返回上一级
-`cd ~`返回home目录
##### 文件目录切换
-`mkdir -p`:创建目录，允许补全父目录
-`touch`:创建空文件/更新时间
-`cp`:复制文件
-`mv`:移动或重命名
-`rm`:删除
>通常不进回收站
-`cat/less`:直接/分页查看
#### shell常用组合
-变量：```name="RM"
-       echo"$namea"```
>保存一个值再用变量引用
-重定向：`echo"hello">hello.txt`
>">"覆盖写入；">>"追加到文件末尾
-管道：`ls-la|grep txt`
>将输出交给下一个程序
-条件连接：`mkdir-p demo&&cd demo`
>前一条成功后执行下一条
-快速查看选项和用法：`ls--help`
-打开手册，q退出：`man ls`
-补全：`Tab`
-调出历史：上/下键


### vim
#### vim文件操作
-创建项目：`vim 文件名`
>若新文件不编辑则不保存
-删除项目：`rm 文件名`
#### vim内部操作方式
##### 普通模式
-移动：`hjkl`/左下上右
-删除行：`dd`
-复制行/粘贴：`yy/p`
-撤销：`u`
-搜索`word：/word`
>`i`进入插入模式
>`:`进入命令行模式
##### 命令行模式
-保存：`w`
-退出：`q`
-保存退出：`wq`
-放弃修改退出：`q！`
>！代表强制
**## git与github以及工作流程**
### git与github分工
-git：本地版本控制
-github：远程托管、协作
### git
-位置：*工作区*——git add——*暂存区*——git commit——*本地仓库*——git pushu——*github远程仓库*
>    正在编辑的文件   本次提交的内容清单      已记录的提交历史       远程保存的提交历史
>commit只到本地仓库；push才能提交到远程
-提交作者：`git config--global user.name"    "`
-邮件：`git config--global user.email"     "`
### SSH密钥
-公钥：复制到github
-私钥：不分享
>数据安全
### git常用命令
#### 获取查看
-`git clone<仓库地址>`：下载仓库及提交历史
-`git status`：查看当前分支和文件状态
-`git diff`：查看未暂存的修改
-`git log --oneline`：简短提交历史
#### 分支提交
-`git switch -c feature/hello`：创建并切换功能分支
-`git add hello.txt`：暂存文件
-`git commit -m "Add greeting"`：记录一次本地提交
-`git push -u origin feature/hello`：推送并建立跟踪关系
-`git switch main`：切回主分支
-`git pull`：获取、整合当前分支的远程更新
### 团队开发
#### 开发线路
![分支线路](/home/ajx/rm/missing-semester/2027/ajx-xu/homework/images/截图 2026-10-20 20-26-17 （已编辑 1）.png) 
#### 团队PR工作流
![工作流程图](/home/ajx/rm/missing-semester/2027/ajx-xu/homework/images/截图 2026-10-20 20-33-24 （已编辑 1）.png)
#### 团队贡献流程
![一次团队贡献流程图](/home/ajx/rm/missing-semester/2027/ajx-xu/homework/images/截图 2026-10-20 20-35-35 （已编辑 1）.png)
