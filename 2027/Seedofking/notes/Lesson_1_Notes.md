#### 前言:
###### 先前一直有做笔记的习惯, 所以现在将已有笔记搬到了notes文件夹中, 并摘取了部分与第一节课相关的笔记放在本节课的笔记

## Linux

#### Linux系统特性:
1. 开源
2. 透明, 可以知道任何运行的程序
3. 设备、硬盘本质上都是文件夹

#### Linux目录:
##### 从 / 开始
/home/user: 用户文件
/etc: 系统配置
/usr: 程序与库
/var: 日志等可变数据
/dev: 设备文件

![目录](https://bee-reg-ab.imagency.cn/mr/202610/20/6ad6e2486d9fe.png "目录")

## Shell

### 帮助
##### 在命令后加上-h或者-help可以查看这个命令可以加入什么参数或者这个参数有什么用
    xxx -h / xxx -help					
##### 使用man + 命令也可以查看手册, 有命令的参数形式和英文解释
    man xxx						

### 调出历史
###### 可以调出上次命令
    up down 						

### 目录操作：
##### 终端打开后默认是用户目录 /home/seed 用户目录，也用 ~ 来表示
##### 用户目录的上一层是 /home 目录
##### 再上一层是根目录 /
##### 使用cd可以改变当前目录, 进入指定目录
    cd 地址						
##### . 表示当前目录, 进入当前目录的xxx文件
    cd ./xxx 						
##### .. 表示上一级目录, 进入上一级目录
    cd ..							


### 文件操作:

#### 创建:
##### 在当前目录创建xxx文件
    mkdir xxx					
##### 在 /home/seed/文档/ 地址下创建名为Ohyeah的文件	
    mkdir /home/seed/文档/Ohyeah	
##### 在当前目录创建名为xxx的.txt文件
    touch xxx.txt					
##### 在 /地址 下创建名为xxx的.txt文件, 若已存在则可以更新文件时间
    touch /地址/xxx.txt				

#### 复制:
    cp hello.txt hello-copy.txt

#### 移动或重命名:

##### 重命名文件
    mv hello.txt hello-yeh.txt

##### 移动文件到另一个目录:
###### 已在该目录的文件前路径可省略, 移动到另一个目录注意是 路径/ , 最后空着表示目录
    mv file.txt /home/user/documents
    mv mv /home/seed/文档/Markdown/Markdown_Library.md /home/seed/文档/命令/
##### 移动并重命名:
    mv file.txt /home/user/documents/newfilename.txt
##### 移动多个文件到目录:
    mv a.txt b.txt c.txt /home/user/


### 命令行环境
###### 值得注意的是, shell不会保存别名, 如果想让别名持续生效, 就需要放进 .bashrc 或者 .zshrc
    alias alias_name="command_toalias arg1 arg2"		#让alias_name等价为 "命令"
    alias sl="ls"						#让sl等价为ls

### 数据整理
##### 输出命令行的日志
    cat .bash_history			
##### 使用管道运算符 | 在.bash_history输出的命令中执行grep apt, 查找含有apt的日志		
    cat .bash_history | grep apt				
##### 使用less可以创建一个文件分页器, 可以通过翻页的方式浏览较长的文本
    cat .bash_history | grep apt | less 			
##### sed: 基于文本编辑器ed, 可用一些的简短的命令修改文件, ''是正则表达式
###### 其中的s命令是替换命令, 实现把file1中匹配Dynamicx的文字替换为dx, /g则会更改每一个匹配项
    cat file1 | sed 's/DynamicX/dx/g'			

### 常用组合
#### 变量
    name = "RM"						#保存一个值, 再通过变量引用它
    echo $(name)

#### 重定向
    echo "hello" > hello.txt				#>覆盖写入; >>在末尾追加
    echo "world" >> hello.txt

#### 管道 
    ls -la grep txt						#将输出交给下一个程序

#### 条件连接
    mkdir -p demo && cd demo				#前一条执行成功后, 执行下一条

## Vim

#### 打开Vim编辑flie(若已存在, 不存在就会创建): 
    Vim file

##### 进入Vim之后, 先按 i , 即可开始编辑
##### 编辑之后, 按Esc退出编辑模式, 进入普通模式
##### 按wq保存并退出, 按q!不保存并退出
##### dd 删除一行、撤销、搜索

# ***Git & Github***

### 初始化：

设置Git用户名：
`git config --global user.name "用户名"`
设置Git邮箱：
`git config --global user.email "邮箱"`

### 初次使用：

#### 第一步：

###### 新建：初始化git仓库，在该文件夹生成隐藏的 .git 文件夹

`git init`

###### 克隆：将别人的Github仓库克隆到本地

`git clone https://github.com/用户名/仓库名.git`

#### 第二步：将文件加入暂存区

###### 所有文件：

`git add .`

###### 加单个文件：

`git add main.cpp`

###### 加多个指定文件：(多个文件使用空格隔开，文件名带空格的话用引号包裹“My file.txt”)

`git add main.cpp test.cpp README.md`

###### 文件格式匹配：

*cpp:
`git add *.cpp`
*h:
`git add src/*.h`

#### 第三步：提交到本地仓库

`git commit -m "提交的备注"`

#### 第四步：关联远程Github仓库（只需执行一次）

`git remote add origin https://github.com/用户名/仓库名.git`

###### 移除仓库绑定的方法：

`git remote remove <旧名> （旧名可能是origin, 用git remote -v看）`

##### （获取链接：进入仓库后，展开Code可以获得仓库的链接）

![Github截图](https://bee-reg-ab.imagency.cn/mr/6001/26/40a994dddd706077f63790bf0b2543f0.png "“展开绿色的Code可以获取仓库链接”")
`git remote add origin https://github.com/Seedofking/C.git`

###### 查看远程仓库是否绑定成功：

`git remote -v`

#### 第五步：推送到Github远程仓库

`git push -u origin main`

###### 如果报错 Does not match any, 就换成`git push -u origin master`，因为Git旧版本分支默认叫 master

###### 使用`git config --global init.defaultBranch main`可以把本地默认分支改成main

###### 使用`git branch`可以查看本地所有分支

### 后续提交：

#### 第一步：文件加入缓存区

`git add .`/`git add mian.cpp`/`git add main.cpp test.cpp README.md`/`git add *.cpp`/`git add src/*.h`

###### 取消加入缓存区
`git reset HEAD .`

#### 第二步：提交到本地仓库

`git commit -m "更新xxx/新增xxx"`

#### 第三步：提交代码

`git push`

##### 如果出现 The upstream branch of your current branch does not much:(本地分支名与上游分支名不匹配)

###### 临时方案：把本地当前 HEAD（也就是本地 master 分支）推送到远程仓库的main分支

`git push origin HEAD:main`,但是执行该命令不会绑定上游分支，下次push还是要写完整命令

###### 绑定上游方案：

`git push -u origin HEAD:main`,使用-u绑定上游后下次就可以直接git push

##### 修改本地分支名方案：

###### 第一步：将本地master分支名重命名为main

`git branch -m master main`

###### 第二步：将本地main分支绑定远程origin main并推送

`git push -u origin main`

### 通用字符解释：

#### -u

##### 绑定上游分支：

`-u`等价于`--set-upstream`,用于git push
upstream = 远程仓库上对应的那条分支
`git push -u origin master:main`执行完之后就会把下游分支branch绑定上游分支main，下次就不用再写完整命令
本地与github同名就写`git push -u origin main`
以后可以直接写`git push`推送，`git pull`拉取

#### -m

##### 1.提交信息：

`-m`等价于`--message`
`-m "备注"`可以直接在命令行写本次提交的说明文字，跳过vim编辑器
`git commit -m "备注"`

##### 2.移动/重命名分支：

此时`-m`等价于`--move`,作用是重命名分支
`git branch -m master main`将旧分支名字master重命名为新分支名字main



## 后续管理:

### 日志和状态查看:
#### 查看工作区状态:
`git status`

##### 查看提交历史:
`git log`

##### 查看全部(包含其他分支)的提交历史
`git log --all`

##### 显示提交的具体更改内容
`git log -p`

##### 以图形化方式显示分支和合并历史
`git log --graph`

##### 以列表形式查看指定文件的历史修改记录
`git blame test1.txt`

##### 一行显示简短提交信息
`git log --oneline`

##### 全部, 一行, 图形化地显示日志信息 
`git log --all --oneline --graph`

##### 尚未缓存的改动:
`git diff`

##### 查看已缓存的改动:
`git diff --cached`

##### 显示摘要而非整个diff:
`git diff --stat`

##### 显示暂存区和工作区的差异:
`$ git diff [file]`

##### 显示暂存区和上一次提交(commit)的差异:
`$ git diff --cached[file]`

##### 显示两次提交之间的差异 (指定某文件):
`$ git diff [front-branch-id] [back-branch] ( -- src/main.c)`

### 删除
###### 手动从目录删除文件运行`git status`的时候会有`Changes not staged for commit`的提示

##### 将文件从暂存区和工作区删除:
`git rm test.txt`
###### 删除后通过commit将删除提交

##### 将先前已经更改并提交到暂存区的文件强制删除, 删除修改后的文件
`git rm -f test.txt`

##### 仅把文件从暂存区删除
`git rm --cached test.txt`

### 分支管理

###### 切换分支之前, 要`git add` + `commit`, or `git stash`否则将会把工作区的改动带入下一个分支, 造成污染

##### 查看现在有什么分支
`git branch`
###### `* b1` 意为现在正处于b1分支上

##### 创建并切换到一个分支
`git checkout -b Branch2`

##### 切换分支
`git checkout main`

##### 合并分支
`git checkout main1`
`git merge feature-xyz`
###### 合并分支前需要先切换到分支, 合并会把merge后的feature-xyz分支合并到当前所处的main1分支

#### 冲突处理
###### 冲突状态下不要随意切换分支, 建议`git merge --abort`取消合并
###### git合并时, 若主分支没有修改而只有branch有修改, 则主分支会直接接上新的修改
###### 若主分支和新分支都有修改, 则可能会产生冲突, 需要手动修改文件解决冲突
###### 可以用`git status`查看哪里有冲突
###### 打开冲突文件时, 可以发现git插入冲突标记, 有当前分支代码和合并分支的代码, 需要手动编辑留下的部分
###### 冲突解决后, `git add`添加有冲突的文件, `git commit`即可默认添加合并时的注释

##### 取消合并
###### 若看见冲突后不想再合并, 则可以终止合并, 回到合并前的状态
`git merge --abort`

##### 图形化冲突合并处理工具
###### 打开可视化界面, 左右分别展示两个分支版本, 中间是最终结果, 点选保留内容
`git mergetool`

#### 删除分支

##### 删除本地分支
`git branch -d b1`

##### 强制删除未合并的分支
`git branch -D b2`

##### 删除远程分支
`git push origin --delete b3`

### 回退版本

##### 回退所有内容到上一个版本
`git reset HEAD^`

##### 回退指定文件到上一个版本
`git reset HEAD^ test2.txt`

##### 回退到上上上个版本
`git reset --soft HEAD-3`

##### 软回退到指定版本
###### 如果该版本没有某个文件, 那么那个文件在工作区仍存在但不包含在git中
`git reset --soft commit_id`

##### 硬回退到指定版本
`git reset --hard commit_id`

### 删除仓库

#### 方法:1安装gh工具, 并完成添加密钥, 配置源和安装
```
(type -p wget >/dev/null || (sudo apt update && sudo apt install wget -y)) \
&& sudo mkdir -p -m 755 /etc/apt/keyrings \
&& out=$(mktemp) && wget -nv -O$out https://cli.github.com/packages/githubcli-archive-keyring.gpg \
&& cat $out | sudo tee /etc/apt/keyrings/githubcli-archive-keyring.gpg > /dev/null \
&& sudo chmod go+r /etc/apt/keyrings/githubcli-archive-keyring.gpg \
&& sudo mkdir -p -m 755 /etc/apt/sources.list.d \
&& echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null \
&& sudo apt update \
&& sudo apt install gh -y
```

##### 授权(只需一次)
`gh auth refresh -s delete_repo`

##### 删除指定仓库
`gh repo delete 用户名/仓库名`
`gh repo delete Seedofking/missing-semester`

#### 方法2:github仓库内操作

##### 进入仓库, 在上面一栏中找到设置
![进入仓库设置](https://bee-reg-ab.imagency.cn/mr/202610/20/6ad6e1bd284cc.png "进入仓库设置")

##### 在设置中一直往下滑, 最下面的Danger Zone中就有删除仓库(不可恢复)
![删除仓库](https://bee-reg-ab.imagency.cn/mr/202610/20/6ad6e1f154def.png "删除仓库")

### 团队协作:

#### 创建分支:
###### 团队协作时若自己没有仓库的推送权限, 则需要用Fork创建分支, 若有则可以直接先pull再push至仓库
###### fork分支的作用是将别人的仓库复制一份到自己的仓库, 这样就可以在git clone编辑后通过pull request申请合并到别人仓库
###### 若直接git clone别人的仓库, 只是把代码复制到本机, 没有复制为自己仓库, 则不能推送和申请PR

##### 申请fork:
![fork](https://bee-reg-ab.imagency.cn/mr/202610/20/6ad6e21542911.png "fork")

##### 申请了fork之后可以在自己仓库找到, 找到再克隆到本机即可, 之后就会在remote绑定该仓库
![clone](https://bee-reg-ab.imagency.cn/mr/202610/20/6ad6e21a2cd1a.png "clone")

##### 申请pull request
###### 如果有冲突需要自己处理, 或者联系仓库持有者拉取自己仓库进行修改、请求指导如何修改, 不可以提交带有冲突的PR
###### 仓库持有者通过了Merge之后, 自己的PR就可以进入持有者的仓库中
![PR](https://bee-reg-ab.imagency.cn/mr/202610/20/6ad6e1a0b204d.png "PR")

