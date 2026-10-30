# 第一课笔记：Linux、Vim 与 Shell、Git 与 GitHub

## 1. Linux 基础

### 文件系统与路径

Linux 的目录从根目录 `/` 开始。绝对路径从 `/` 开始，相对路径以当前目录为起点。`~` 表示当前用户的家目录，`.` 表示当前目录，`..` 表示上一级目录。文件名区分大小写，以 `.` 开头的文件通常默认隐藏。

| 命令 | 用途 | 示例 |
| --- | --- | --- |
| `pwd` | 查看当前目录 | `pwd` |
| `ls` | 列出文件 | `ls -lah` |
| `cd` | 切换目录 | `cd ..` |
| `mkdir` | 创建目录 | `mkdir -p notes homework` |
| `cp` | 复制文件 | `cp lesson-one.md lesson-one.backup.md` |
| `mv` | 移动或重命名文件 | `mv old.md new.md` |
| `cat` / `less` | 查看文本 | `less notes/lesson-one.md` |
| `man` | 查看命令手册 | `man ls` |

路径中有空格时，要使用引号，例如 `cd "my notes"`。删除文件前先确认路径，`rm` 通常不会把文件放进图形界面的回收站。

### 权限

`ls -l` 中的 `r`、`w`、`x` 分别表示读、写、执行权限，依次对应文件所有者、所属组、其他用户。目录的 `x` 权限表示允许进入、访问目录中的条目。

```bash
chmod +x homework/lesson-one.sh
./homework/lesson-one.sh
```

直接执行脚本需要执行权限；`bash homework/lesson-one.sh` 则由 Bash 读取脚本。`sudo` 用于以其他身份执行命令，不能把它当作解决所有报错的方法。

## 2. Vim

Vim 通过不同模式完成移动和编辑。打开文件后先处于普通模式，按 `i` 进入插入模式，按 `Esc` 回到普通模式。在普通模式按 `:` 可以输入保存、退出等命令。

| 操作 | 按键或命令 |
| --- | --- |
| 左、下、上、右移动 | `h`、`j`、`k`、`l` |
| 移动到下一个单词开头 | `w` |
| 移动到行首、行尾 | `0`、`$` |
| 移动到文件开头、末尾 | `gg`、`G` |
| 删除一行、复制一行、粘贴 | `dd`、`yy`、`p` |
| 撤销、重做 | `u`、`Ctrl-r` |
| 进入可视模式选择文本 | `v` |
| 向后搜索文本 | `/text`，按回车后用 `n` 找下一个 |
| 保存 | `:w` |
| 保存并退出 | `:wq` |
| 放弃未保存修改并退出 | `:q!` |

基本编辑过程：`vim notes/lesson-one.md` → `i` → 输入内容 → `Esc` → `:wq`。不确定当前模式时先按 `Esc`。可以使用 `vimtutor` 练习。

## 3. Shell 与 Bash 脚本

终端提供输入和显示界面，Shell 解释命令；Bash 是一种 Shell。脚本首行 `#!/usr/bin/env bash` 表示直接运行文件时使用 Bash。

### 变量、引用与退出状态

```bash
name="Wit-1"                  # 等号两侧不能有空格
printf 'Hello, %s\n' "$name"  # 双引号允许变量展开
printf '%s\n' '$name'         # 单引号保留字面内容
today=$(date +%F)             # 命令替换，保存命令的标准输出
printf '%s\n' "$today"
```

使用 `"$variable"` 可以避免变量中的空格被拆成多个参数。`$?` 表示上一条命令的退出状态，`0` 通常表示成功，非零表示失败。`command1 && command2` 只在前一条成功时执行后一条；`command1 || command2` 只在前一条失败时执行后一条。

### 标准流、重定向与管道

程序通常使用标准输入（0）、标准输出（1）、标准错误（2）。

| 写法 | 含义 |
| --- | --- |
| `command > file` | 将标准输出写入文件，覆盖原内容 |
| `command >> file` | 将标准输出追加到文件末尾 |
| `command < file` | 从文件读取标准输入 |
| `command 2> error.log` | 将标准错误写入文件 |
| `command1 \| command2` | 将前一条命令的标准输出交给后一条 |

把一组命令的输出写入同一个文件，可以使用大括号和重定向：

```bash
{
    date '+%Y-%m-%d %H:%M:%S %Z %z'
    printf 'Hello, Shell!\n'
} > output.txt
```

