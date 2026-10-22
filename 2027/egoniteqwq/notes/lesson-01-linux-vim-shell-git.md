# 第一课笔记：Linux、Vim & Shell、Git & GitHub

> 课程主题：熟练使用命令行工具，搭建起日常开发所需的基础工作流。
> 本笔记按四个部分整理：**Linux 基础**、**Vim 编辑器**、**Shell 脚本**、**Git & GitHub**。

---

## 1. Linux 基础

### 1.1 文件系统层次结构

Linux 中“一切皆文件”，所有路径都从根目录 `/` 开始。

| 目录 | 用途 |
| --- | --- |
| `/` | 根目录，所有文件的起点 |
| `/home` | 普通用户的主目录，如 `/home/egonite` |
| `/root` | root 用户的主目录 |
| `/etc` | 系统与软件的配置文件 |
| `/usr` | 用户安装的程序与共享库（`/usr/bin`） |
| `/var` | 经常变化的数据，如日志 `/var/log` |
| `/tmp` | 临时文件，重启后可能被清空 |
| `/dev` | 设备文件，如 `/dev/sda`、`/dev/null` |
| `/proc` | 内核与进程信息的虚拟文件系统 |

几个特殊路径：

- `~`：当前用户的主目录。
- `.`：当前目录；`..`：上一级目录。
- 绝对路径以 `/` 开头；相对路径从当前目录算起。

### 1.2 常用命令

```bash
pwd                 # 显示当前目录
ls -lah             # 列出文件（含隐藏文件、人类可读大小）
cd /path/to/dir     # 切换目录；cd - 返回上一次目录
mkdir -p a/b/c      # 递归创建目录
touch file.txt      # 创建空文件 / 更新时间戳
cp -r src dst       # 递归复制
mv old new          # 移动或重命名
rm -rf dir          # 递归强制删除（慎用！）
cat file            # 输出文件内容
less file           # 分页查看，q 退出，/ 搜索
head -n 20 file     # 前 20 行；tail -f 实时跟踪日志
grep -rn "todo" .   # 递归搜索并显示行号
find . -name "*.md" # 按名字查找文件
which python3       # 查看命令所在路径
man ls              # 查看手册；apropos 按关键词搜索
```

**通配符与管道**

- `*` 匹配任意多个字符，`?` 匹配单个字符，`[abc]` 匹配集合中的一个。
- `|` 把前一个命令的标准输出作为后一个命令的标准输入：`ps aux | grep python`。
- `>` 覆盖重定向，`>>` 追加，`2>` 重定向错误输出，`&>` 同时重定向标准输出与错误。

### 1.3 权限与用户

`ls -l` 的第一列形如 `-rwxr-xr--`：

- 第 1 位：文件类型（`-` 普通文件、`d` 目录、`l` 符号链接）。
- 接下来 9 位：属主、属组、其他人的 `r`（读）、`w`（写）、`x`（执行）权限。

```bash
chmod 755 script.sh     # rwxr-xr-x
chmod u+x script.sh     # 给属主增加执行权限
chown user:group file   # 修改属主与属组（通常需要 sudo）
sudo command            # 以 root 身份执行
id                      # 查看当前用户的 UID、GID 与所属组
```

数值表示法：`r=4, w=2, x=1`，三组权限相加即可，例如 `644` 表示 `rw-r--r--`。

### 1.4 进程与作业管理

```bash
ps aux            # 查看所有进程
top / htop        # 动态查看资源占用
kill -9 <PID>     # 强制结束进程
command &         # 放到后台运行
jobs              # 查看当前 shell 的作业
fg %1 / bg %1     # 切回前台 / 放到后台
Ctrl-C            # 中断前台进程
Ctrl-Z            # 暂停前台进程
```

### 1.5 环境变量

```bash
echo $PATH              # 查看可执行文件搜索路径
export MY_VAR="hello"   # 设置并导出为子进程可见的环境变量
env                     # 列出所有环境变量
source ~/.bashrc        # 重新加载配置（当前 shell 生效）
```

`~/.bashrc` 适合放别名、`PATH` 和提示符配置，修改后需要 `source` 或重开终端。

---

## 2. Vim 编辑器

Vim 的设计思想是**模式编辑**：不同模式下按键含义不同。掌握少量命令即可高效编辑。

### 2.1 四种常用模式

| 模式 | 进入方式 | 作用 |
| --- | --- | --- |
| Normal（普通） | `Esc` | 移动、删除、复制等操作 |
| Insert（插入） | `i`、`a`、`o` 等 | 输入文本 |
| Visual（可视） | `v`、`V`、`Ctrl-v` | 选中字符 / 行 / 块 |
| Command-line（命令行） | `:`、`/`、`?` | 保存、退出、替换、搜索 |

