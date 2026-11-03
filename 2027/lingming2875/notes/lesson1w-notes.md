# 27赛季控制视觉招新培训第一课：从 Linux 到第一次 Pull Request
> DynamicX 招新第一课
> 课程目标：学习Linux基础，完成第一次PR，在真实目录练习，将修改提交团队

## 课程安排
| Linux 基础 | Shell 与 Vim | Git & GitHub | 总结与答疑 |
|---|---|---|---|
| 20 分钟 | 25 分钟 | 35 分钟 | 10 分钟 |

## 一 Linux 基础
### Windows、macOS、Linux对比
- **Windows11**：个人桌面，软件游戏生态丰富
- **macOS**：Apple生态，适合日常与开发
- **Ubuntu(Linux)**：服务器、机器人、嵌入式，也可作为桌面系统

### Linux内核与发行版
> Linux一般指**内核**；发行版 = 内核 + 系统工具软件包 + 默认配置

常见发行版：
- Ubuntu：团队选用，ROS兼容性好，部署一致，开发生态完善
- Debian
- Fedora
- Arch Linux

### 机器人开发中的Linux
开发电脑通过网络SSH连接机器人主机；机器人运行Ubuntu Server，无桌面环境。
- ROS节点/控制程序运行在机器人主机
- C++工具链、SSH远程开发、服务自动启动

### Linux 文件目录
- 根目录：`/`，路径**大小写敏感**
- `~`：当前用户家目录
- `.`：当前目录
- `..`：上一级目录

关键目录：
|路径|说明|
|---|---|
|`/home/user`|用户文件|
|`/etc`|系统配置|
|`/usr`|程序与库|
|`/var`|日志等可变数据|
|`/dev`|设备文件|

路径类型
1. **绝对路径**：从`/`开始，例：`/home/user/example`
2. **相对路径**：从当前位置开始，例：`./hello.txt`
3. **家目录简写**：`~/example`

### 终端、Shell、程序
- **Terminal 终端**：输入输出界面
- **Shell(Bash/Zsh)**：解释命令、路径展开、调度执行，我们常用Bash
- **程序/系统**：真正执行功能，如`ls`、`git`

> 演示操作：打开终端输入`ls`，区分终端、Shell、程序。

练习主线：创建`~/example`目录 → 创建`hello.txt` → Vim编辑 → Git记录修改。

## 二 Shell & Vim
### Shell命令格式
命令 [选项] [参数]

示例：`ls -la ~/projects`
- `ls`：命令，做什么
- `-la`：选项，怎样执行；`-l`详细信息，`-a`显示隐藏文件
- `~/projects`：参数，操作对象

> 演示命令：`pwd`、`ls -la`

### 目录切换命令
|命令|作用|
|---|---|
|`pwd`|显示当前所在目录|
|`ls`|列出目录内容|
|`cd 目录名`|进入指定目录|
|`cd ..`|返回上一级目录|
|`cd ~`|回到家目录|

### 文件目录操作
> ⚠️ `rm`删除**不进回收站**，删除前确认路径文件名

|命令|作用|示例|
|---|---|---|
|`mkdir -p`|创建目录，自动补齐父目录|`mkdir -p ~/linux_demo`|
|`touch`|新建空文件 / 更新文件时间戳|`touch hello.txt`|
|`cp`|复制文件|`cp ello.txt hello-copy.txt`|
|`mv`|移动 / 重命名文件|`mv hello-copy.txt backup.txt`|
|`rm`|删除文件|`rm backup.txt`|
|`cat`|直接输出文件内容|`cat hello.txt`|
|`less`|分页查看文件|`less hello.txt`|

### Shell四大常用能力
- 1. **变量**
   name="RM" echo "$name"
- 2. **重定向**
    >：覆盖写入文件 echo "hello" > hello.txt
    >：追加写入文件
- 3. **管道** 
   `|`： 前一个命令输出交给下一个程序 `ls -la | grep txt`
- 4. **条件连接**
   `&&`：前命令执行成功，才执行后面 `mkdir -p demo && cd demo`

- 获取命令帮助

|命令|作用|
|---|---|
|命令 --help|快速查看参数用法，例：`ls --help`|
|man 命令|完整手册，按q退出，例：`man ls`|
|Tab 命令|路径自动补全|
|`↑ / ↓`|翻看历史输入命令|

