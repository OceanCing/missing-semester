# WZL的培训课笔记

## Linux

### 1.目录与路径
* **根目录**：`/`
    * **用户文件**：`/home/user`
    * **系统配置**：`/etc`
    * **程序与库**：`/usr`
    * **日志等可变数据**：`/var`
    * **设备文件**：`/dev`

注意路径大小写，~表示home目录，.是当前目录，..是上一级。在路径最前面的`/`加上 ~ . .. 分别表示对应含义，不加东西就是从根目录开始。

### 2.终端、Shell与命令
* **结构**：命令 选项 参数

* **常用基础命令**：

| 命令 | 作用 |
|:-------|:----:|
| pwd    | 显示当前目录 |
| ls     | 列出目录内容 |
| cd 目录 | 进入指定目录 |
| cd ..  | 返回上一级 |
| cd ~   | 返回home目录 |

| 命令 |  作用 | 示例 |
|:-----|:------:|:--------:|
| mkdir -p | 创建目录，允许补齐父目录 | mkdir -p ~/linux_demo |
| touch    | 创建空文件/更换时间     | touch hello.txt       |
| cp       | 复制文件              | cp hello.txt hello-copy.txt |
| mv       | 移动或重命名           | mv hello-copy.txt backup.txt |
| rm       | 删除文件              | rm backup.txt |
| cat/less | 直接查看/分页查看       | cat hello.txt |

注意rm不进回收站Trash,删除前先确认路径和文件名，防止误删。

* **Shell 常用组合**：

| 组合 | 作用 | 示例 |
|:-----|:------:|:--------:|
| 变量    | 保存一个值，再通过变量引用它 | name="RM"然后echo"$name" |
| 重定向   | >覆盖写入;>>追加到文件末尾 | echo"hello">hello.txt    |
| 管道    | 将输出交给下一个程序        | ls -la \| grep.txt       |
| 条件连接 | 前一条命令成功后才会执行下一条 | mkdir -p demo && cd demo |

* **求助技巧**：`命令 --help` 或输入`man 命令`打开手册，使用 `Tab` 键补全，`↑` / `↓` 调出历史命令。

### 3.Vim
![Vim模式与必备按键](<Weixin Image_20261022152523_1_3.jpg>)
![Vim基本使用](<Weixin Image_20261022152523_2_3.jpg>)

### 4.Git和Github
常用 Git 命令

    git clone <仓库地址>：下载仓库及提交历史

    git status：查看当前分支和文件状态

    git diff：查看尚未暂存的修改

    git log --oneline：简短格式查看提交历史

    ——————————————————————————————————注意输入命令要用英文输入法

    git switch -c feature/分支名：创建并切换到功能分支（-c表示创建）

    git add laoda.txt：暂存指定文件（是文件要加后缀，是文件夹要加/，文件或文件夹名字有空格左右要加""）

    git commit -m"修改说明":记录一次本地修改，在修改说明里用英语写你做了什么

    git push -u origin feature/分支名：推送并建立跟踪关系

    git switch main：切回主分支

    git pull：获取并整合远程更新

团队 PR 工作流（Fork 模式）

    Fork：在 GitHub 上把团队仓库复制一份到自己的账号

    Clone：把个人的 Fork 克隆到本地电脑

    Branch：创建并切换到新的功能分支，如 git switch -c feature/Laoda666

    。。。。。。开始写代码。。。。。。
    。。。。。。A few moments later。。。。。。

    Commit：保存当前进度在本地（快照存档🤔）

    Push：推送到个人 Fork 仓库（上传云端🤔）

    PR：发起拉取请求，申请合并进团队仓库

    Review：评审

    Merge：负责人同意合并

