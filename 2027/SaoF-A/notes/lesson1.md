# DynamicX lesson1 学习笔记--杨海--3125002584

## 一、课程目标

1. 学习 Linux 基础操作。
2. **学习终端和 Shell 的常用用法。**
3. 使用 Vim 编辑文件。
4. 使用 Git 管理代码和文件版本。
5. 通过 GitHub Fork、分支和 Pull Request 提交作业。

## 二、Linux部分

| 路径| 作用 |
|---|---|
| `/home/user` | 用户文件 |
| `/etc` | 系统配置 |
| `/usr` | 程序和库 |
| `/var` | 日志等可变数据 |
| `/dev` | 设备文件 |

Linux 路径区分大小写。
- /home/user/example：绝对路径，从 / 开始。
- ./hello.txt：相对路径，从当前目录开始。
- ~/example：当前用户主目录下的 example。
- .：当前目录。
- ..：上一级目录。

| 常用命令| 作用 |
|---|---|
| `pwd` | 查看当前目录 |
| `ls` | 查看目录内容 |
| `cd directory` | 进入目录 |
| `mkdir directory` | 创建目录 |
| `touch file.txt` | 创建文件 |
| `cp a.txt b.txt` | 复制文件 |
| `mv a.txt b.txt` | 移动或重命名文件 |
| `rm file.txt` | 删除文件 |
| `cat file.txt` | 查看文件内容 |


- `/home/user/example`：绝对路径，从 `/` 开始。
- `./hello.txt`：相对路径，从当前目录开始。
- `~/example`：当前用户主目录下的 `example`。
- `.`：当前目录。
- `..`：上一级目录。

---

## 三、常用终端命令

### 查看当前位置和文件

| 命令 | 作用 | 示例 |
|---|---|---|
| `pwd` | 显示当前目录 | `pwd` |
| `ls` | 查看目录内容 | `ls` |
| `ls -la` | 查看详细信息和隐藏文件 | `ls -la` |
| `tree` | 以树状结构查看目录 | `tree` |
| `clear` | 清空终端屏幕 | `clear` |

### 文件和目录操作

| 命令 | 作用 | 示例 |
|---|---|---|
| `cd` | 切换目录 | `cd notes` |
| `cd ..` | 返回上一级目录 | `cd ..` |
| `mkdir` | 创建目录 | `mkdir demo` |
| `mkdir -p` | 连同上级目录一起创建 | `mkdir -p a/b/c` |
| `touch` | 创建空文件 | `touch test.txt` |
| `cp` | 复制文件 | `cp a.txt b.txt` |
| `mv` | 移动或重命名文件 | `mv old.txt new.txt` |
| `rm` | 删除文件 | `rm test.txt` |

### 查看和处理文本

| 命令 | 作用 | 示例 |
|---|---|---|
| `cat` | 查看文件内容 | `cat notes.md` |
| `less` | 分页查看文件 | `less notes.md` |
| `head` | 查看文件开头 | `head -n 5 notes.md` |
| `tail` | 查看文件结尾 | `tail -n 5 notes.md` |
| `grep` | 搜索指定文字 | `grep "Linux" notes.md` |
| `wc` | 统计行数、单词数和字符数 | `wc notes.md` |
| `sort` | 对文本排序 | `sort names.txt` |
| `uniq` | 删除相邻重复行 | `sort names.txt \| uniq` |

### 陌生命令帮助

```bash
ls --help
man ls

- `--help`：查看命令的简要帮助。
- `man`：查看命令手册。

在 `man` 页面中按 `q` 退出。

终端快捷操作：

- `Tab`：自动补全命令或路径。
- `↑`、`↓`：调出历史命令。
```

### 输出、变量和管道

变量可以保存一个值：

```bash
name="Linux"
echo "$name"
```

输出文本：

```bash
echo "Hello Linux"
printf "Name: %s\n" "Linux"
```

文件重定向：

```bash
echo "first line" > output.txt
echo "second line" >> output.txt
```

- `>`：覆盖文件内容。
- `>>`：追加到文件末尾。
- `|`：把前一个命令的输出传给下一个命令。

示例：

```bash
ls -la | grep ".md"
```

删除文件前要确认路径：

