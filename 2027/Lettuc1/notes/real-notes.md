# 培训第一课笔记

## Linux终端指令
- cd ~ 回到home
- cd 进入某目录
- cd.. 回到上一级目录
- ls 列出目录内容
- mkdir -p 创建目录，允许补齐父目录（make direction（？））
- touch 创建空文件
- cp 复制文件（copy
- mv 移动或者重命名文件（move
- rm 删除文件（remove
- cat/less 直接查看/分页查看文件

*如果不记得了，就用‘ls --help’快速查看选项和用法，或者‘man ls’打开手册（按q退出*
- ### 一定要看清当前目录！不要在回收站里操作！

## shell的四种常用组合
- 变量 ‘=’赋值，‘echo’打印
- 重定向 ‘>’覆盖写入，'>>'，追加到文件末尾
- 管道 ‘|’，上一个命令的输出内容是下一个命令的输入内容
- 条件连接 ‘&&’前一条成功执行后，执行下一条（类似if

## vim的基本使用
- vim <某文件> 打开文件
- i 进入插入模式
- Esc 切换回普通模式
- :wq 保存并退出
- dd 删除一行
- u 撤销
- /某关键词 搜索

## git常用命令
- git swich -c xxxx 创建并切换至功能分支
- git add xxxx 暂存指定文件
- git commit -m "xxxx" 记录一次本地提交
- git push -u origin xxxx 推送并建立跟踪关系
- git switch main 切回主分支
- git pull 获取并整合当前分支的远程更新

*简而言之，先fork目标仓库，然后clone到本地，git add到工作区，git commit到本地暂存，git push到远程，然后交PR*