`>` 会覆盖文件，所以运行前需要确认目标路径。管道默认不传递标准错误，必要时要单独处理。

### 历史记录与字数统计

`history 10` 显示 Bash 历史列表中的最后 10 条记录。普通非交互脚本不会自动继承当前终端内存中的历史，因此脚本先用 `history -r` 读取历史文件；运行前在原来的 Bash 终端执行 `history -a`，把本次会话新增的记录追加到历史文件。

`history -c` 在独立脚本进程中只清空该进程的内存历史列表。本作业脚本不写入历史文件，也不清除原终端的历史。历史不足 10 条时，只输出实际存在的记录。

`wc -w` 按空白分隔统计词数，不能准确统计连续中文的词数；`wc -m` 在 UTF-8 locale 下统计字符数，包含空格、标点和换行。作业同时输出这两个结果，明确统计口径。

## 4. Git 与 GitHub

Git 在本地管理版本，GitHub 托管远程仓库并提供 Pull Request（PR）等协作功能。`commit` 保存本地提交，`push` 把提交推到远程，两者不是同一步操作。

### 工作区、暂存区与提交

1. 在工作区修改文件。
2. 使用 `git diff` 检查修改，再用 `git add` 选择本次要提交的文件。
3. 使用 `git diff --cached` 检查暂存内容。
4. 使用 `git commit` 创建提交，用 `git log --oneline` 查看历史。

`git status` 可以随时检查当前分支和文件状态。只暂存本次作业相关文件，避免把临时文件带入提交。

### 本次作业的协作流程

先在 GitHub 上 Fork `OceanCing/missing-semester` 到个人账号。`origin` 指自己的 Fork，`upstream` 指本次作业的目标仓库。

```bash
git clone https://github.com/Wit-1/missing-semester.git
cd missing-semester
git remote add upstream https://github.com/OceanCing/missing-semester.git
git switch -c homework/lesson-1
mkdir -p 2027/Wit-1/notes 2027/Wit-1/homework

# 完成笔记和脚本、运行检查后，再提交。
git status
git diff
git add 2027/Wit-1/notes/lesson-one.md \
    2027/Wit-1/homework/lesson-one.sh \
    2027/Wit-1/homework/output.txt
git diff --cached
git commit -m "Add lesson one notes and homework."
git push -u origin homework/lesson-1
```

创建 PR 时确认目标是 **`OceanCing/missing-semester:master`**，来源是 **`Wit-1/missing-semester:homework/lesson-1`**。英文标题以大写字母开头，描述写清楚新增的笔记、脚本和验证结果。commit 消息以英文大写字母开头，以句号结尾。

### 同步上游与处理冲突

```bash
git fetch upstream
git switch homework/lesson-1
git merge upstream/master
```

合并前先用 `git status` 确认工作区干净；有未完成修改时先妥善保存。出现冲突后，打开冲突文件，理解双方修改，编辑出最终内容并移除冲突标记，再执行 `git add`、`git commit`，重新验证后 `git push`。

已有 PR 会随着同一来源分支的新提交自动更新。评审提出修改或出现冲突时，继续在这个分支上处理，保留原 PR，不通过关闭再重开的方式规避冲突。

## 5. 本次脚本的运行方法

在**交互式 Bash 终端**中，从仓库根目录运行：

```bash
history -a
HISTFILE="${HISTFILE:-$HOME/.bash_history}" ./2027/Wit-1/homework/lesson-one.sh
cat 2027/Wit-1/homework/output.txt
```

也可以使用 `bash 2027/Wit-1/homework/lesson-one.sh` 运行。脚本通过自身所在目录定位笔记和 `output.txt`，从其他工作目录调用也能写到正确位置。脚本会检查笔记和历史文件是否可读，检查失败时会在标准错误中提示，并返回非零状态。

`output.txt` 记录运行时的系统时间、指定 Bash 历史文件中最近 10 条记录、笔记字符数与空白分隔词数。每次运行会覆盖上次结果。提交前检查这些历史命令适合公开。

## 参考资料

- [第一课作业要求](https://github.com/OceanCing/missing-semester/blob/master/2027/TASK.md)
- [Missing Semester：课程概览与 Shell](https://missing.csail.mit.edu/2020/course-shell/)
- [Vim 用户手册：开始编辑](https://vimhelp.org/usr_02.txt.html)
- [Pro Git：向一个项目贡献](https://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project)
- 本机手册：`help history`、`man bash`、`man wc`。
