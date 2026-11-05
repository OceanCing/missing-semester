# Lesson 01: Linux, Shell, Vim, Git and Github

Before the lesson notes: Markdown 语法

- **标题（分级）**：`# 一级` `## 二级` `### 三级`，`#` 后面必须空一格
- **无序列表**：`- 项目` 或 `* 项目`，短横后也空一格
- **有序列表**：`1. 第一步`，序号可以全都写 `1.`，渲染时自动编号
- **加粗、斜体**：`**粗**` `*斜*`，用半角星号
- **行内代码**：`ls -la`，用反引号包起来，代码里的记号不会被当成排版
- **代码块**：首尾各三个反引号包住命令，成对出现；开头后可以跟语言名，如 `bash`、`cpp`。例如 `bash` 告诉渲染器“里面是 bash 脚本，请用 bash 语法上色”
- **引用、提示**：`> 注意：…`，`>` 后面有空格
- **链接**：`[显示文字](https://…)`，方括号在前、圆括号在后
- **表格**：第一行表头，第二行 `|---|---|` 分隔，第二行那一排横线不能省
- **分隔线**：`---` 单独一行

## 1. Linux

### 1.1 常见系统：Windows、macOS、Linux

- Linux：服务器，机器人（ROS），嵌入式；也可以作为桌面系统

### 1.2 Linux 与发行版

**Linux 通常指内核**，发行提供一套可以安装和使用的系统：内核 + 系统工具与软件包 + 默认配置。

常见发行版：Ubuntu、Debian、Fedora、Arch Linux。

团队使用 **Ubuntu**：ROS 兼容性、部署一致性和现有开发生态。

### 1.3 机器人开发里的 Linux

开发电脑通过网络或 SSH，连到机器人主机（常是 Ubuntu Server），控制程序或 ROS 节点在**没有桌面**的环境里运行。

包含内容：C++ 工具链、SSH 远程开发、服务自动启动。

### 1.4 Linux 的目录与路径

- `/`：根目录，存放系统与本身安装的应用
- `/home/user`：家目录，存放用户文件
- `/etc`：系统配置
- `/usr`：程序与库
- `/var`：日志等可变数据
- `/dev`：设备文件

- **绝对路径**：从 `/` 开始写，例如 `/home/user/example`
- **相对路径**：从当前位置开始算，例如 `./hello.txt`
- `~` 是当前用户的家目录，`.` 是当前目录，`..` 是上一级目录
- Linux **区分大小写**，`Hello.txt` 和 `hello.txt` 是两个文件

## 2. Shell

### 2.1 终端、Shell 与命令

- **Terminal 终端**：输入输出界面
- **Shell（Bash/Zsh）**：解释命令、展开途径、组织执行
- **程序/系统**：`ls` 查看文件，`git` 管理版本

### 2.2 命令的结构

- **命令** `ls`：要做什么 —— 列出目录
- **选项** `-la`：怎样执行 —— `-l` 详细信息，`-a` 包含隐藏项
- **参数** `~/projects`：操作哪个对象 —— 目标目录

### 2.3 目录与文件操作

| 命令 | 作用 | 示例 |
|---|---|---|
| `pwd` | 显示当前目录 | `pwd` |
| `ls` | 列出目录内容 | `ls -la` |
| `cd` | 切换目录 | `cd ~/linux_demo` |
| `mkdir -p` | 创建目录，自动补齐父目录 | `mkdir -p ~/linux_demo` |
| `touch` | 创建空文件或更新时间 | `touch hello.txt` |
| `cp` | 复制文件 | `cp hello.txt hello-copy.txt` |
| `mv` | 移动或重命名 | `mv hello-copy.txt backup.txt` |
| `rm` | 删除文件 | `rm backup.txt` |
| `cat` | 直接输出文件内容 | `cat hello.txt` |
| `less` | 分页查看，按 `q` 退出 | `less hello.txt` |

完整流程：

```bash
mkdir -p ~/linux_demo
cd ~/linux_demo
touch hello.txt
cp hello.txt hello-copy.txt
mv hello-copy.txt backup.txt
cat backup.txt
rm backup.txt
```

> 注意：`rm` 默认不进回收站，删掉就没了。执行前先用 `pwd` 和 `ls` 确认路径与文件名。

### 2.4 Shell 四种常用组合

