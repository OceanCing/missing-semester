# 培训课第一课笔记：Linux、Vim && Shell、Git && GitHub

> 课程仓库：<https://github.com/OceanCing/missing-semester>
> 整理人：`77430`　｜　更新日期：2026-10-21

## 目录

1. [Linux 基础](#1-linux-基础)
2. [Shell 脚本](#2-shell-脚本)
3. [Vim 编辑器](#3-vim-编辑器)
4. [Git 与 GitHub](#4-git-与-github)
5. [本节课的作业工作流](#5-本节课的作业工作流)
6. [参考资料](#6-参考资料)

---

## 1. Linux 基础

### 1.1 “一切皆文件”

Linux 把设备、进程、网络连接都抽象成文件，因此**绝大多数操作都可以归结为对文件的操作**。
根目录是 `/`，常见目录的用途如下：

| 目录 | 作用 |
| :--- | :--- |
| `/bin`、`/usr/bin` | 系统命令（可执行文件） |
| `/etc` | 全局配置文件 |
| `/home` | 普通用户的家目录 |
| `/root` | root 用户的家目录 |
| `/tmp` | 临时文件，重启后可能被清空 |
| `/var` | 经常变化的数据，如日志 `/var/log` |
| `/dev` | 设备文件，如 `/dev/null` |
| `/proc` | 内核和进程信息（虚拟文件系统） |

### 1.2 常用命令

| 类别 | 命令 | 说明 |
| :--- | :--- | :--- |
| 浏览 | `pwd` | 显示当前所在路径 |
| 浏览 | `ls -alh` | 列出全部文件（含隐藏文件）、大小易读 |
| 浏览 | `cd -` | 回到上一次所在的目录 |
| 文件 | `cp -r src dst` | 递归复制目录 |
| 文件 | `mv a b` | 移动或重命名 |
| 文件 | `rm -rf dir` | **危险**：递归强制删除，不可恢复 |
| 查看 | `cat` / `less` | 一次输出 / 分页浏览 |
| 查看 | `head -n 20` / `tail -f` | 看前 20 行 / 实时跟踪文件末尾 |
| 查找 | `grep -rn "pattern" dir` | 递归搜索内容并显示行号 |
| 查找 | `find . -name "*.sh"` | 按名字、类型、时间查找文件 |
| 权限 | `chmod 755 file` | 修改权限 |
| 权限 | `chown user:group file` | 修改属主 |
| 进程 | `ps aux` / `top` / `kill -9 PID` | 查看、监控、结束进程 |
| 磁盘 | `df -h` / `du -sh dir` | 查看磁盘 / 目录占用 |
| 其他 | `man ls` / `tldr ls` | 查看手册 / 查看常用示例 |

### 1.3 权限与 `chmod`

`ls -l` 的输出中，前 10 位形如 `-rwxr-xr--`：第一位是文件类型（`-` 普通文件、`d` 目录、`l` 软链接），
后 9 位依次是 **属主 / 属组 / 其他人** 的 `rwx` 权限，对应数字 `4 2 1`。

```bash
chmod 755 script.sh   # 属主 rwx，其他人 r-x
chmod +x script.sh    # 直接给所有用户加执行权限
```

### 1.4 重定向与管道

```bash
echo "hello" > out.txt      # 覆盖写入（标准输出）
echo "world" >> out.txt     # 追加写入
ls not-exist 2> err.txt     # 重定向标准错误
ls -l | grep "\.sh$"        # 管道：前一个命令的输出作为后一个命令的输入
command 2>&1 | tee log.txt  # 合并标准错误与标准输出，同时打印并写文件
```

> 记住三个默认文件描述符：`0` 标准输入、`1` 标准输出、`2` 标准错误。

### 1.5 通配符

| 通配符 | 含义 | 例子 |
| :--- | :--- | :--- |
| `*` | 任意多个字符 | `*.md` |
| `?` | 任意一个字符 | `file?.txt` |
| `[abc]` | 方括号中任意一个字符 | `[abc].sh` |

---

## 2. Shell 脚本

### 2.1 基本结构

```bash
#!/usr/bin/env bash
# 第一行是 shebang，指定用哪个解释器执行本脚本

set -euo pipefail   # 出错即退出 / 未定义变量报错 / 管道中任一命令失败即失败

echo "Hello, ${USER}"
```

执行脚本的两种方式：

```bash
bash script.sh        # 用新的 bash 进程执行，不需要执行权限
./script.sh           # 依赖 shebang 和执行权限，需要先 chmod +x
```

### 2.2 变量与参数

```bash
name="Ocean"                 # 等号两边不能有空格
echo "hello, $name"          # 引用变量
echo "params: $# -> $@"      # $# 参数个数，$@ 全部参数
echo "first: $1"             # 第一个参数
echo "exit code: $?"         # 上一条命令的退出码，0 表示成功
```

### 2.3 条件与循环

```bash
if [[ -f "$file" && -r "$file" ]]; then
  echo "文件存在且可读"
elif [[ -d "$file" ]]; then
  echo "是个目录"
else
  echo "不存在"
fi

for f in *.md; do
  echo "处理 $f"
done

while read -r line; do
  echo "$line"
done < input.txt
```

### 2.4 函数与目录定位

```bash
main() {
  local dir
  dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  echo "脚本所在目录：$dir"
}

main "$@"    # 把脚本参数原样传给 main
```

> 用 `"$(cd "$(dirname ...)" && pwd)"` 取脚本自身目录，比 `$0` 直接拼接更可靠，
> 因为无论从哪个目录调用脚本，结果都正确。

---

## 3. Vim 编辑器

### 3.1 四种模式

| 模式 | 进入方式 | 作用 |
| :--- | :--- | :--- |
| Normal（普通） | `Esc` | 移动、删除、复制等，默认模式 |
| Insert（插入） | `i` `a` `o` `O` | 输入文字 |
| Visual（可视） | `v` `V` `Ctrl+v` | 选中区域后操作 |
| Command（命令行） | `:` `/` `?` | 保存、退出、替换、搜索 |

### 3.2 常用操作

| 分类 | 按键 | 说明 |
| :--- | :--- | :--- |
| 移动 | `h j k l` | 左下上右 |
| 移动 | `w` / `b` / `e` | 下一个词首 / 上一个词首 / 词尾 |
| 移动 | `0` / `^` / `$` | 行首 / 首个非空字符 / 行尾 |
| 移动 | `gg` / `G` / `:42` | 文件首 / 文件尾 / 第 42 行 |
| 编辑 | `x` / `dd` / `yy` / `p` | 删字符 / 删行 / 复制行 / 粘贴 |
| 编辑 | `u` / `Ctrl+r` | 撤销 / 重做 |
| 编辑 | `ciw` / `cw` | 修改光标所在单词 |
| 搜索 | `/word` `n` `N` | 向下搜索、下一个、上一个 |
| 替换 | `:%s/old/new/g` | 全文替换 |
| 替换 | `:%s/old/new/gc` | 全文替换，逐个确认 |
| 保存 | `:w` `:q` `:wq` `:q!` `ZZ` | 保存 / 退出 / 保存退出 / 强制退出 / 保存退出 |

### 3.3 一个顺手的 `.vimrc`

```vim
set number            " 显示行号
set relativenumber    " 显示相对行号，方便跳转
set tabstop=4         " Tab 显示为 4 个空格
set shiftwidth=4      " 自动缩进 4 个空格
set expandtab         " Tab 转为空格
set ignorecase        " 搜索忽略大小写
set smartcase         " 但包含大写时区分大小写
syntax on             " 语法高亮
```

> 练习方式：终端里直接运行 `vimtutor`，跟着做完一遍基本就够用了。

---

## 4. Git 与 GitHub

### 4.1 四个区域

```text
工作区 (Working Directory)  --git add-->  暂存区 (Staging Area)
       --git commit-->  本地仓库 (Local Repository)  --git push-->  远程仓库 (Remote)
```

### 4.2 常用命令

| 场景 | 命令 |
| :--- | :--- |
| 初始化 / 克隆 | `git init` / `git clone <url>` |
| 查看状态 | `git status` / `git diff` |
| 暂存并提交 | `git add <file>` → `git commit -m "Message."` |
| 查看历史 | `git log --oneline --graph --all` |
| 分支 | `git branch` / `git switch -c <name>` / `git switch <name>` |
| 合并 | `git merge <branch>` |
| 远程 | `git remote -v` / `git push -u origin <branch>` / `git pull --rebase` |
| 撤销 | `git restore <file>`（撤销工作区修改） / `git restore --staged <file>`（取消暂存） |
| 回退 | `git revert <commit>`（安全，生成反向提交） |

### 4.3 分支与冲突

1. 改代码前先建分支，**不要把实验性改动直接提交到 `master`**；
2. `git pull --rebase origin master` 把上游最新提交垫到本地提交之前，历史更干净；
3. 出现冲突时，编辑文件中的 `<<<<<<<` / `=======` / `>>>>>>>` 标记，保留正确内容后：

```bash
git add <冲突文件>
git rebase --continue    # 或 git merge --continue
```

> 冲突的正确处理办法是**解决冲突并提交**，而不是关掉 PR 重新开一个来绕过。

### 4.4 Commit 规范

本课程要求：**英文、首字母大写、句号结尾、祈使句**。

```text
Add notes and homework script for the first lesson.   ✅
update                                  ❌ 太小写、太笼统
Added some files                         ❌ 过去式且无句号
```

### 4.5 Pull Request 流程

```text
fork / 建分支 → 本地提交 → push 到自己的远程 → 在目标仓库发起 PR → 等待 review → 按评论修改并继续 push
```

PR 标题用英文、首字母大写；描述可以用中文，但要写清楚**改了什么、为什么改、怎么验证**。

---

## 5. 本节课的作业工作流

```bash
# 1. 克隆自己的 fork，并建一个开发分支
git clone git@github.com:<你的用户名>/missing-semester.git
cd missing-semester
git switch -c homework-01

# 2. 在 2027/ 下建自己的目录
mkdir -p 2027/<你的用户名>/notes 2027/<你的用户名>/homework

# 3. 写笔记和脚本（见本目录下的文件）

# 4. 提交并推送
git add 2027/<你的用户名>
git commit -m "Add notes and homework script for the first lesson."
git push -u origin homework-01

# 5. 在 GitHub 上向 OceanCing/missing-semester 发起 PR
```

几个容易踩的坑：

- [ ] 目录名必须是**自己的 GitHub 用户名**，放在 `2027/` 下面；
- [ ] `.md` 文件名必须是英文，例如 `linux-vim-shell-git.md`；
- [ ] `.sh` 脚本要 `chmod +x`，输出重定向到**同目录**的 `output.txt`；
- [ ] 后续 `TASK.md` 更新会让 PR 产生冲突，要 pull 后手动解决，**不要 Close PR**；
- [ ] 开了 PR 之后仍然可以改标题、描述、分支，新提交会自动进入 PR。

---

## 6. 参考资料

- [MIT Missing Semester](https://missing.csail.mit.edu/)：本课程的主要参考
- [Pro Git（中文版）](https://git-scm.com/book/zh/v2)：Git 最权威的免费书
- [Learn Git Branching](https://learngitbranching.js.org/?locale=zh_CN)：可视化练习分支操作
- [Vim Adventures](https://vim-adventures.com/)：用游戏练 Vim 按键
- [tldr pages](https://tldr.sh/)：命令的常用示例速查
- 终端里的 `man <command>`、`vimtutor`、`git help <command>`
