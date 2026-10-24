# NOTE

This is all my note for code study

## 第一次培训课
### 1.Linux
- `~` 为当前用户的家目录
- `.` 为当前目录
- `..` 为上一级
- `/home/user` 用户文件
- `/etc` 系统配置
- `/usr` 程序与库
- `/var` 日志等可变数据
- `/dev` 设备文件

### 2.Vim && Shell
**终端输入命令，由Shell解释，再交给程序运行**
-Shell命令结构`命令 [选项] [参数]`

- **命令**
- `pwd` 显示当前目录
- `ls` 列出目录内容
- `cd 目录` 进入指定目录
- `cd ..`返回上一级
- `cd ~`返回家目录
- `mkdir -p` 创建目录允许补全父目录
- `touch` 创建空文集/更新时间
- `cp` 复制文件
- `mv` 移动或重命名
- `rm` 删除文件 ***注意！rm通常不进回收站，删除前先确认路径和文件名！！！***
- `cat/less` 直接查看/分页查看

- **Shell的四种常用组合**
- 1.变量 `name="RM"`  保存一个值，再通过变量引用它
         `echo "$name"`
- 2.重定向 `echo"hello" > hello.txt`   `>`为覆盖写入；‘>>’ 为追加到文件末尾
- 3.管道 `ls -la | grep txt` 将输出交给下一个程序
- 4.条件连接 `mkdir -p demo && cd demo` 前一条成功后，再执行下一条

- **陌生命令的帮助入口**
- `ls --help` 快速查看选项和用法
- `man ls` 打开手册，按q退出
- `tab`  补全命令或路径
- `↑ / ↓` 调出历史命令

- **vim的基本使用**
- `:w` 保存
- `:q` 退出
- `:wq` 保存退出
- `:q!` 放弃修改并退出
- `h j k l` 左/下/上/右
- `dd` 删除当前行
- `yy/p` 复制当前行
- `u` 撤销
- `/word` 搜索word

### 3.Git && GitHub
-**Git 常用命令：获取与查看**
- `git clone <仓库地址>` 下载仓库及提交历史
- `git status` 查看当前分支和文件状态
- `git diff` 查看尚未暂存的修改
- `git log --oneline` 用简短格式查看提交历史
- `git switch -c feature/hello` 创建并切换到功能分支
- `add hello.txt` 暂存指定文件
- `commit -m "Add greeting"` 记录一次本地提交
- `push -u origin feature/hello` 推送并建立跟随关系
- `switch main`切回主分支
- `pull` 获取并整合当前分支的远程更新
