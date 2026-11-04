# 课程笔记

## 目录

一 . [Markdown语法](#markdown-语法)
二 . [Linux](#linux)
三 . [Vim编辑器](#vim编辑器)
四 . [Shell](#shell)
五 . [Git And Github](#git-and-github)




## Markdown 语法
  ### 标题表示：几级就是几个# , 最多6级

  ### 引用

  >这是一段引用

  ### 有序列表
  1. 阿巴阿巴
  2. 阿巴阿巴
 
  ### 无序列表
  - 可以用短横线
  - 也可以用星号
  * 一个用短横线一个用星号会被识别为两个不同的列表，所以这一行和上一行之间应该会空一行

  ### 任务列表
  - [ ] 吃饭
  - [ ] 学习
  - [x] 耍起
 
  ### 代码块
 ```c

 nclude<stdio.h>

 i nt main ()
 {
  printf("hello,world");
  return 0 ;
 }
 ```

 ```c++
 include<iostream>

 using namespace std;

 int main ()
 {

  cout << "hello" << "world";
  return 0 ;
 }
 ```


 ### 数学公式

 #### 1.行内数学公式
  勾股定理：$a^2+b^2=c^2$

 #### 2.独立数学公式
 $$
 a^2+b^2=c^2
 $$
 #### 3.常用数学公式表达

 ##### 分数 
 $\frac{a}{b}$
 -a做分子

 ##### 平方根
 $\sqrt{x}$

 ##### 不等于 
 $\neq$
 -无穷大
 $\infty$

 ##### 下标
 $a_1 + a_2$

 ##### 正负号
 $\pm$


 ### 表格



 |姓名|年龄   |成绩|
 |:---|----:|:---:|
 |靠左对齐|靠右对齐   |居中|


 ### 注释

 这里有一个注释 [^注释]

 [^注释]:是的，方括号里的可以随意命名

 ### 可以自己加横线

 ---

 确实可以


 ### 加一个链接

 [我的github主页](https://github.com/ws643 "我的主页")

 [这里可以跳转到标题1]（#课程笔记）

 <https://github.com/ws643>

 ### *一个星号表示斜体*

 ### **两个星号表示加粗**

 ### 两个反引号表示行内代码 
  ``printf()``

 ### <u>u表示下划线</u>

 ### :smile:


 H~2~O  $H_2O$

 ==这是一段高亮文字==
## Linux

### 语法

  - cd  ----更改文件夹
  - cd .. -----回到上一级目录
  - pwd -----显示当前目录
  - clear -----清屏
  - ls ----显示当前路径下的所有文件
  - ls -al ----显示所有文件包括隐藏文件
  - touch ----新建文件
  - file ----显示文件类型
  - mkdir ----新建文件夹
  - rm 文件名----  删除文件
  - rm -r---- 递归删除文件夹里所有文件
  - cp 原路径 目标路径 -----复制文件到目标路径
  - cp -r 原路径 目标路径  -----复制文件夹到目标路径
  - mv 原路径 目标路径 -----复制文件到目标路径
  - find 目录 -name 名字 ----在路径下搜索文件
  - sudo ----暂时获得管理员权限
  - chmod 权限 文件名 ------如遇 [ permission denied ] 就是缺执行权限
  [+x表示加执行权限 -R递归（必须大写）]
  - cat ----打印整个文件内容到终端
  - ps ---- 查看当前进程
  - top ---查看系统资源占用 
  - sleep 100 ----当前进程等待100秒
  - exec -----替换当前进程

## Vim编辑器 
### 进入
vim 文件名

### 模式

#####  普通模式-----光标移动，删除，复制，粘贴
#####  插入模式-----修改字符，写入文本
#####  命令行模式---执行命令

### 普通模式进入插入模式

 - i ---- 从光标前开始写入
 - o ---- 向下新建一行
 - x ----删除光标所在字符
 - dd---删除行
 - 数字+dd----删除当前行往下N行
 - u----撤销操作
 - ctrl+r----撤销刚才的撤销
 - yy---复制行
 - 数字+yy----复制当前行往下N行
 - p----粘贴到光标后

### 插入模式进入普通模式
 - esc  ----- 退出插入模式 

### 普通模式进入命令行模式

 - :w  ----保存
 - :q----退出
 - :wq---保存并退出
 - q!----不保存，强制退出
 - :wq! -----强制保存退出（针对只读文件）
 - :行号-----跳到指定行

### 操作
 - h,j,k,l ----左，下，上，右
 - w-----跳到下一个单词开头
 - b-----跳到上一个单词开头
 - 0----跳到行首
 - $----跳到行尾
 - gg----跳到第一行
 - G----跳到最后一行

### 注意
 - 忘了在什么模式？----按几下esc肯定回到普通模式
 - ctrl + s终端卡住？----这是暂停输出，按ctrl+Q
 - 保存报错E45：‘readomly'option is set?---- :w !sudo tee %强制提权保存 ，可能还要写：e!重新加载文件，再:q退出




## shell
SHELL是用户和系统之间的翻译官，负责解析并执行命令,运行外部程序时，由SHELL请求内核创建新进程，新进程拥有新PID，运行后子进程会自动退出(sleep , exec除外)
###### 区分SHELL内置命令和外部命令：type 命令名
##### logic
 打开终端，bash自动运行，输出a@a
   - echo $$ ----输出当前进程的PID
   - cat /etc/shells  ----列出所有shell
   - echo ----打印
   - $变量名 -----读取环境变量的值
   - echo $HOME ----输出家目录，即 ～
   - echo $SHELL ----- 输出默认登录SHELL，登录后不变
   - echo $0  ------在终端里敲会输出当前运行的shell
              ------在脚本(.sh文件)里写：
              -----如果是用./test.sh运行，输出./test.sh                         
              -----如果是用bash test.sh运行，输出test.sh,会自动跳过脚本里的shebang(脚本第一行#！解释器路径)
  - 进入子SHELL直接敲SHELL名字，退出用exit
  - echo "$SHLVL" ---查看当前在第几个进程，输出1表示在第1个进程，2表示在一级子进程中


  ### 脚本编写
  #### 脚本基础
  - shebang ---- #!解释器路径
  - exit 0   ----退出，0表示成功
  - chmod +x test.sh ----为文件赋予执行权限
  - ./test.sh ----运行文件
  - bash test.sh ----创建子进程运行文件
  - source text.sh ----在当前进程上直接运行text.sh
  #### 输入输出
  - echo  "xxx"  ---打印xxx , echo默认会自动换行
  - printf "%s\n" "内容" -----格式化输出
  ```shell
  pritf "hello world\n"   # 输出hello world
  ```
  ```shell
  #!/bin/bath

  name="tom"
  printf "姓名： %s\n" "$name"
  ```
  - read 变量名 ----作用类似scanf
  - read -p "请输入：" name -----带提示语
  - raed -s password ----读取不会显示在屏幕上，适合输密码
  - read -t 5 name  ----最多等5秒
  - read -r line ----不处理反斜杠，建议总是加上，比如输入c:/code,不会自动变成c:code,而是保留原始输入
  #### 其他命令
  - date ----显示当前日期和时间
  - whoami ----显示当前用户
  - export name -----把name升级成环境变量，子进程中也能看见
  - export -n name ------把name从环境变量中拿出来
  - 脚本传参：
    ./test.sh apple banana ...
    程序名 参数1 参数2 ...
    - $0 ---./test.sh
    - $1 ---apple
    - $2 ---banana
    - $# ---2(总参数数)
    - $@ ---apple banana (所有参数)
    - $? ---获取上一条命令的退出码
      用法：
      ```shell
      ls 
      echo $?
      # ls执行成功输出0,否则输出非0
      ```
    - ; ----无论前一个命令是否成功，都执行后一个命令
    - && -----前一条成功，执行下一条
    - || -----前一条失败，执行下一条
    - \| -----转义，让后面一个特殊字符失去特殊含义
    ```shell
    echo a \| b # 输出 a | b
    ```
  #### 循环
  ```shell
  for i in 1 2 3
  do 
    echo $i 
  done
  #do done类似c语言大括号
  #输出1\n 2\n 3\n
  ```
  ```shell
  n = 0
  while [ $n -lt 5 ]; #-ls less than, 循环条件是n<5 
  do
  n=$((n+1))  # 等于C语言的n = n + 1;
  echo $n
  done
  ```
 #### 函数
 ```shell
 say()  #直接写函数名
 {
  echo "hello $1"
 }

 say world #调用say函数，传参world
 echo $? #接收最后一条执行命令的退出码
 ```
 #### 管道
  ```shell
  ls | grep txt 
  # ls列出文件，不显示，直接交给grep过滤出含txt的行
  ```
#### 重定向
```shell
echo hello > a.txt #把hello写入a.txt，如果文件不存在就创建，如果存在，就清空之后写入
echo hello >> a.txt #hello追加写入a.txt,不会清空原文档
wc -l < a.txt #wc--t统计字数，-l统计行数，表示，读取a.txt的行数并输出（不用echo就输出） 
```

 ## Git And Github

 ### 用处
  - 版本控制
  - 多人开发，团队协作必用
  - git是当前最流行的版本控制工具
  - git 存的是整个文件的快照，存之前算哈希，一样就存指针，不一样才存文件，zlib压缩，以哈希命名存在.git/objects里，如果是大文件，改得次数多但是每次改得不多，就差异存储，主文件存一份，其他存他跟主文件的差异，这些全打包在一起成为.pack文件存储，gti gc可以强制执行差异存储+压缩（只对文本文件有效）
  ###### SSH密钥配置
  - ssh-keygen -t ed25519 -C
  - cat 地址
  - ssh -T git@github.com
  - git remote set-url origin git@github.com:用户名/仓库名.git
  - git remote -v
### 语法 
 ###### origin = clone的自己的远程仓库的别名
 git clone会自动绑定origin
 ###### upstream = fork过来的原仓库
 上游：本地分支对应的远程分支
  - ==git remote add upstream <老师仓库的URL> ------绑定原仓库用于pull更新==
  - git rename origin upstream ----如果clone下来发现origin绑错了，可以更名
  - git remote -v    -------查看连了哪些仓库
    （会输出4个，两个fetch两个push,origin和upstream）
  - git branch -vv  ------查看当前分支绑定了哪个上游仓库
  - git diff ----对比工作区和暂存区
  - git diff --staged ---暂存区对比最新commit
  - git diff HEAD ----工作区对比上一次commit、
  - git push  ---推送本地分支到远程
    git push -u origin 分支名---第一次push，绑定上游分支
     （-u : 绑定分支与分支，以后在这个分支git push ,git pull默认
  - git pull <网址> <要拉取的远程仓库的分支名> <本地分支名（可以不写，默认当前所在分支）>-----从远程拉取仓库
  git pull = git fetch + git merge
  fetch:只下载，不合并 
  - git branch-----列出所有分支
  - git switch -c 分支名 ---- 创建并切换分支
  - git switch 分支名 ----切换分支，如果远程没有同名分支且本地也没有会报错
  - git merge 分支名  ----合并指定分支到当前分支
  - git branch -d 分支名 ------删除分支
  - git log ----查看历史提交记录
  A..B  ------只看B有，A没有的
  A...B ------看A有B没有加上A没有B有的
  - git log <排除的基准>..<包含的终点> -----"查看在【终点】里但不在【基准】里的提交记录"
  - git reset --soft HEAD~x----撤销上x次commit,退回暂存区
  - git reset --mixed HEAD~x---也是撤销上x次commit,但是退回工作区
  - git reset --soft <某次commit的哈希>----退回哈希对应的commit
  - git branch -r ----展示本地电脑里目前缓存的所有远程分支快照
  eg.
  ```shell
  a@a-VMware:~/missing-semester/2027/ws643/notes$ git branch -r
  origin/HEAD -> origin/master #当前默认分支是master，别人克隆我的仓库默认是master分支，别人看我主页是默认展示master分支，别人给我发pr是默认发在master分支
  origin/master    # 展示本地有的，缓存了的分支
  origin/ws643     # 展示本地有的，缓存了的分支
  #由于没有去upstream上拉取过数据，所以本地没有缓存，没有展示这个
  ```


### worktree
  ### 介绍
  实现多分支并行开发，同一个仓库开出多个工作树（==每个工作树有独立的暂存区，全部存在主库.git文件里==），每个工作树绑定不同分支（git clone下来的库也算一个工作树），每个工作树在实体对应一个文件夹，每个文件夹里的东西可能不一样因为你要改，开出的新工作树不包括.git文件，但是有一个小的.git文件但是只记录路径）在任意一个工作树中执行git push，其他工作树里不会变化,在其中一个工作树里git pull,共享的.git文件会变化当前工作树也会变化，git fetch只有.git文件会变，当前工作树不变，无论是pull还是fetch，其他工作树都不变，想同步得手动merge
  ### 语法
  ####  基于本地已存在的分支创建
  git worktree add <新文件路径> <已存在的分支名>
  一般建议放在库的同级目录下
  #### 切换工作树
  因为工作树是文件
  => 直接 cd 地址
  ==终端换了，别忘了vscode也得换文件夹==
  #### 查看工作树
  - git worktree list----输出每个树的物理路径
  - git worktree list --porcelain -----查看详细信息，用的少
  #### 删除工作树
  ==不能直接rm -rf会有git残留==
  - git worktree remove <路径>
  如果rm -rf了
  git worktree prune
  #### 移动工作树
  - git worktree move <原路径> <新路径>
  ### .git文件
  - objects------每一次commit都存在这
  - refs-----存放指针，记录不同分支都在哪次的commit
  - HEAD----记录当前在哪个分支
  - index---暂存区
  - logs----记录操作历史
  - config---记录当前仓配置，比如远程仓库地址
  - worktree ----如果没建其他树就没有这个文件，建了多个tree会在这个文件夹下出现多个以树的名字命名的子文件夹
    - index---专属暂存区
    - HEAD---记录当前树在哪个分支
    - gitdir---记录当前树的绝对路径
    - commondir---“我的历史数据不在这，去主库.git文件找"

  ### 后悔药
  - git restore . ----撤销还未add的更改
  - git restore 文件名 -----恢复单个文件
  - git restore --staged . ---add之后后悔了用
  - git merge --abort ---终止正在进行的合并
  - git stash---将现在暂未提交的工作暂时收起来
    git stash push -m "备注"
  - git stash -u ---同时存下未add过的文件
  - git stash pop ---取出，删除储藏记录
  - git stash apply ----取出但是不删记录
  - git reflog----查看本地git引用和HEAD曾经指向的提交
  - git reset --hard 哈希 ---将当前分支的指针，暂存区，工作区都重置到哈希对应的提交