###  Vim 终端编辑器
>>适用SSH远程、无桌面环境编辑配置、代码；模式编辑器
- 1.三种模式：
   普通模式：  移动光标、删除复制
   插入模式：  输入文本
   命令行模式：保存、退出，普通模式输入:进入

- 2.Vim核心按键

普通模式按键：(按键功能)

|按键|作用|
|---|---|
|`h`` j`` k`` l`|左 / 下 / 上 / 右移动光标|
|`dd`|删除当前整行|
|`yy / p`|复制当前行/粘贴|
|`u`|撤销|
|`:w`|保存|
|`:q`|退出|
|`:wq`|保存并退出|
|`:q!`|放弃修改强制退出|

模式切换：
普通→ 插入：`i`
插入→ 普通：`Esc`

- 3.Vim完整基础流程

|按键|作用|
|---|---|
|`vim hello.txt`|打开文件|
|`i`|进入插入模式输入文字|
|`Esc`|返回普通模式|
|`:wq`|保存退出|

>>再次打开练习：  dd删除行、u撤销、/xxx搜索

## 三.Git & GitHub
1. **介绍** 
- Git:        本地版本控制 
- GitHub:     远程仓库托管、团队协作评审
- 代替互相传压缩包，多人协作修改同一项目

2. **Git四个工作位置**
- 工作区 → git add → 暂存区 → git commit → 本地仓库 → git push → GitHub远程仓库
- 工作区：正在编辑的文件
- 暂存区：准备本次提交的清单
- 本地仓库：本机保存的提交历史
- 远程仓库：GitHub服务器保存代码
- commit只保存到本机；必须push才上传到GitHub。

3. **Git首次配置**
-  设置提交作者信息（写入提交记录）
   `git config --global user.name "Your Name"`
   `git config --global user.email "you@example.com"`
-  查询配置
   `git config --get user.name`
   SSH密钥免密登录GitHub
-  生成密钥
   `ssh-keygen -t ed25519 -C "you@example.com"`
-  查看公钥
   `cat ~/.ssh/id_ed25519.pub`
   公钥（.pub结尾）：复制粘贴到GitHub网页设置
   私钥（id_ed25519）：留在本机，严禁截图、分享、上传
   GitHub网页添加SSH Key
   Settings → SSH and GPG keys → New SSH key
   Title写备注，Key type选Authentication Key，粘贴完整公钥，保存
- 测试连接
  `ssh -T git@github.com`

4. **Git常用命令**

- 获取、查看状态

|命令|作用|
|---|---|
|`git clone <仓库地址>`|下载仓库完整代码和历史|
|`git status`|查看当前分支、文件修改状态|
|`git diff`|查看未暂存的改动|
|`git log --oneline`|简短查看提交历史|

- 分支、提交、推送

|命令|作用|
|---|---|
|`git switch -c feature/hello`|创建并切换新功能分支|
|`git add hello.txt`|将文件加入暂存区|
|`git commit -m "Add greeting"`|生成本地提交，写提交说明|
|`git push -u origin feature/hello`|推送到远程，建立跟踪关系|
|`git switch main`|切回主分支|
|`git pull`|获取远程最新更新|


5. **分支理念**

-  main主分支：保持代码稳定可用
   新建feature/xxx功能分支做开发，修改评审完成后合并进main
   分支让多份开发互不干扰，每一次commit记录一个版本快照

 - 团队Fork协作工作流
   在GitHub上Fork团队仓库，生成属于你个人的副本
   git clone把你的Fork仓库下载到本地电脑
   在本地新建功能分支，修改代码
   git add → git commit → git push推送到你自己的Fork
   GitHub网页发起 Pull Request(PR) 请求合并到原团队仓库
   团队评审Review，通过后Merge合并代码
   如果拥有仓库直接推送权限，可以不用Fork，直接在原仓库建分支

## 四 完整技术链路回顾
- `Linux` :   搭建开发环境
- `Shell` :   终端定位、操作文件
- `Vim`   :   终端编辑修改文件
- `Git`   :   创建分支、暂存、本地提交
- `GitHub`:   发起PR、评审、合并，完成团队贡献

## 五 课后作业任务
- 仓库地址：`https://github.com/OceanCing/missing-semester`
- Fork目标仓库，clone到本地
- 阅读仓库内`2027/TASK.md`，读懂任务要求完成作业
- 使用课程学习的Git工作流，提交PR到目标仓库
- 该仓库后续用于提交其他课程作业
- 拓展学习：catkin、CMake


