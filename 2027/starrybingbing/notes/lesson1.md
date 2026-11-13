# 第一课 学习笔记

## 一. linux 的基础
### 1. linux 的作用
    控制程序序可以在无桌面环境运行
### 2. linux 的目录结构
* **核心目录：**

  * /home/user: 用户文件
  * /etc: 系统配置
  * /usr: 程序与库
  * /var: 日志等可变数据
  * /dev: 设备文件

* **路径概念：**
  
  * **绝对路径**：从`/`开始    (eg:/home/user)
  * **相对路径**：从当前位置开   (eg:./hello.txt)
  * `.`代表当前目录，`..`代表上一级目录，`~`代表主目录(eg:~/example为home目录中的example文件)

  <span style ="color:yellow">***注意：Linux路径大小写敏感***</span>

  例如`Hello.txt`和`hello.txt`是两个文件

---

## 二.终端，shell与命令
### 1.概念对应
* **Terminal** -> 黑框
* **Shell** -> 解释命令、展开路径、组织执行 ( *翻译和执行* )
### 2.命令的结构
*`做什么+怎么做+对谁做`*
### 3.基本命令
* **`pwd`**:显示当前目录（迷路时先用）
* **`ls`**:列出目录内容（习惯加`-la`显示隐藏和详细信息，`-l`是隐藏，`-a`是详细）
* **`cd <文件名>`**:进入指定目录
* **`mkdir`，`mkdir -p`和`touch`**:
  
  | mkdir | mkdir -p | touch |
  | :---: | :---: | :---: |
  | 创建**单级**目录，**无父目录就报错** | **自动补全父目录** | 创建空文件，**若已有文件，不会覆盖，只会更新时间** |
* **`cp`**:copy文件  ***(cp 文件旧 文件新)***
* **`mv`**:移动/重命名 ***(mv 旧 新)***
* **`rm`**:删除<span style="color:yellow">（慎用！！！）</span> ***习惯使用`rm-r`，删除前要看好路径和文件名***
* **`cat`/`less`**:查看文件/大文件用`less`分页查看
### 4.Shell的四种常用组合
* **变量**：先保存一个值，再用变量引用  **(`name="rm" echo "$name"`)**
* **重定向**：>为覆盖写入，>>为追加 **(`echo "hello">> test.txt`)** 
* **管道`|`**：将前一个命令的输出交给下一个程序 **(`ls-la | grep txt`)**

  #### 小知识：grep txt ：即查找名字里带有txt的文件
    |  常用后缀 | 效果 | 例子 |
    | :--- | :--- | :--- |
    | grep -i | 忽略大小写搜索  | `grep -i "error"`把ERROR和error都找出来 |
    | grep -v | 反向搜索 |`ls -la \| grep -v ".log"`列出除.log以外的 |
    | grep -r | 递归搜索从父到子
    | grep -w | 全字匹配 | 防止搜`cat`时搜出`category` |
    <span style="color:pink">后缀可以连写 ，eg：grep -iw为忽略大小写并全字匹配 </span>
* **条件连接**：前一条成功后再执行下一条 **(`mkdir -p demo && cd demo`)**

  #### <span style="color:yellow">陌生命令查找帮助：</span>
  * **`ls --help`**:快速查找选项和用法
  * **`man ls`**:打开手册，按q退出
  * **`Tab`**:补全命令或路径
  * **`↑/↓`**:看历史记录

---

## 三.Vim 编辑器
* **普通模式**：

  * **移动**：`h j k l` 左/下/上/右
  * **删除**：`x`删去光标所在的单个字符，`dd`删去光标所在的一整行
  * **复制，粘贴**：`yy`/`p`，复制当前行/粘贴
  * **撤销**：`u`
  * **搜索**：`/`**(eg:/word 搜索word)**

* **插入模式**：按`i`进入，`Esc`返回普通模式
* **命令行模式**：`:w`保存，`:q`退出，`:wq`保存退出，`:q!`放弃修改退出（改乱了用这个）

---

## 四.Git 和 Github
### **1.分工**：
  * **Git**：记录提交，变更历史，创建新分支
  * **Github**：托管Git仓库，用于团队合作，讨论和评审

### **2.流转的四个位置**：
| 工作区 | 暂存区 | 本地仓库 | Github远程仓库 |
| :--- | :--- | :--- | :--- |
| 正在编辑的文件-> | 本次提交的内容清单-> | 已记录的提交历史-> | 远程保存的提交历史 |
| git add | git commit | git push |

  ***add 到暂存 , commit到本地仓库 ， push到远程***

### **3.git的常用指令**：
* **获取和查看**：

  * **`git clone<地址>`**：下载仓库和提交历史
  * **`git status`**：查看分支和文件状态
  * **`git log --oneline`**：简短看提交历史
  * **`git diff`**：查看未暂存的修改

* **分支和提交**：<span style="color:pink"> 分支让开发互不打扰 </span>

  * **`git switch -c <demo>`**：创建并切换到demo分支
  * **`git switch main`**：切换到main分支
  * **`git pull`**：获取加自动合并

    <span style="color:yellow">**切分支前前先看status**</span>

* **安全底线**：
   <span style="color:red">公钥（.pub）可公开，私钥不可公开</span>

---

## 五.PR工作流
* 1.**Fork**：在 GitHub 上复制团队仓库到个人账号
* 2.**Clone**：下载到本地
* 3.**新建分支**:`git switch -c <文件>` <span style="color:yellow">（**切忌直接在 main 分支改代码**）</span>
* 4.**提交**:`add`->`commit`
* 5.**推送**：`git push` 到个人的 GitHub 仓库
* 6.**发起 PR**：请求团队 **review**，审核通过后 **merge** 进主分支