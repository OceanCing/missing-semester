## 系统常用命令

### 帮助
##### 在命令后加上-h或者-help可以查看这个命令可以加入什么参数或者这个参数有什么用
    xxx -h / xxx -help					
##### 使用man + 命令也可以查看手册, 有命令的参数形式和英文解释
    man xxx						

### 调出历史
###### 可以调出上次命令
    up down 						

### 目录操作：
##### 终端打开后默认是用户目录 /home/seed 用户目录，也用 ~ 来表示
##### 用户目录的上一层是 /home 目录
##### 再上一层是根目录 /
##### 使用cd可以改变当前目录, 进入指定目录
    cd 地址						
##### . 表示当前目录, 进入当前目录的xxx文件
    cd ./xxx 						
##### .. 表示上一级目录, 进入上一级目录
    cd ..							
##### 查看磁盘容量
    df -h

### 文件操作:

#### 创建:
##### 在当前目录创建xxx文件
    mkdir xxx					
##### 在 /home/seed/文档/ 地址下创建名为Ohyeah的文件	
    mkdir /home/seed/文档/Ohyeah	
##### 创建多级路径
###### -p 可以使父目录不存在的时候自动创建
    mkdir -p a/b/c
##### 在当前目录创建名为xxx的.txt文件
    touch xxx.txt					
##### 在 /地址 下创建名为xxx的.txt文件, 若已存在则可以更新文件时间
    touch /地址/xxx.txt				

#### 复制:
    cp hello.txt hello-copy.txt

#### 移动或重命名:

##### 重命名文件
    mv hello.txt hello-yeh.txt

##### 移动文件到另一个目录:
###### 已在该目录的文件前路径可省略, 移动到另一个目录注意是 路径/ , 最后空着表示目录
    mv file.txt /home/user/documents
    mv mv /home/seed/文档/Markdown/Markdown_Library.md /home/seed/文档/命令/
##### 移动并重命名:
    mv file.txt /home/user/documents/newfilename.txt
##### 移动多个文件到目录:
    mv a.txt b.txt c.txt /home/user/

#### 删除:
##### 删除文件:
    rm 文件名
    rm test.txt
##### 删除文件夹:
    rm -r 文件夹名
    rm -r file
##### 强制删除带有权限保护的文件夹:
    sudo rm -rf 文件夹名
    sudo rm -rf build



### 命令行环境
###### 值得注意的是, shell不会保存别名, 如果想让别名持续生效, 就需要放进 .bashrc 或者 .zshrc
    alias alias_name="command_toalias arg1 arg2"		#让alias_name等价为 "命令"
    alias sl="ls"						#让sl等价为ls

### 数据整理
##### 输出命令行的日志
    cat .bash_history			
##### 使用管道运算符 | 在.bash_history输出的命令中执行grep apt, 查找含有apt的日志		
    cat .bash_history | grep apt				
##### 使用less可以创建一个文件分页器, 可以通过翻页的方式浏览较长的文本
    cat .bash_history | grep apt | less 			
##### sed: 基于文本编辑器ed, 可用一些的简短的命令修改文件, ''是正则表达式
###### 其中的s命令是替换命令, 实现把file1中匹配Dynamicx的文字替换为dx, /g则会更改每一个匹配项
    cat file1 | sed 's/DynamicX/dx/g'			



### 常用组合
#### 变量
    name = "RM"						#保存一个值, 再通过变量引用它
    echo $(name)

#### 重定向
    echo "hello" > hello.txt				#>覆盖写入; >>在末尾追加
    echo "world" >> hello.txt

#### 管道 
    ls -la grep txt						#将输出交给下一个程序

#### 条件连接
    mkdir -p demo && cd demo				#前一条执行成功后, 执行下一条



## 下载安装软件：
#### 1.apt命令：
    sudo apt update		#刷新软件列表
    sudo apt install 软件名	#安装
    sudo apt remove 软件名		#卸载

#### 2.deb安装包：
    sudo dpkg -i 软件包.deb	   #通过在网站中下载的.deb安装软件，dpkg是Debian Package的缩写，负责解包、安装、卸载软件的工具，-i意味install
    sudo apt -f install		#若损坏则修复

#### 3.Snap安装
    sudo snap install 软件名 	

#### 4.AppImage(免安装绿色版)
    chmode +x 软件.AppImage		#直接赋予执行权限
    ./软件.AppImage			#直接运行

#### 5.可直接解压运行的文件:
    右键解压, 进入bin内, 打开终端
    ./xxx.sh			#直接使用命令运行该程序


## Vim

#### 打开Vim编辑flie(若已存在, 不存在就会创建): 
    Vim file

##### 进入Vim之后, 先按 i , 即可开始编辑
##### 编辑之后, 按Esc退出编辑模式, 进入普通模式
##### 按wq保存并退出, 按q!不保存并退出

##### dd 在普通模式，可删除一行、撤销、搜索
按下d之后, 继续按d就会删除, 一直按着d就会一直删除
输入d1000就会删除光标后1000行字

##### 全选、复制:
按下gg, 光标就会跳到最前
按下大写V, 就会开始选中内容
按下大写G, 就会跳到最后
按下 *y, 就会把选中的内容复制进剪贴板

##### xclip配置:
在普通模式下, 即Esc下, 按\ac可以全选并复制, 按Enter就可以复制到宿主机剪贴板
使用大写V选中的内容可以按\c复制, 按Enter也可复制到剪贴板