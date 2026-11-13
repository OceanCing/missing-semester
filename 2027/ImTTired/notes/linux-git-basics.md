# DynamicX 27赛季控制视觉招新培训第一课-笔记整理

**学习内容：Linux基础、Shell、Vim、Git与GitHub**（提交的有些晚了抱歉ya~）（脚本有使用ai辅助，笔记是人工手打的~）

Author: 26生工王子豪（ImTTired）

- **本科纲要:** 进入 Linux-终端操作-vim编辑-Git提交-GitHub PR
## 一、Linux 基础

### 1. Linux 与开发环境

Windows、macOS 和 Linux 都是主流操作系统

常见的 Linux 发行版：Ubuntu、Debian、Fedora、Arch Linux 

团队选择 Ubuntu，主要考虑 ROS 兼容性、开发环境一致性和现有生态。

机器人开发中，我们可以在开发电脑上编写代码，通过 SSH 连接机器人主机，运行 ROS 节点和控制程序。机器人主机不一定需要图形桌面。

### 2. Linux 文件系统

Linux 的目录从根目录 `/` 开始，路径区分大小写。(表格好难敲啊！)

| 路径 | 含义 |
|---|---|
| `/home` | 用户文件 |
| `/etc` | 系统配置 |
| `/usr` | 程序与库 |
| `/var` | 日志等可变数据 |
| `/dev` | 设备文件 |
| `~` | 当前用户主目录 |
| `.` | 当前目录 |
| `..` | 上一级目录 |

路径分为绝对路径和相对路径：

- 绝对路径：`/home/tom/projects`
- 相对路径：`./hello.txt`

### 3. 终端与 Shell

终端（Terminal）负责提供输入输出界面，Shell 负责解释命令、组织程序执行。

Bash 是常见的 Shell，`ls`、`git` 等是我们通过 Shell 调用的程序。

**理解：** 终端是操作窗口，Shell 是命令解释器，两者并不是同一个概念。

---

## 二、Shell 常用操作

### 1. 命令的基本结构

```bash
命令 [选项] [参数]
```

例如：

```bash
ls -la ~/projects
```

- `ls`：做什么
- `-l`：显示详细信息
- `-a`：包含隐藏项
- `~/projects`：操作哪个对象，目标目录

### 2. 常用命令

| 命令 | 功能 |
|---|---|
| `pwd` | 显示当前工作目录 |
| `ls` | 列出目录内容 |
| `cd 目录` | 进入指定目录 |
| `cd ..` | 返回上一级 |
| `cd ~` | 返回家目录 |
| `mkdir -p` | 创建目录及必要的父目录 |
| `touch` | 创建空文件或更新时间 |
| `cp` | 复制文件 |
| `mv` | 移动或重命名 |
| `rm` | 删除文件 |
| `cat` | 查看文件内容 |
| `less` | 分页查看文件 |

注意：`rm` 删除的文件通常不会进入回收站，删除前先确认路径和文件名。

遇到陌生命令，可以使用：

```bash
ls --help
man ls
```

`Tab` 可以补全命令或路径，上下方向键可以查看历史命令。

### 3. Shell 的四种常用组合

**（1）变量**

```bash
name="RM"
echo "$name"
```

保存一个值，再通过变量引用它，变量用于保存和引用数据，赋值时等号两边不能加空格。

**（2）重定向**

```bash
echo "hello" > hello.txt
echo "world" >> hello.txt
```

- `>`：覆盖写入
- `>>`：追加写入

**（3）管道**

```bash
ls -la | grep txt
```

`|` 将输出交给下一个程序

**（4）条件连接**

```bash
mkdir -p demo && cd demo
```

`&&` 表示前一个命令成功后才执行后一个命令。

**理解：** Shell 的优势不仅是执行单条命令，还能将多个简单命令组合起来，完成比较复杂的任务。

---

## 三、Vim 编辑器

谷歌说：Vim 是一种常用的终端文本编辑器，尤其适合 SSH 远程开发及无桌面环境。

### 1. Vim 的模式

| 模式 | 作用 |
|---|---|
| 普通模式 | 移动、复制、删除等操作 |
| 插入模式 | 输入文本 |
| 命令行模式 | 保存、退出等操作 |

### 2. 常用按键

| 按键 | 功能 |
|---|---|
| `i` | 由普通模式进入插入模式 |
| `Esc` | 返回普通模式 |
| `h j k l` | 左、下、上、右 |
| `dd` | 删除当前行 |
| `yy` | 复制当前行 |
| `p` | 粘贴 |
| `u` | 撤销 |
| `/word` | 搜索 |
| `:w` | 保存 |
| `:q` | 退出 |
| `:wq` | 保存并退出 |
| `:q!` | 不保存直接退出 |

### 3. 基本操作流程

```bash
vim hello.txt
```

按 `i` 进入插入模式，输入内容，然后按 `Esc`，输入 `:wq` 并回车即可保存退出，`dd   u   /word` 删除一行、撤销、搜索


**理解：** Vim 最重要的是掌握模式切换和记忆命令，初次上手，感觉还是与传统Windows有很大区别的

---

## 四、Git 版本控制

### 1. Git 与 GitHub（多人修改同一项目，用版本记录和评审代替互传压缩包）

- **Git**：本地版本控制工具，用于记录提交、比较修改、管理分支。
- **GitHub**：远程代码托管和协作平台，支持 Pull Request、代码评审和合并。


### 2. Git 的四个位置（打箭头不容易啊^_^）

```text
工作区
  ↓ git add
暂存区
  ↓ git commit
本地仓库
  ↓ git push
GitHub 远程仓库
```

- 工作区：正在编辑的文件。
- 暂存区：本次提交的内容清单
- 本地仓库：保存已经提交的版本记录。
- 远程仓库：远程保存的提交历史

