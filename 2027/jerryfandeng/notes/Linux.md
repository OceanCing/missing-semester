## Linux与发行版
- Linux通常指Linux内核
- Linux发行版指内核+软件包+默认配置(如ubuntu)

## 文件树
- ~(等于/home/user)是用户目录 /是文件系统根目录

## 常见shell命令
|命令|作用|
|---|---|
|cd|进入目录|
|mkdir|创建目录|
|mv|移动目录或文件|
|touch|创建文件|
|rm|删除文件或目录|

## shell常用组合
### 管道
- ls -la | grep \.c(查找目录下所有的.c文件)
### 重定向(> 与 >>)
- echo "hello" > hello.txt(完全覆盖)
- echo "hello" >> hello.txt(文件末尾追加)

## vim
- 普通模式(yy复制当前行 p粘贴 dd删除当前行 u撤销 /word查找单词)
- 插入模式(普通模式按i进入 esc退出)
- 命令行模式(普通模式:进入  :wq保存并退出)