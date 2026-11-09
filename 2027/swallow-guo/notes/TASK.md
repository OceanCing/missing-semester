#训一
##基础概念理解
      本地，就是电脑里面的ubuntu系统
      远程（remote）,就是github网站服务器
      clone,远程下载到本地
      push,本地上传到远程

##linux使用指南
      目录认识：
                输入：   `ls /` 就能看到全部一级目录
**/etc**
存放系统配置：用户、服务、环境变量
**/home*
普通用户目录
**/root**
root 管理员专属家目录，普通用户一般无权进入。
**/tmp**
临时文件夹，任何用户可读可写，重启文件全部丢失。
**/usr**
安装大部分应用程序，类似 Windows 的`Program Files`
- `/usr/bin` 用户命令
- `/usr/lib` 程序库
- `/usr/share` 文档、资源
**/var**
会不断变大的数据
`/var/log` 系统日志，排查报错最常用！
       
       两个特殊目录
             - `.` 当前目录
             - `..` 上一级目录

## 路径两种写法

**绝对路径**：从 `/` 开始写
`/home/ubuntu/test.txt`

 **相对路径**：相对于当前目录，不从 `/` 开头
当前在 `/home/ubuntu`，直接写 `test.txt`
相对路径是以当前位置为起点


##linux命令
ls /                 查看根目录
ls /etc
ls /home
pwd                  看当前位置
cd /etc              跳转到etc（绝对路径）
cd ../home           相对路径跳转

##文件与目录操作
mkdir -p	创建目录，允许补齐父目录	mkdir -p /linux_demo
touch   	创建空文件 / 更新文件时间	touch hello.txt
cp	        复制文件                	cp hello.txt hello-copy.txt
mv	        移动或重命名	            mv hello-copy.txt backup.txt
rm	        删除文件
rm          通常不进回收站，删除前先确认路径和文件名	             rm backup.txt
cat      	查看文件内容	                                         cat hello.txt


##Shell 的四种常用组合
类型	示例	说明
变量	name="RM"
        echo "$name"	         保存一个值，再通过变量引用它
重定向	echo "hello">hello.txt	> 覆盖写入；>> 追加到文件末尾
管道	ls -la | grep txt	将输出交给下一个程序处理
条件连接	mkdir -p demo && cd demo	前一条命令执行成功后，再执行下一条



###重点说明
**重定向**
- `>`：清空原有文件内容，再写入新内容（覆盖）
- `>>`：在文件最后面追加文字，不会删掉原来内容
**管道 `|`**：把前面命令输出结果，传给后面命令作为输入。`ls -la | grep txt` 就是列出所有文件，筛选名字含`txt`的。
**`&&` 条件连接**：前面命令返回成功，才执行后面；如果前面失败，后面不会运行。



##vim笔记
   模式：
       - 普通模式：移动、删除、复制
       - 插入模式：输入文本
       - 命令行：保存、退出
   模式切换：
        普通模式  --(按i)-->  插入模式
        插入模式  --(Esc)-->  普通模式
        普通模式  --(按:)-->  命令行输入
   普通模式按键：
            h j k l	左 / 下 / 上 / 右
            dd	删除当前行
            yy / p	复制当前行 / 粘贴
            u 	撤销
            /word	搜索 word

  命令行命令（普通模式下先输入 `:`）：
    
          :w	保存
          :q	退出
          :wq	保存并退出
          :q!	放弃修改，强制退出 
  Vim 基本使用流程:
	vim hello.txt	打开练习文件
	i	进入插入模式，输入文本
	Esc	返回普通模式
	:wq	保存并退出
	vim hello.txt	再次打开文件
	dd / u / /word	删除一行、撤销、搜索

