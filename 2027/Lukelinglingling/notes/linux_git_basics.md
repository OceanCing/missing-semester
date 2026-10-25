# Linux & Git 基础笔记

## Linux 目录结构
- Linux 的目录从 `/` 开始
- `/home/user`：用户文件
- `bin`：二进制
- `boot`：启动
- `etc`：数据库（配置文件）
- `home`：挂在根目录上

## Terminal 与 Shell
- Terminal 终端 → Shell (Bash/Zsh) → 程序 / 系统

## 常用命令
- `pwd`：查看路径
- `ls`：列出有哪些文件
- `ls -la`：列出全部（包含隐藏文件）
- `mkdir xxx`：创建一个名为 xxx 的文件夹
- `cd xxx`：进入 xxx
  - `..`：上级目录
  - `~`：根目录（注：Linux 中 ~ 通常指家目录）
  - `.`：当前目录
- `rm xxx`：删除，**不进回收站**，使用需谨慎
- `touch xxx`：创建但不编辑
- `cp`：复制文件
- `mv`：移动 / 重命名
- `cat` / `less`：直接查看 / 分页查看
- `bat xxx`：交互式查看
- 按 `Tab` 可以自动补全

## Shell 命令的结构
命令 [选项] [参数]

## 变量与重定向
- `name="RM"`：保存一个值，再用变量引用它
- `echo "$name"`
- `echo "hello" > hello.txt`：`>` 覆盖写入
- `>>`：追加到文件末尾

## 管道与条件连接
- `ls -la | grep txt`：将输出交给下一个程序
- `mkdir -p demo && cd demo`

## 帮助与历史
- `ls --help`
- `man ls`：查看选项 & 用法
- 上下左右键可看历史命令

## Vim 模式
- 普通模式 → 按 `i` → 插入模式
- 普通模式 → 按 `:` → 命令行输入
- `:w` 保存；`:q` 退出；`:wq` 保存并退出；`:q!` 放弃修改并退出

## Git & GitHub
- Git：本地版本控制
- `.git`：核心文件夹
- GitHub：远程托管与协作
- 工作流：工作区 → `git add` → 暂存区 → `git commit` → 本地仓库 → `git push` → GitHub 远程仓库
- **git commit 只记录到本地仓库；push 才把提交发送到远程**