1. **变量**：先存一个值，再用 `$` 引用它

   ```bash
   name="RM"
   echo "$name"
   ```

2. **重定向**：`>` 覆盖写入，`>>` 追加到末尾

   ```bash
   echo "hello" > hello.txt
   echo "again" >> hello.txt
   cat hello.txt
   ```

3. **管道**：把左边命令的输出交给右边命令处理

   ```bash
   ls -la | grep txt
   ```

4. **条件连接**：前一条成功才执行后一条

   ```bash
   mkdir -p demo && cd demo
   ```

**辨析**

| 写法 | 执行逻辑 |
|---|---|
| `cd ... && vim ...` | `cd` 失败就不执行 `vim` |
| `cd ... ; vim ...` | 不管 `cd` 成败，都执行 `vim` |
| `cd ... \|\| vim ...` | `cd` 失败才执行 `vim`（反向逻辑） |

### 2.5 陌生命令的帮助入口

| 命令 | 作用 |
|---|---|
| `ls --help` | 快速查看选项和用法 |
| `man ls` | 打开手册，按 `q` 退出 |
| `Tab` | 补全命令或路径 |
| `↑` / `↓` | 调出历史命令 |

## 3. Vim：在终端中编辑文件

### 3.1 三种模式

Vim：**有模式的编辑器**，同一批按键在不同模式下含义不同：

- **普通模式**：默认进入，用于移动、删除、复制
- **插入模式**：按 `i` 进入，用于输入文本
- **命令行模式**：普通模式下按 `:` 进入，用于保存、退出

### 3.2 必备按键

| 按键 | 作用 |
|---|---|
| `i` | 进入插入模式 |
| `Esc` | 回到普通模式 |
| `:w` | 保存 |
| `:q` | 退出 |
| `:q!` | 放弃修改并退出 |
| `h` `j` `k` `l` | 左/下/上/右 |
| `dd` | 删除当前行 |
| `yy` / `p` | 复制当前行 / 粘贴 |
| `u` | 撤销 |
| `/word` | 搜索 word，按 `n` 跳下一个 |

> 注意：先在终端敲 `:wq` 会提示 command not found —— `:` 开头的命令只能在 Vim 的普通模式里输入。同理，`i`、`gg=G` 也不是终端命令。

## 4. Git 与 Github 的分工

### 4.1 分工

| 工具 | 作用 |
|---|---|
| Git | 本地版本控制：记录提交、比较修改、查看历史、创建分支 |
| GitHub | 远程托管与协作：托管仓库、通过 PR 讨论评审、合并修改 |

### 4.2 四个位置

```text
git add --> git commit --> git push
 工作区  -->   暂存区   --> 本地仓库 --> Github 远程仓库
```

| 位置 | 含义 |
|---|---|
| 工作区 | 正在编辑的文件 |
| 暂存区 | 本次要提交的内容清单 |
| 本地仓库 | 已记录的提交历史 |
| 远程仓库 | GitHub 上保存的提交历史 |

即：`commit` 只写进本地仓库，`push` 才发送到远程；`pull` 把远程更新取回本地。

### 4.3 首次使用 Git：提交作者信息与 SSH 认证

设置提交作者信息：

```bash
git config --global user.name "s7ar4"              # 设置用户名
git config --global user.email "you@example.com"   # 设置邮箱地址
git config --get user.name                         # 获取用户名
```

生成 SSH 密钥并读出公钥：

```bash
ssh-keygen -t ed25519 -C "you@example.com"
cat ~/.ssh/id_ed25519.pub
```

把 SSH 公钥添加到 GitHub：

1. 复制 `id_ed25519.pub` 的全部内容
2. 打开 GitHub 的 Settings → SSH and GPG keys → New SSH key
3. 类型选 Authentication Key，填写名称，粘贴公钥

检查密钥是否成功设置：

```bash
ssh -T git@github.com
```

- `id_ed25519.pub` 是**公钥**
- `id_ed25519` 是**私钥**，不上传、不截图、不发群
- `ssh -T git@github.com` 首次连接要核对 GitHub 官方主机指纹，成功后显示 `Hi <用户名>!`

### 4.4 常用命令

获取与查看：

