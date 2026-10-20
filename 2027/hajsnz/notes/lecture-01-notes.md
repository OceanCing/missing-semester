# 培训课第一课笔记：Linux、Vim && Shell、Git && GitHub

> 课程：2027 届培训课 · 第一课
> 内容涵盖：Linux 基础操作、Vim 编辑器、Shell 与脚本、Git 与 GitHub 协作

---

## 目录

1. [Linux 基础](#一linux-基础)
2. [Vim 编辑器](#二vim-编辑器)
3. [Shell 与脚本](#三shell-与脚本)
4. [Git 与 GitHub](#四git-与-github)
5. [易错点与小结](#五易错点与小结)

---

## 一、Linux 基础

### 1.1 核心思想

Linux 有两条贯穿始终的哲学：

- **一切皆文件**：设备、进程信息、配置都以文件形式暴露在文件系统里（如 `/dev/sda`、`/proc/cpuinfo`）。
- **小工具组合**：每个命令只做好一件事，通过**管道**把多个命令拼起来完成复杂任务。

### 1.2 目录结构

| 路径 | 作用 |
| --- | --- |
| `/` | 根目录，所有路径的起点 |
| `/bin`、`/usr/bin` | 常用可执行文件（`ls`、`cp`、`grep` 等） |
| `/etc` | 系统与软件的配置文件 |
| `/home/<user>` | 普通用户的家目录 |
| `/root` | root 用户的家目录 |
| `/var` | 经常变化的数据，如日志 `/var/log` |
| `/tmp` | 临时文件，重启可能被清空 |
| `/dev` | 设备文件 |
| `/proc`、`/sys` | 内核与进程的虚拟文件系统 |
| `/opt` | 第三方软件安装位置 |
| `/mnt`、`/media` | 挂载点 |

### 1.3 常用命令速查

**文件与目录**

```bash
pwd                  # 显示当前路径
ls -la               # 列出全部文件（含隐藏文件）及详细信息
cd /path/to/dir      # 切换目录；cd - 返回上一个目录；cd ~ 回家目录
mkdir -p a/b/c       # 递归创建多级目录
cp -r src dst        # 递归复制
mv old new           # 移动 / 重命名
rm -rf dir           # 递归强制删除（慎用！）
touch file.txt       # 创建空文件 / 更新时间戳
ln -s target link    # 创建软链接
```

**查看与搜索**

```bash
cat file             # 输出整个文件
less file            # 分页阅读，q 退出，/ 搜索
head -n 20 file      # 前 20 行
tail -f app.log      # 实时跟踪日志
grep -rn "keyword" . # 递归搜索并显示行号
find . -name "*.md"  # 按名称查找文件
wc -l file           # 统计行数（-w 词数，-c 字节数，-m 字符数）
man ls               # 查看手册
```

**进程与系统**

```bash
ps aux               # 查看所有进程
top / htop           # 实时监控
kill -9 <pid>        # 强制结束进程
df -h                # 磁盘使用率
du -sh dir           # 目录占用大小
free -h              # 内存使用
uname -a             # 系统信息
```

### 1.4 权限

`ls -l` 输出如 `-rwxr-xr--`，共 10 位：第 1 位是类型（`-` 普通文件、`d` 目录、`l` 链接），后 9 位每 3 位一组，分别对应**属主 u / 属组 g / 其他 o** 的 `r`(4) `w`(2) `x`(1) 权限。

```bash
chmod 755 script.sh      # rwxr-xr-x：属主全权限，其他人可读可执行
chmod +x script.sh       # 更常用的写法：加执行权限
chown user:group file    # 修改属主与属组
```

### 1.5 管道与重定向

```bash
ls | grep ".md"              # 管道：前一个命令的 stdout 作为后一个的 stdin
echo "hi" > out.txt          # 覆盖写入
echo "hi" >> out.txt         # 追加写入
cmd 2> err.log               # 只重定向标准错误
cmd > out.txt 2>&1           # stdout 与 stderr 一起重定向
cmd &> /dev/null             # 丢弃全部输出（bash 专有简写）
```

### 1.6 环境变量与包管理

```bash
echo $PATH                   # 查看可执行文件搜索路径
export MY_VAR="value"        # 设置并导出给子进程
source ~/.bashrc             # 重新加载配置

# Debian / Ubuntu 系
sudo apt update && sudo apt install -y vim git
# RedHat / CentOS 系
sudo dnf install -y vim git
```

---

## 二、Vim 编辑器

### 2.1 为什么用 Vim

- 纯键盘操作，手不离主键区，编辑效率高；
- 服务器上几乎必有，SSH 登录后无需安装；
- 可高度定制（`.vimrc`），支持插件生态。

### 2.2 模式

| 模式 | 进入方式 | 用途 |
| --- | --- | --- |
| Normal（普通） | `Esc` | 移动、删除、复制等操作，**默认模式** |
| Insert（插入） | `i` `a` `o` `I` `A` `O` | 输入文本 |
| Visual（可视） | `v` `V` `Ctrl-v` | 选中文本（字符 / 行 / 块） |
| Command-line（命令行） | `:` `/` `?` | 保存、退出、查找替换 |

> 新手最容易卡住的地方：**先按 `Esc` 回到普通模式**再做事。

### 2.3 保存与退出

```vim
:w        " 保存
:q        " 退出
:wq       " 保存并退出
:q!       " 强制退出不保存
:x        " 保存并退出（有改动才写盘）
ZZ        " 普通模式下保存并退出
```

### 2.4 移动

```vim
h j k l         " 左 下 上 右
w / b / e       " 下一个词首 / 上一个词首 / 词尾
0 / ^ / $       " 行首 / 第一个非空字符 / 行尾
gg / G          " 文件首行 / 末行
:n              " 跳到第 n 行
Ctrl-d / Ctrl-u " 向下 / 向上翻半屏
Ctrl-f / Ctrl-b " 向下 / 向上翻整屏
{ / }           " 按段落跳转
%               " 跳到匹配的括号
```

### 2.5 编辑

```vim
x           " 删除光标处字符
dd          " 删除整行（3dd 删 3 行）
yy          " 复制整行（3yy 复制 3 行）
p / P       " 粘贴到光标后 / 前
u / Ctrl-r  " 撤销 / 重做
r           " 替换单个字符
cc          " 删除整行并进入插入模式
cw          " 修改一个词
ci"         " 修改引号内的内容（ci( 、ci{ 同理）
~           " 大小写切换
.           " 重复上一次修改
```

**数字前缀 + 动作**是 Vim 的语法核心：`3dd`、`5j`、`2dw`、`4x` 都是“重复次数 + 操作”。

### 2.6 查找与替换

```vim
/pattern        " 向下查找，n / N 跳转
?pattern        " 向上查找
*               " 查找光标下的单词
:%s/old/new/g   " 全文替换
:%s/old/new/gc  " 全文替换并逐个确认
:5,15s/a/b/g    " 指定行范围替换
```

### 2.7 分屏与多文件

```vim
:sp file    " 水平分屏
:vsp file   " 垂直分屏
Ctrl-w h/j/k/l  " 在窗口间移动
:tabnew     " 新建标签页，gt / gT 切换
:e file     " 打开另一个文件
```

### 2.8 `.vimrc` 常用配置

```vim
set number          " 显示行号
set relativenumber  " 相对行号，配合 j/k 很好用
set tabstop=4       " Tab 显示宽度
set shiftwidth=4    " 自动缩进宽度
set expandtab       " Tab 转空格
set cursorline      " 高亮当前行
syntax on           " 语法高亮
```

---

## 三、Shell 与脚本

### 3.1 什么是 Shell

Shell 是用户与内核之间的**命令解释器**，接收命令、交给内核执行、返回结果。常见实现有 `bash`、`zsh`、`sh`，本课程默认 `bash`。

### 3.2 脚本骨架

```bash
#!/usr/bin/env bash
# 第一行 shebang 指定解释器，脚本需要可执行权限（chmod +x）

set -euo pipefail   # 出错即停 / 未定义变量报错 / 管道中任一环节失败即失败
```

> `set -euo pipefail` 几乎是**每个脚本都该加**的一行，能避免错误被静默吞掉。

### 3.3 变量

```bash
name="hajsnz"          # 赋值时等号两边不能有空格
echo "Hello, $name"    # 使用 $ 引用
echo "${name}_notes"   # ${} 明确变量边界，推荐
readonly PI=3.14       # 只读变量
unset name             # 删除变量
```

**引号的区别（重点）**：

| 写法 | 行为 |
| --- | --- |
| `'abc $name'` | 单引号：原样输出，不解析变量 |
| `"abc $name"` | 双引号：解析变量与命令替换 |
| `abc $name` | 不加引号：会词分割、通配符展开，**容易出 bug** |
| `"$var"` | 最佳实践：变量几乎总该加双引号 |

### 3.4 特殊变量

| 变量 | 含义 |
| --- | --- |
| `$0` | 脚本名 |
| `$1`…`$9` | 第 1…9 个参数 |
| `$#` | 参数个数 |
| `$@` | 所有参数（加引号时每个参数独立） |
| `$?` | 上一条命令的退出码，0 表示成功 |
| `$$` | 当前脚本的 PID |

### 3.5 条件判断

```bash
if [[ -f "$file" ]]; then
    echo "$file 存在且是普通文件"
elif [[ -d "$file" ]]; then
    echo "$file 是目录"
else
    echo "不存在"
fi

# 常用测试
[[ -f path ]]   # 是文件
[[ -d path ]]   # 是目录
[[ -z "$s" ]]   # 字符串为空
[[ -n "$s" ]]   # 字符串非空
[[ "$a" == "$b" ]]      # 字符串相等
[[ "$a" -eq "$b" ]]     # 数值相等（-ne -lt -gt -le -ge）
```

### 3.6 循环

```bash
# 列表循环
for f in *.md; do
    echo "处理 $f"
done

# C 风格
for ((i = 1; i <= 5; i++)); do
    echo "$i"
done

# 逐行读文件（推荐 while read）
while IFS= read -r line; do
    echo "$line"
done < input.txt
```

### 3.7 函数

```bash
greet() {
    local name="$1"          # local 限制作用域
    echo "Hello, $name"
}

greet "hajsnz"
```

### 3.8 常用技巧

```bash
$(command)          # 命令替换：把命令输出作为值
$((1 + 2))          # 算术展开
*.md                # 通配符：任意字符串
file-?.txt          # 单个字符
{a,b,c}.txt         # 花括号展开 → a.txt b.txt c.txt
sort | uniq -c      # 排序去重并计数
xargs               # 把 stdin 变成参数
seq 1 10            # 生成序列
```

### 3.9 本课作业脚本要点

作业要求脚本输出「系统时间 + 最近 10 条命令 + 笔记字数」并重定向到同目录 `output.txt`，核心三行：

```bash
date                              # 系统时间
history | tail -n 10              # 最近 10 条命令
wc -w ../notes/*.md               # 笔记字数
```

再整体重定向：`{ ...; } > output.txt` 或 `exec > output.txt`。

---

## 四、Git 与 GitHub

### 4.1 三个区域

```
工作区 (Working Directory)  →  git add  →  暂存区 (Staging Area)  →  git commit  →  本地版本库 (Repository)
                                                                                    →  git push  →  远程仓库 (Remote)
```

理解这三个区域，Git 的大部分命令就能对上号。

### 4.2 初始配置

```bash
git config --global user.name  "hajsnz"
git config --global user.email "you@example.com"
git config --global init.defaultBranch main
git config --list                 # 查看配置
```

### 4.3 基本流程

```bash
git init                    # 初始化仓库
git status                  # 查看状态（最常用）
git add file / git add .    # 加入暂存区
git commit -m "Add notes."  # 提交
git log --oneline --graph   # 查看历史
git diff                    # 工作区 vs 暂存区
git diff --staged           # 暂存区 vs 最近一次提交
```

> **规范要求**：commit message 用英文、**首字母大写、以句号结尾**，例如 `Add first lesson notes.`。

### 4.4 分支

```bash
git branch                     # 列出分支
git switch -c 2027/hajsnz      # 创建并切换分支（新版写法）
git checkout -b 2027/hajsnz    # 旧写法，等价
git switch master              # 切回 master
git merge 2027/hajsnz          # 把分支合并进当前分支
git branch -d 2027/hajsnz      # 删除已合并分支
```

### 4.5 远程协作

```bash
git remote add origin https://github.com/hajsnz/missing-semester.git
git remote -v
git push -u origin 2027/hajsnz    # 首次推送并建立追踪
git push                          # 之后直接推
git fetch origin                  # 只拉取不合并
git pull --rebase origin master   # 拉取并变基，保持历史线性
```

### 4.6 撤销与后悔药

| 场景 | 命令 |
| --- | --- |
| 撤销工作区某文件改动 | `git restore file` |
| 取消暂存 | `git restore --staged file` |
| 撤销最近一次提交（保留改动） | `git reset --soft HEAD~1` |
| 撤销提交并取消暂存 | `git reset HEAD~1` |
| 彻底丢弃提交与改动 | `git reset --hard HEAD~1`（危险） |
| 已推送后安全回滚 | `git revert <commit>` |
| 临时保存改动 | `git stash` / `git stash pop` |

### 4.7 冲突处理

多人改同一处时 `git merge` / `git pull` 会冲突，文件里出现：

```
<<<<<<< HEAD
你的版本
=======
别人的版本
>>>>>>> other-branch
```

处理步骤：

1. 打开文件，**手工决定保留哪部分**，删掉 `<<<<<<<`、`=======`、`>>>>>>>` 标记；
2. `git add <file>` 标记为已解决；
3. `git commit`（merge）或 `git rebase --continue`（rebase）。

> 关键：**不要用 close PR 再重开的方式逃避冲突**，TASK.md 明确说会扣分。

### 4.8 GitHub 协作流程（Fork + PR）

```bash
# 1. 在 GitHub 页面点 Fork，得到自己的副本 hajsnz/missing-semester
# 2. 克隆自己的副本
git clone https://github.com/hajsnz/missing-semester.git
cd missing-semester

# 3. 添加上游仓库，方便同步
git remote add upstream https://github.com/OceanCing/missing-semester.git
git fetch upstream

# 4. 从最新 master 建开发分支
git switch -c 2027/hajsnz

# 5. 写作业、提交
git add 2027/hajsnz
git commit -m "Add first lesson notes and homework script."

# 6. 推送到自己的仓库
git push -u origin 2027/hajsnz

# 7. 在 GitHub 上向 OceanCing/missing-semester 发起 Pull Request
```

**PR 要求**：

- 标题英文、首字母大写；
- 描述可用中文，但要写清楚**改了什么、为什么改**；
- 提交后在 PR 里继续关注 comments，有问题就改代码再 push，PR 会自动更新。

### 4.9 `.gitignore`

```
*.log
node_modules/
.env
output.txt      # 作业生成的输出文件，可选
```

### 4.10 常见操作对照

| 想做什么 | 命令 |
| --- | --- |
| 看谁改的某行 | `git blame file` |
| 临时切到某次提交看代码 | `git switch --detach <sha>` |
| 找哪次提交引入了 bug | `git bisect` |
| 打标签 | `git tag -a v1.0 -m "Release 1.0."`、`git push --tags` |
| 只挑一个提交到当前分支 | `git cherry-pick <sha>` |

---

## 五、易错点与小结

### 5.1 高频踩坑

1. **变量赋值加空格**：`name = "x"` 会当成命令执行，必须写 `name="x"`。
2. **不引用变量**：路径含空格时 `rm $file` 会误删，应写 `rm "$file"`。
3. **`cd` 不检查失败**：`cd /wrong && rm -rf *` 里的 `&&` 能救命，脚本里可加 `set -e`。
4. **往仓库提交敏感信息**：token、密码写进代码后很难彻底清除。
5. **直接 `git push -f`**：会覆盖别人历史，公共分支禁用。
6. **提交前不 `git status`**：容易漏文件或误提交临时文件。
7. **Vim 里找不到退出方式**：先 `Esc`，再 `:q!`。

### 5.2 一句话总结

- **Linux**：一切皆文件，小工具通过管道组合出强大能力；
- **Vim**：模式化编辑 + 动词/名词组合，熟练后效率极高；
- **Shell**：把重复劳动写成脚本，`set -euo pipefail` 保证健壮；
- **Git**：理解「工作区 → 暂存区 → 版本库」三区模型，Fork + PR 是开源协作的标准姿势。

### 5.3 课后待办

- [x] 完成第一课笔记
- [x] 编写作业脚本并生成 `output.txt`
- [ ] 后续课程继续在 `2027/hajsnz/notes` 与 `homework` 中积累
- [ ] 关注 PR comments，及时修正
