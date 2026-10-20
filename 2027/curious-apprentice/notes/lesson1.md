# 第一次培训：Linux、shell && vim、Github

## 1.Linux

### （1）Linux的基本构成
    - Linux内核：负责管理系统的硬件和提供最基本的系统服务，包含设备驱动程序、进程管理、内存管理、文件系统、网络协议栈等关键的底层功能和组件。
    - 系统库：一组用于支持应用程序开发的软件库。
    - shell:命令行解释器。
    - 应用程序：平时使用的软件。
### （2）Linux发行版
    - Linux通常只是指内核，发行版提供可安装和使用的系统。
    - Ubuntu：ROS兼容性、部署一致性和现有的开发生态，是团队使用的重要原因。
### （3）机器人开发中的Linux
    - 开发电脑通过网络连接机器人主机，控制程序可以在无桌面环境运行。
```mermaid
    graph LR
    A([开发电脑]) -- "SSH/网络" --> B([Ubuntu Server]) --> C([ROS节点/控制程序])
```
### (4)Linux特性
    -以/为目录开始，~是用户家目录，.是当前目录，..是上级目录。
    -绝对路径：/开始，相对路径：./开始。
    -文件名含义：home/user(用户文件)，etc（系统配置），usr（程序与库），var（日志等可变数据），dev（设备文件）

---

## 2.shell && vim
    -工作流程：在终端输入命令，由shell解释，再交给程序执行。
```mermaid
     graph LR
    A([Terminal 终端]) --> B([shell(Bash/Zsh)]) --> C([程序/系统])
    - 使用vim原因:在机器人的开发过程中一般没有图形界面，也没有图形化编辑器，只能通过命令行操作。
 ```
### （1）shell
    **命令结构**
    | 命令 | 选项 | 参数 |
    | :--- | :--- | :--- |
    | ls    | -l,-a,-h    |  |
    | cd    |             |  |
    | pwd   |             |  |
    | mkdir | -p(递归建立)|  |
    | ...   |             |  |
    - 参数一般是操作对象。
    -其余常用命令：touch , cp , mv（先旧后新） , rm , cat/less等等。
    
    **常用组合**
    | 名称 | 实例 | 作用 |
    | :--- | :--- | :--- |
    | 变量     | name="RM"  echo "$name"  |保存值，通过变量引用 |
    | 重定向   | echo "hello" > hello.txt | >覆盖写入；>>追加到文件末尾 |
    | 管道     | ls -la \| grep txt       | 将输出交给右侧程序 |
    | 条件选择 | mkdir -p demo && cd demo | 前一条成功后，执行下一条 |
    
    **其他一些命令操作**
    - --help:快速查看选项和用法。
    - man ls:打开手册。
    - Tab   :补全命令或路径。
    - PgUp/PgDn: 调出历史命令。


### （2）vim介绍
    - vim有三种常用模式：命令模式（回车/ESC），插入模式（a,i,o,A,I,O），尾行模式(:)
    - 必备按键：:w（保存） , :q（退出） , :wq（保存退出） , :q!(放弃修改并退出) ， h j k l(左下上右) ， dd（删除当前行) , yy/p(复制当前行/粘贴) ，u（撤销） ， /word（搜索word）
    - 基本使用流程：1.vim 文件名，2.i(进入插入模式)，3.Esc（返回普通模式），:wq(保存退出)。

---

## 3.Github
    - Git:本地版本控制，Github:远程托管与协作
###  (1)Git介绍

```mermaid
    graph LR
    A([工作区]) -- "git add" --> B([暂存区]) -- "git commit -m" --> C([本地仓库]) -- "git push" --> D ([Github 远程仓库])
```
    - Git初始配置：git config--global user.name"" ; git config--global user.email "" 。
    - SSH密钥
