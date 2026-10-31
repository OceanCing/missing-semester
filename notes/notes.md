#First class
1.Linux本质上是一个操作系统“内核”，Ubuntu是外壳
2.目录路径（系统文件夹）：/home/user 用户文件
/etc 系统配置
/usr 程序与库
/var 日志等可变数据
/dev 设备文件
tips:1.~-家目录 eg.cd~
2..-当前目录
3...-上一级目录 eg.cd..
（绝对路径从/开始，相对路径从.开始）
3.专业名词：
shell：解释命令然后程序执行（Linux默认的shell是Bash）
ls(list):查看文件，列出文件夹内容（针对文件夹）
Git(软件):可存档与回档，分支与提交
(1)git clone:把网站上文件克隆到你的电脑
(2)git checkout -b.dev:建dev分支
(3)git commit/add:拍照存档
(4)git push:把存档上传到网站
mkdir(make directory):创建目录（文件夹）
cd(change directory):切换当前所在目录
vim:文本编辑器
(1)Esc:返回普通模式
(2)i:进入插入模式，输入文本
(3):wq:保存并退出
(4)dd u /word:删除一行、撤销、搜索
-al(long,all):用于ls后面，列出包括隐藏项的所有文件
pwd(print working directory):显示当前工作目录/文件夹
-p(parents):递归创建目录（防止报错）eg.mkdir. -p a/b/c
touch:创建一个空文件/更新已有文件时间（可连续建多个文件）
cp(copy):复制文件/文件夹并粘贴（复制文件夹前加-r）
rm(remove):删除（无法取回）
cat/less(concatenate):打印全部文件内容（针对文件）
ech:回声，打印（>:覆盖写入；>>:追加写入）
man(manual):（+命令）可查看官方说明手册
--help:告诉命令参数（-l,-a什么意思）