| 命令 | 作用 |
|---|---|
| `git clone <仓库地址>` | 下载仓库及提交历史 |
| `git status` | 查看当前分支和文件状态 |
| `git diff` | 查看尚未暂存的修改 |
| `git log --oneline` | 简短格式查看提交历史 |

分支与提交：

| 命令 | 作用 |
|---|---|
| `git switch -c feature/hello` | 创建并切换到新分支 |
| `git add hello.txt` | 暂存指定文件 |
| `git commit -m "Add greeting."` | 记录一次本地提交 |
| `git push -u origin feature/hello` | 推送并建立跟踪关系 |
| `git pull` | 取回并整合远程更新 |

### 4.5 分支

分支是为新功能保留的独立开发线：`main` 保持稳定，`feature/hello` 承载本次修改，评审通过后再合并。同一仓库里的多条开发线互不打扰，提交点记录每一步修改。

```bash
git switch main               # 切回主分支
git pull                      # 先同步远程
git switch -c feature/hello
```

> 注意：切分支前先 `git status`，确认没有未提交的修改，否则修改会跟着你到新分支。

### 4.6 团队 Fork 协作流程

1. **Fork**：在 GitHub 上把团队仓库复制到自己的账号下
2. **Clone**：`git clone git@github.com:<你的用户名>/<仓库名>.git`
3. **Branch**：`git switch -c <分支名>`，不要直接改主分支
4. **Commit**：`git add` 选好本次内容，再 `git commit -m "..."`
5. **Push**：`git push -u origin <分支名>`
6. **PR**：在自己的仓库点 Compare & pull request，向团队仓库发起合并请求
7. **Review**：按评审意见继续在同一分支上修改，PR 会自动更新
8. **Merge**：评审通过后由维护者合并

> 注意：Fork 是仓库副本，Branch 是仓库内的开发线，两者不能混着说。有团队仓库写权限时才可以省掉 Fork。



## 5. 提交项目的详细流程

### 5.1 阶段 A · 准备

#### A1 fork（网页）

对方仓库页面右上角点 **Fork** → 你的账号下多出一份副本。

不然后面 push 会失败：对别人的仓库没有写权限。

#### A2 克隆自己那份（终端）

```bash
git clone https://github.com/s7ar4/仓库名.git
cd 仓库名
```

输出示例：

```text
正克隆到 '仓库名'...
完成。
```

#### A3 确认远程地址（终端）

```bash
git remote -v
```

输出示例：

```text
origin  https://github.com/你的用户名/仓库名.git (fetch)
origin  https://github.com/你的用户名/仓库名.git (push)
```

约定俗成：`origin` = 自己的仓库；`upstream` = 原作者的仓库。

### 5.2 阶段 B · 开分支改文件

#### B1 建并切到新分支（终端）

```bash
git switch -c fix-notes
```

输出示例：

```text
切换到一个新分支 'fix-notes'
```

> 不要在 `main` 上直接改：`main` 保持和原仓库一致，改动全在分支上，PR 才干净、好重提。

#### B2 改完看状态（终端）

```bash
git status
```

```text
修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）
```

#### B3 暂存 + 提交（终端）

```bash
git add notes.md
git commit -m "Add notes."
```

`add`：将提交的改动放进暂存区；`commit`：本地存档。

#### B4 看存档点（终端）

```bash
git log --oneline
```

### 5.3 阶段 C · 推送到 fork（终端）

```bash
git push -u origin fix-notes
```

`-u` 只需第一次：之后在这条分支上再推，直接 `git push`。

保险工作，推之前确认这次 PR 到底带过去什么：

```bash
git diff --stat main fix-notes
```

### 5.4 阶段 D · 提 PR（网页）

1. 打开 fork，点击页面上 **Compare & pull request**
2. 核对两栏：`base: 原仓库/main` ← `head: 你的用户名/fix-notes`
3. 填标题和说明（改了什么、为什么改）→ **Create pull request**

> PR 是「请求」，不是合并本身。维护者点 Merge 之后才算进入原仓库。




写在最后：笔记我先将PPT上的内容按顺序过一遍后，加入了我之前问ai的预习内容，在学习提交项目的流程中，我也多次询问了ai，把ai的回答与我的实际经历也整理在了上面。关于使用的markdown语法，我先将第一版喂给ai，再根据正确格式进行修改。如果有哪里不恰当或错误，恳请学长学姐们指出，十分感谢！