### 2.2 打开、保存与退出

```vim
vim file.txt      " 打开文件
:q                " 退出
:q!               " 强制退出，放弃修改
:w                " 保存
:wq  /  :x        " 保存并退出
:w newfile        " 另存为
```

### 2.3 移动光标（Normal 模式）

| 按键 | 作用 |
| --- | --- |
| `h j k l` | 左、下、上、右 |
| `w` / `b` | 下一个 / 上一个单词开头 |
| `0` / `$` | 行首 / 行尾 |
| `gg` / `G` | 文件开头 / 结尾 |
| `:42` | 跳到第 42 行 |
| `Ctrl-d` / `Ctrl-u` | 向下 / 向上翻半页 |
| `f x` / `F x` | 移动到本行下一个 / 上一个字符 x |

### 2.4 编辑操作

```vim
i / a / o         " 光标前插入 / 光标后插入 / 下方新开一行
dd                " 删除（剪切）整行；3dd 删除 3 行
yy                " 复制整行；3yy 复制 3 行
p / P             " 粘贴到光标后 / 前
u / Ctrl-r        " 撤销 / 重做
r x               " 将当前字符替换为 x
cw                " 修改一个单词
x                 " 删除当前字符
>> / <<           " 增加 / 减少缩进
J                 " 合并下一行
```

### 2.5 搜索与替换

```vim
/keyword          " 向下搜索，n / N 跳转
?keyword          " 向上搜索
:%s/old/new/g     " 全文替换
:%s/old/new/gc    " 全文替换并逐个确认
```

### 2.6 多文件与窗口

```vim
:e other.txt      " 编辑另一个文件
:sp / :vsp        " 水平 / 垂直分屏
Ctrl-w w          " 在窗口间切换
:bn / :bp         " 下一个 / 上一个缓冲区
```

### 2.7 配置

`~/.vimrc` 中可以保存个性化设置，例如：

```vim
set number          " 显示行号
set tabstop=4       " Tab 显示为 4 个空格
set shiftwidth=4    " 自动缩进宽度
set expandtab       " 用空格代替 Tab
set hlsearch        " 高亮搜索结果
syntax on           " 语法高亮
```

> 学习建议：先用 `vimtutor` 完成一遍交互式教程，再在日常编辑中逐步替换鼠标操作。

---

## 3. Shell 脚本

### 3.1 脚本的基本结构

```bash
#!/usr/bin/env bash
# 第一行 shebang 指定解释器
set -euo pipefail   # 出错退出、未定义变量报错、管道错误不忽略

echo "Hello, $USER"
```

赋予执行权限并运行：

```bash
chmod +x script.sh
./script.sh
```

### 3.2 变量与引用

```bash
name="world"             # 等号两边不能有空格
echo "hello, $name"      # 双引号中变量会展开
echo 'hello, $name'      # 单引号中一切原样输出
echo "today is $(date)"  # 命令替换
echo "home: ${HOME}"     # 花括号界定变量名边界
```

- 始终给变量加双引号（`"$var"`）可以避免空格与通配符带来的问题。
- 位置参数：`$0` 脚本名，`$1` 第一个参数，`$#` 参数个数，`$@` 所有参数。

### 3.3 条件判断

```bash
if [ -f "$file" ]; then
    echo "regular file"
elif [ -d "$file" ]; then
    echo "directory"
else
    echo "not found"
fi

# 常用测试
[ -e path ]   # 存在
[ -f path ]   # 普通文件
[ -d path ]   # 目录
[ -z "$s" ]   # 字符串为空
[ "$a" -eq "$b" ]  # 数值相等
[ "$a" = "$b" ]    # 字符串相等
```

### 3.4 循环与函数

```bash
for f in *.md; do
    echo "processing $f"
done

count=0
while [ "$count" -lt 3 ]; do
    echo "count=$count"
    count=$((count + 1))
done

greet() {
    local who="$1"
    echo "hello, $who"
}
greet "shell"
```

### 3.5 重定向与管道

```bash
command > out.txt      # 标准输出覆盖写入
command >> out.txt     # 追加
command 2> err.txt     # 错误输出
command &> all.txt     # 标准输出 + 错误输出
command < input.txt    # 从文件读取标准输入
a | b | c              # 管道串联
```

`/dev/null` 是“黑洞”，常用于丢弃输出：`command > /dev/null 2>&1`。

### 3.6 退出状态

- `$?` 保存上一条命令的退出码，`0` 表示成功，非 `0` 表示失败。
- `set -e` 让脚本在命令失败时立即退出。
- `trap 'cleanup' EXIT` 可用于清理临时文件。

### 3.7 实用技巧

