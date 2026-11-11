linux--Vim--Git--Github PR
系统：Windows11 macOS Linux（Ubuntu）
Linux（内核） Ubuntu(发行版)
Linux在机器人开发中的作用：开发电脑通过网络连接机器人主机，控制程序在无桌面环境运行
/home/user 用户文件
/etc 系统配置
/user 程序与库
/var 日志等可变数据
/dev 设备文件
重要shell脚本 ：
/home/user/example
从/开始
./hello.txt
从当前位置开始
～/example
home目录中的example

Shell脚本
Terminal终端——Shell（bash）——程序/系统
ps.
ls查看文件
git管理版本

shell命令的结构
e.g. ls（命令）-做什么
     -la(选项)-怎样执行
     ~/projects(参数)-操作哪个对象
     
pwd---显示当前目录
ls---列出目录内容
cd"目录"---进入指定目录
cd..---返回上一级
cd~---返回home目录

常用文件和目录操作
mkdir -p   创建目录         例子：mkdir -p ~/linux_demo
touch   创建空文件      例子：touch hello.txt
cp    复制文件       例子: cp hello.txt hello-copy.txt
mv    移动或重命名       例子:mv hello-copy.txt backup.txt
clear   清空Terminal终端内的指令
sudo   下载
cat   查看      例子：cat hello.txt

Vim的使用
（编辑器）
普通模式：移动 删除 复制
插入模式：输入文本
命令行 ---
:w    保存
:q    退出
:wq   保存并退出
esc   退出插入模式
i     进入插入模式


Git
第一步：录入姓名和邮箱地址（作者信息）
第二步：设置ssh公钥（可以公开） 私钥（不可公开）
git clone <仓库地址>    下载仓库及提交历史
git status    查看当前分支和文件状态
git diff    查看尚未暂存的修改
git log --oneline    用简短格式查看提交历史

Fork 仓库副本----clone 克隆到本地---commit修改---push---发起PR ---review评审