**注意：** `commit` 只提交到本地，`push` 才会上传到远程。`git add` 后如果又修改了文件，还需要再次 `add`。

### 3. 常用 Git 命令(第22页ppt)

| 命令 | 作用 |
|---|---|
| `git clone URL` | 下载仓库及提交历史 |
| `git status` | 查看当前分支和文件状态 |
| `git diff` | 查看未暂存的修改 |
| `git log --oneline` | 简要查看提交历史 |
| `git switch -c 分支名` | 创建并切换分支 |
| `git add 文件` | 暂存某文件 |
| `git commit -m "..."` | 记录本地提交 |
| `git push -u origin  feature/hello` | 推送并设置跟踪分支 |
| `git pull` | 获取并整合当前分支的远程更新|


### 4. 分支

Branch（分支）用于隔离不同开发任务，避免直接影响主分支。
main 保持稳定，feature/hello 承载本次修改，评审后再合并

**理解：** 分支让多人能够相对独立地开发，Git 负责记录变化，最后由作者确认是否合并

---

##  SSH 身份认证

SSH 可以使用公钥和私钥进行远程身份认证。使用密钥完成远程认证，私钥保留在自己的电脑上

生成密钥：

```bash
ssh-keygen -t ed25519 -C "your_email"
```

查看公钥：

```bash
cat ~/.ssh/id_ed25519.pub
```

注意：**公钥可以上传到 GitHub，私钥必须保存在本机，不能泄露！！！** 


## 团队工作流

- **Fork**：将团队仓库复制到自己的 GitHub 账号。
- **Branch**：在仓库内创建独立开发分支。
- **Pull Request（PR）**：请求项目维护者评审并合并自己的修改。

培训要求的标准工作流：

```text
团队仓库
   ↓ Fork
个人远程仓库
   ↓ Clone
本地仓库
   ↓ 创建 Branch
修改文件
   ↓ Add / Commit
本地提交
   ↓ Push
个人远程仓库
   ↓ Pull Request
Code Review → Merge
```

**理解：** PR 不只是上传作业，也是一次正式的团队协作过程。代码需要通过评审，遇到意见时应继续在原分支修改，而不是反复关闭和重开 PR。

---

## 实践记录：遇到的问题及解决（实在打不动了，以下ai生成，但都是真的啊！）

本次培训作业不仅学习了 Git 命令，还实际遇到了 Linux 环境与网络问题。

### 问题一：Ubuntu 与 Windows 命令混淆

安装 Ubuntu 后，我在 Ubuntu 终端中执行：

```bash
wsl -l -v
```

结果提示：

```text
wsl: command not found
```

**原因：** `wsl` 是 Windows 侧的管理命令，而当前终端运行的是 Ubuntu Bash。

**解决：** 回到 Windows PowerShell，执行 `wsl -l -v`，确认 Ubuntu 24.04 运行在 WSL2 上。

这让我意识到，虽然 WSL2 可以连接 Windows 和 Linux，但两套环境的命令和路径仍有区别。

### 问题二：GitHub 网络连接超时

第一次执行 `git clone` 时，终端长时间没有反应。

使用：

```bash
curl -I --connect-timeout 10 --max-time 20 https://github.com
```

发现连接超时。

安装 WSL 时也曾出现 localhost 代理配置无法镜像到 NAT 网络的警告。

**排查与解决：**

检查代理环境变量和 Git 全局代理配置后，发现 Ubuntu 中没有配置相应代理。随后关闭 Windows 上正在使用的零点云 VPN，再次测试 GitHub，成功返回：

```text
HTTP/2 200
```

说明 VPN 或本地代理配置很可能影响了 WSL2 的 GitHub 连接。

### 问题三：Git Clone 下载中断

恢复连接后，完整克隆仍然非常缓慢，甚至出现：

```text
RPC failed; curl 92
HTTP/2 stream was not closed cleanly
fatal: early EOF
fatal: fetch-pack: invalid index-pack output
```

这表明 Git 在接收仓库对象时，网络传输异常中断，数据不完整。

**解决方法：**

改用 HTTP/1.1 和浅克隆：

```bash
git -c http.version=HTTP/1.1 clone \
  --depth 1 \
  --single-branch \
  --branch master \
  https://github.com/ImTTired/missing-semester.git
```

最终成功下载 225 个 Git 对象，完成仓库克隆。

其中：

- `http.version=HTTP/1.1`：指定使用 HTTP/1.1。
- `--depth 1`：只获取最近一层提交历史。
- `--single-branch`：只获取指定分支。

**收获：** 网络能够连接并不代表数据传输一定稳定。遇到问题时，应根据具体错误信息排查，并尝试减少不必要的数据传输，而不是一直重复同一条命令。

### 问题四：创建个人开发分支

仓库克隆成功后，使用：

```bash
git remote add upstream https://github.com/OceanCing/missing-semester.git
git switch -c ImTTired
```

成功建立个人开发分支，并通过：

```bash
git branch --show-current
git remote -v
git status
```

确认了当前分支和远程仓库关系。

这让我对 Fork、Branch、origin 和 upstream 的区别有了更直观的认识。

---

## 七、课程总结

本次培训从 Linux 终端操作开始，学习了 Shell 命令组合、Vim 编辑、Git 版本控制及 GitHub PR 协作。

对我而言，最大的收获是初步理解了这些工具如何连接成一套完整的开发流程，而不只是记住单独的命令。

在实际配置环境时遇到的网络故障，也让我体会到：**开发不仅是写代码，还需要能够理解环境、定位错误并找到可靠的解决方案。**

之后希望继续熟悉 Git 分支协作和 Linux 命令行操作，把这些技能应用到控制组的实际开发任务中。