```bash
history | tail -n 10   # 查看最近 10 条命令
!!                     # 重复上一条命令
!$                     # 上一条命令的最后一个参数
Ctrl-R                 # 反向搜索历史命令
alias ll='ls -alF'     # 定义别名
```

---

## 4. Git & GitHub

### 4.1 版本控制与核心概念

- **仓库（repository）**：被 Git 管理的项目目录，包含 `.git`。
- **工作区（working tree）**、**暂存区（staging area）**、**本地仓库**、**远程仓库**。
- **提交（commit）**：一次不可变的快照，带有作者、时间和说明。
- **分支（branch）**：指向提交的可移动指针，默认分支通常为 `main` 或 `master`。

### 4.2 常用命令

```bash
git init                     # 初始化仓库
git clone <url>              # 克隆远程仓库
git status                   # 查看工作区状态
git add file / git add .     # 加入暂存区
git commit -m "Add notes."   # 提交（消息大写开头，句号结尾）
git log --oneline --graph    # 查看提交历史
git diff                     # 查看未暂存的修改
git diff --staged            # 查看已暂存的修改
```

### 4.3 分支与合并

```bash
git branch                   # 列出本地分支
git switch -c feature        # 新建并切换分支
git switch master            # 切换分支
git merge feature            # 把 feature 合并进当前分支
git branch -d feature        # 删除已合并分支
git rebase master            # 变基（改写历史，谨慎使用）
```

### 4.4 远程协作

```bash
git remote -v                # 查看远程地址
git remote add upstream <url>  # 添加上游仓库
git fetch upstream           # 只拉取，不自动合并
git pull --rebase upstream master  # 拉取并变基
git push origin branch       # 推送到自己的远程分支
```

典型 **fork 工作流**：

1. 在 GitHub 上 fork 目标仓库到自己的账号。
2. `git clone` 自己的 fork，并添加上游仓库 `upstream`。
3. 从最新 `upstream/master` 创建开发分支。
4. 修改、`git add`、`git commit`、`git push origin <branch>`。
5. 在 GitHub 上向目标仓库发起 **Pull Request**。
6. 根据 review 意见继续提交，新的提交会自动进入 PR；**不要关闭 PR 重开**。

### 4.5 解决冲突

当两个分支修改了同一文件的同一区域时，合并会产生冲突，文件中会出现：

```text
<<<<<<< HEAD
当前分支的内容
=======
要合并进来的内容
>>>>>>> feature
```

处理步骤：

1. 打开冲突文件，保留正确内容，删除 `<<<<<<<`、`=======`、`>>>>>>>` 标记。
2. `git add <file>` 标记冲突已解决。
3. `git commit`（merge）或 `git rebase --continue`（rebase）。

### 4.6 撤销与回退

| 场景 | 命令 |
| --- | --- |
| 撤销工作区修改 | `git restore <file>` |
| 取消暂存 | `git restore --staged <file>` |
| 修改最近一次提交 | `git commit --amend` |
| 回退提交但保留修改 | `git reset --soft HEAD~1` |
| 回退提交并丢弃修改 | `git reset --hard HEAD~1` |
| 生成反向提交（安全） | `git revert <commit>` |

> `reset --hard` 会丢失未提交的修改；已经推送到远程的历史，优先使用 `revert`。

### 4.7 `.gitignore` 与提交规范

```gitignore
# 忽略编译产物与本地配置
build/
*.o
.env
.DS_Store
```

良好提交的习惯：

- 一次提交只做一件事，提交信息使用祈使句、英文、首字母大写、句号结尾。
- 提交前用 `git status` 和 `git diff` 自查，避免把无关文件或密钥提交上去。
- 推送前先同步上游，减少冲突。

### 4.8 GitHub 协作功能

- **Issue**：记录 bug、需求与讨论。
- **Pull Request**：代码评审与合并请求，可关联 Issue（`Closes #12`）。
- **Actions**：CI/CD 自动化，在 push / PR 时运行测试。
- **Pages**：直接托管静态站点。
- **SSH / Token**：GitHub 已不支持密码认证，推荐配置 SSH key 或 Personal Access Token。

---

## 5. 小结

| 主题 | 最需要记住的内容 |
| --- | --- |
| Linux | 目录结构、权限、管道与重定向、进程管理 |
| Vim | 模式切换、移动与文本对象、搜索替换、`.vimrc` |
| Shell | 变量引用、条件与循环、`set -euo pipefail`、退出码 |
| Git | 工作区/暂存区/提交、分支合并、fork + PR 工作流、冲突处理 |

命令行的价值在于**可组合**：小工具通过管道组合成强大的工作流。熟练之后，应进一步学习 `make`、CMake、catkin 等构建工具，把重复劳动交给脚本与自动化。
