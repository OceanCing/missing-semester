#控制培训第一课

## Linux基础：

1. 目录
目录从/开始
- /home/user  用户文件
- /etc        系统配置
- usr         程序与库
- var         日志等可变数据
- dev         设备文件

2. 路径分类
- 相对路径：
 ./test.txt   表示从当前位置开始
- 绝对路径；
/home/eason/snap 从/开始
- ~/example  表示home目录中的

3. 当前位置与目录切换
- pwd       显示当前目录
- ls        列出目录的内容
- cd 目录   进入指定目录
- cd ..     返回上一级
- cd ~      返回到家目录

4. 文件和目录操作
| 命令 | 作用 | 示例 |
| ---- | ---- | ---- |
| `mkdir -p` | 创建目录，允许补齐父目录 | mkdir -p ~/linux |
| `touch` | 创建空文件 / 更新时间 | touch hello.txt |
| ` cp` | 复制文件 | cp hello.txt hello-copy.txt |
| ` mv` | 移动或重命名 | mv hello-copy.txt backup.txt |
| ` rm` | 删除文件 | rm backup.txt | 
| ` cat` /  `less` | 直接查看 / 分页查看 | cat hello.txt |
 **`rm`通常不进入回收站，删除前先确认路径和文件名**

## shell的四种常用组合

- 变量  `name="RM" echo "$name"`       保存一个值，再通过变量引用它
- 重定向 `echo "hello" > hello.txt`    >覆盖写入；>>追加到文件末尾
- 管道 `ls -la | grep txt`             将输出交给下一个程序
- 条件连接  `mkdir -p demo && cd demo` 前一条成功后，在执行下一条

## 陌生命令的帮助入口

- `ls --help`   快速查看选项和用法
- `man ls`      打开手册，按q退出
- `tab`         补全命令和路径
- 上 / 下       调出历史命令   
 
## Vim
 
1. 模式
- 普通模式：移动、删除和复制
- 插入模式：输入文本
- 命令行： 保存退出

2. vim模式与按键
- `i`   普通到插入
- `esc` 返回普通模式
- `:`   普通到命令行

- `h j k l`  左/下/上/右
- `dd`      删除当前行
- `yy / p`  复制当前行 / 粘贴 
- `u`       撤销 
- `/word`   搜索 word

- `:w`  保存
- `:q`  退出
- `:wq` 保存退出
- `:q!` 放弃修改并退出

## Git
 
1. git的常用命令：     获取和查看
- `git clone < >`      下在仓库及提交地址
- `git status`         查看当前分支以及文件状态
- `git diff`           查看未暂存的修改 
- `git log --online`   用简短的格式查看提交历史

2. 常用命令：分支与提交
- `git switch -c feature/hello`       创建并切换到功能分支
- `git add hello.txt`                 暂存指定文件
- `git commit -m "add greeting"`      记录一次本地提交
- `git push -u origin feature/hello`  推送并建立追踪关系
- `git switch main`                   切回主分支
- `git pull`                          获取并整合当前的分支的远程更新
 

