# 第一课笔记：Linux、Vim & Shell、Git & GitHub

## Linux

Linux 是类 Unix 内核，Ubuntu、Debian 这些是发行版。日常操作基本都在终端里完成。

文件系统从 `/` 开始。常用的目录有 `/home`（用户目录）、`/etc`（配置）、`/usr`（程序）、`/var`（日志）、`/tmp`（临时文件）。路径分绝对路径和相对路径，`~` 是主目录，`.` 是当前目录，`..` 是上级目录。

常用命令：

```bash
pwd            # 当前路径
ls -la         # 列目录，含隐藏文件
cd /tmp        # 切换目录
mkdir -p a/b   # 递归建目录
cp -r src dst  # 复制目录
mv a b         # 移动或重命名
rm -rf build   # 删除目录，慎用
cat file       # 查看文件
less file      # 分页查看
grep -rn xxx . # 递归搜索
find . -name "*.md"
```

管道 `|` 把前一个命令的输出交给后一个命令。`>` 覆盖写文件，`>>` 追加，`2>` 重定向错误输出。

权限里的 `rwx` 分别对应 4、2、1。`chmod +x script.sh` 加执行权限，私钥一般设成 `chmod 600`。需要管理员权限时用 `sudo`。

## Vim

Vim 分模式，打开文件时是普通模式，这时按键都是命令。

- `i` 进入插入模式，`Esc` 回到普通模式
- `:w` 保存，`:q` 退出，`:wq` 保存退出，`:q!` 强制退出
- `h j k l` 移动光标，`dd` 删行，`yy` 复制行，`p` 粘贴，`u` 撤销
- `/word` 搜索，`n` 跳到下一个匹配

刚开始记住 `i`、`Esc`、`:wq` 就能用了，剩下的可以慢慢练。

## Shell

脚本第一行写 shebang，指定解释器：

```bash
#!/usr/bin/env bash
```

几个基础点：

- 变量赋值时 `=` 两边不能有空格，比如 `name="wsnb"`，使用时写 `$name` 或 `${name}`。
- 单引号里的内容原样输出，双引号会展开变量。
- `$(command)` 取命令的输出，`$1` 是第一个参数，`$?` 是上一条命令的退出码。
- 条件判断用 `if [[ -f "$file" ]]; then ... fi`，循环用 `for f in *.md; do ... done`。
- 脚本开头可以加 `set -euo pipefail`，让命令出错时直接停下。

另外，非交互执行的脚本里 `history` 是空的，要取最近命令得读 `~/.bash_history`。`*` 匹配任意字符，`&&` 表示前一条成功才执行后一条，`Ctrl+C` 中断，`Ctrl+R` 搜索历史。

## Git 和 GitHub

Git 分三个区域：工作区、暂存区、本地仓库。`git add` 把改动放进暂存区，`git commit` 提交到本地仓库，`git push` 推到远程。

常用命令：

```bash
git status
git add .
git commit -m "Add notes."
git log --oneline
git switch -c new-branch
git switch master
git merge new-branch
git remote -v
git fetch upstream
git pull
git push
```

分支本质上是指向某次提交的指针。合并出现冲突时，文件里会有 `<<<<<<<`、`=======`、`>>>>>>>` 标记，手动改完并删掉标记，再 `git add` 和 `git commit`。

GitHub 的协作流程是：Fork 原仓库，clone 到本地，建自己的分支，修改并提交，push 到自己的 fork，最后向原仓库发 PR。PR 开着的时候继续 push 会自动更新，不要关掉重开。

commit message 按要求用英文、首字母大写、句号结尾，比如 `Add Lesson 1 notes.`。另外只改自己目录里的文件，不要动 `TASK.md` 和别人的文件夹。
