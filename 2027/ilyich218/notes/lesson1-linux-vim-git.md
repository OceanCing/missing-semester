# 第一课笔记

## linux

|代码|作用|
|-|-|
|`cd`|进入指定目录|
|`cd ..`|返回上一级|
|`cd ~`|返回 home 目录|
|`mkdir -p`|创建目录，允许补齐父目录|
|`touch`|创建空文件 / 更新时间|
|`cp`|复制文件|
|`mv`|移动或重命名|
|`rm`|删除文件|
|`cat / less`|直接查看 / 分页查看|



> 补齐父目录：如果本来没有这个目录，（`-p`）可以自动把这个没有的目录补上去，创建这个目录，没有`-p`就不能进行这个操作

## Vim 语法

|按键|作用|
|-|-|
|`h` `j` `k` `l`|左 / 下 / 上 / 右|
|`dd`|删除当前行|
|`yy` / `p`|复制当前行 / 粘贴|
|`u`|撤销|
|`/word`|搜索 word|

|代码|作用|
|-|-|
|`:w`|保存|
|`:q`|退出|
|`:wq`|保存退出|
|`:q!`|放弃修改并退出|

## shell用处

>terminal终端输入→shell解释命令→查看文件 管理文本

|命令|用处|
|-|-|
|`ls`|做什么（命令）|
|`-la`|怎么执行（选项）|
|`~/projects`|操作对象（参数）|



|组合|代码|作用|
|-|-|-|
|变量|`name="RM"`、`echo "$name"`|保存一个值，再通过变量引用它|
|重定向|`echo "hello" > hello.txt`|`>` 覆盖写入；`>>` 追加到文件末尾|
|管道|`ls -la \| grep txt`|将输出交给下一个程序|
|条件连接|`mkdir -p demo && cd demo`|前一条成功后再执行下一条|

## git and github

### git

>首次使用git要提交作者信息
>git config --global user.name "Your Name"
>git config --global user.email "you@example.com"

### SSH

>密钥远程认证
>ssh-keygen -t ed25519 -C "本人邮箱"
>cat ~/.ssh/id_ed25519.pub
>其中公钥为id_ed25519.pub    私钥没有“.pub”


