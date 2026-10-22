# Lesson 1 Notes: Linux, Shell & Vim, Git & GitHub
> 一句话总结：从进 Linux、敲命令、用 Vim，到用 Git 记录版本、在 GitHub 上提 PR 的完整开发链。

## 1. Linux
- Linux 是内核
- 目录从 `/` 开始；`~` 是家目录，`.` 当前，`..` 上级；绝对路径 vs 相对路径
- 路径大小写敏感：`Hello.txt` ≠ `hello.txt`
-一些目录：用户文件是/home/user，系统配置是/etc，程序与库是/usr等等 

## 2. Shell & Vim
`ls -la ~/projects` 这类命令的结构是 `命令 [选项] [参数]`
- 必背命令：`pwd` `ls` `cd` `mkdir -p` `touch` `cp` `mv` `rm` `cat`/`less`
- 四种组合：变量 / 重定向（`>` `>>`）/ 管道（`|`）/ 条件连接（`&&`）
- Vim 三模式：普通 / 插入(`i`) / 命令行(`:`)

| 按键 | 作用 |
| u | 撤销 |
| h j k l | 左 下 上 右 |
| dd | 删一行 |
| :wq | 保存退出 |

```bash
# 我练过的命令示例
ls -la
mkdir -p ~/linux_demo
cd ~/linux_demo
```


## 3. Git & GitHub
- Git（本地版本控制） vs GitHub（远程托管协作）
- 四区：工作区 → 暂存区 → 本地仓库 → 远程仓库；**commit 只进本地，push 才上远程**
- Fork 模式工作流：Fork → Clone → 分支 → 改 → PR

