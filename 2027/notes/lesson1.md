# 一、Linux
- linux通常指linux内核
- Ubuntu为发行版(ROS兼容性，部署一致性和现有开发生态)
- 机器人开发中的Linux(开发电脑通过网络连接机器人主机，控制程序在无桌面环境运行)
- 目录从/开始
**相对路径**:./hello.txt
**绝对路径**:/home/user/example ~/example(home目录中的example)

# 二、Shell命令
## 2.1**在终端输入命令，由Shell解释，再交给程序执行**
## 2.2Shell命令结构
**ls -la ~/projects**
ls是命令，-la怎样执行，~/projects操作对象
## 2.3目录切换
|命令|作用|
|---|---|
|`pwd`|显示当前目录|
|`ls`|列出目录内容|
|`cd 目录`|进入指定目录|
|`cd ..`|返回上一级|
|`cd ~`|返回家目录|

## 2.4文件和目录操作
| 命令 | 作用 | 示例 |
|---|---|---|
| `mkdir -p` | 创建目录，必要时补齐父目录 | `mkdir -p ~/linux_demo` |
| `touch` | 创建空文件或更新时间 | `touch hello.txt` |
| `cp` | 复制文件 | `cp hello.txt hello-copy.txt` |
| `mv` | 移动或重命名文件 | `mv hello-copy.txt backup.txt` |
| `rm` | 删除文件 | `rm backup.txt` |
| `cat` | 直接查看文件内容 | `cat hello.txt` |
| `less` | 分页查看文件内容 | `less hello.txt` |

## 2.5 Shell常用组合

### 变量
```bash
name="RM"
echo "$name"
```
### 重定向
```bash
echo "hello“ > grep txt
```
>覆盖写入，>>追加到文件最后

### 管道
```bash
ls -la | grep.txt
```
- `|`将前一个输出传递给下一个程序
- `grep txt`筛选包含txt的文本

### 条件连接
```bash
mkdir -p demo && cd demo
```
前一条执行完再执行下一条

## 2.6陌生命令帮助
| 操作 | 用途 |
|---|---|
| `ls --help` | 查看命令用法和选项 |
| `man ls` | 打开命令手册 |
| `Tab` | 自动补全命令或路径 |
| `↑` / `↓` | 调出历史命令 |

# 三、Vim

## 3.1在终端中编辑程序，常用于ssh和无桌面环境配置、代码编辑

## 3.2Vim 的三种基本操作状态
| 模式 | 主要用途 |
|---|---|
| 普通模式 | 移动、删除、复制、搜索 |
| 插入模式 | 输入文本 |
| 命令行模式 | 保存、退出等命令 |

## 3.3常用
| 按键或命令 | 作用 |
|---|---|
| `h` | 向左移动 |
| `j` | 向下移动 |
| `k` | 向上移动 |
| `l` | 向右移动 |
| `i` | 进入插入模式 |
| `Esc` | 返回普通模式 |
| `dd` | 删除当前行 |
| `yy` | 复制当前行 |
| `p` | 粘贴 |
| `u` | 撤销 |
| `/word` | 搜索 word |
| `:w` | 保存 |
| `:q` | 退出 |
| `:wq` | 保存并退出 |
| `:q!` | 放弃未保存的修改并退出 |

## 3.4vim基本使用
1. 按 `i`，进入插入模式。
2. 输入一段文本。
3. 按 `Esc`，返回普通模式。
4. 输入 `:wq`，保存并退出。
5. 再次打开文件：

```bash
vim hello.txt
```
6. 按 `dd` 删除当前行。

# 四、Git和GitHub

## 4.1Git：本地版本控制

### 记录每次提交；比较修改、查看历史；为新功能创建分支

## 4.2GitHub：远程托管和协作

## 4.3流程
1. 提交作者信息
`git config --global user.name"yourname"`
`git config --global user.email"your@zeample.com"`
2. `git add`（工作区->暂存区）
3. `git commit`(暂存区->本地仓库）
4. `git push`(本地仓库到GitHub）
5. SSh认证

## 4.4Git常见命令
获取和查看
| 命令 | 作用 |
|---|---|
| `git clone <仓库地址>` | 下载仓库及其提交历史 |
| `git status` | 查看当前分支和文件状态 |
| `git diff` | 查看尚未暂存的修改 |
| `git diff --cached` | 查看已经暂存的修改 |
| `git log --oneline` | 简洁地查看提交历史 |

分支与提交
|命令|作用|
|---|---|
|`git switch -c frature/hello`|创建并切换到功能分支|
|`git add hello.txt`|暂存修改|
|`git commit -m ""`|记录一次本地提交|
|`git push -u origin feature/hello`|推送并建立跟踪关系|
|`git switch main`|切换主分支|
|`git pull`|获取并整合当前分支的远程更新|
- 分支为新功能保留独立开发线

## 4.5PR工作流
1. Fork
2. Clone
3. 创建分支
4. 修改/add/commit
5. push->个人Fork
6. 发起PR
7. review后merge到团队仓库







     







