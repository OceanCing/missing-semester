# 第一课笔记

## 一、Linux 基础

Linux 是一种常用的操作系统内核，Ubuntu 是 Linux 的一种发行版。

常用目录：

- `/`：根目录
- `~`：用户主目录
- `.`：当前目录
- `..`：上一级目录

常用命令：

```bash
pwd
ls -la
cd ..
```

## 二、Shell 常用命令

| 命令 | 作用 |
|---|---|
| `pwd` | 查看当前路径 |
| `ls` | 查看目录内容 |
| `mkdir` | 创建目录 |
| `cp` | 复制文件 |
| `mv` | 移动或重命名文件 |
| `rm` | 删除文件 |

Shell 中常见的符号：

- `>`：覆盖写入
- `>>`：追加内容
- `|`：管道
- `&&`：前一个命令成功后执行下一个命令

## 三、Vim 编辑器

Vim 是一种终端文本编辑器。

- `i/a`：进入插入模式
- `Esc`：返回普通模式
- `dd`：删除当前行
- `u`：撤销
- `:wq`：保存并退出
- `:q!`：不保存退出

## 四、Git 与 GitHub

Git 用于版本控制，GitHub 用于远程托管和团队协作。

常用 Git 命令：

```bash
git status
git add 文件名
git commit -m "Add notes"
git push
```

GitHub 协作流程：

1. Fork：复制仓库到自己的账号。
2. Clone：克隆仓库到本地。
3. Branch：创建开发分支。
4. Commit：提交修改。
5. Push：推送到远程仓库。
6. Pull Request：请求团队评审修改。