```bash
rm file.txt
```

## 四、Vim 编辑器

Vim 是一个运行在终端中的文本编辑器，常用于 SSH 环境和没有图形界面的 Linux 环境。

打开文件：

```bash
vim hello.txt
```

### Vim的常用模式

- 普通模式：移动、删除、复制。
- 插入模式：输入文字。
- 命令行模式：保存和退出。

### 常用按键

| 按键或命令 | 作用 |
|---|---|
| `i` | 进入插入模式 |
| `Esc` | 返回普通模式 |
| `h`、`j`、`k`、`l` | 左、下、上、右移动 |
| `dd` | 删除当前行 |
| `yy` | 复制当前行 |
| `p` | 粘贴 |
| `u` | 撤销 |
| `/word` | 搜索 `word` |
| `:w` | 保存 |
| `:q` | 退出 |
| `:wq` | 保存并退出 |
| `:q!` | 放弃修改并退出 |

## 五、Git 与 GitHub 的区别

### Git

Git 是本地版本控制工具，可以：

- 记录每次提交。
- 查看文件修改和查看提交历史。
- 创建开发分支。

### GitHub

GitHub 是远程代码托管和协作平台，可以：

- 保存远程 Git 仓库。
- 通过 Pull Request 讨论和评审。
- 合并团队认可的修改。

```text
Git：本地版本控制
GitHub：远程托管与协作
```

---

## 六、Git 的四个位置

一次修改通常经过以下位置：

```text
工作区 → 暂存区 → 本地仓库 → GitHub 远程仓库
```

对应命令：

```bash
git add
git commit
git push
```

- 工作区：正在编辑的文件。
- 暂存区：准备本次提交的文件。
- 本地仓库：已经被 Git 记录的提交。
- 远程仓库：GitHub 上保存的提交。

`git commit` 只保存到本地仓库，`git push` 才会发送到 GitHub。

---

## 七、Git 常用命令

### 获取和查看

```bash
git clone <仓库地址>
git status
git diff
git log --oneline
```

| 命令 | 作用 |
|---|---|
| `git clone` | 下载仓库 |
| `git status` | 查看分支和文件状态 |
| `git diff` | 查看未暂存的修改 |
| `git log --oneline` | 查看提交历史 |

### 分支和提交

创建并切换到新分支：

```bash
git switch -c feature/lesson-one
```

暂存文件：

```bash
git add lesson-one.md
```

创建提交：

```bash
git commit -m "Add first lesson notes."
```

推送分支：

```bash
git push -u origin feature/lesson-one
```

切换回主分支：

```bash
git switch main
```

获取远程更新：

```bash
git pull
```

---

## 八、GitHub PR 工作流

团队协作时，通常按照下面的流程：

1. Fork 团队仓库。
2. Clone 自己的 Fork。
3. 创建功能分支。
4. 修改文件。
5. 使用 `git add` 暂存。
6. 使用 `git commit` 创建本地提交。
7. 使用 `git push` 推送到自己的 Fork。
8. 在 GitHub 上创建 Pull Request。
9. 接受评审并继续修改。
10. 评审通过后合并到团队仓库。

Fork 是仓库副本，Branch 是仓库中的开发分支。

---

## 九、SSH 密钥

SSH 密钥由公钥和私钥组成：

- 公钥文件通常以 `.pub` 结尾，可以添加到 GitHub。
- 私钥保存在自己的电脑上，不能上传、截图或分享。

生成 Ed25519 密钥：

```bash
ssh-keygen -t ed25519 -C "你的邮箱"
```

查看公钥：

```bash
cat ~/.ssh/id_ed25519.pub
```

测试 GitHub SSH 连接：

```bash
ssh -T git@github.com
```

GitHub 设置路径：

```text
Settings → SSH and GPG keys → New SSH key
```

---

## 十一、个人学习总结

通过本节课，我学习了 Linux 的目录结构、路径表示方式、常用终端命令、Shell 的变量和重定向、Vim 的基本操作，以及 Git 和 GitHub 的协作流程。

我还理解了工作区、暂存区、本地仓库和远程仓库之间的关系，并学会了通过分支和 Pull Request 提交自己的修改。

