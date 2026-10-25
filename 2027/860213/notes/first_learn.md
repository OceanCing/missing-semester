# markdown学习
## 基础语法
**换行**  
使用两个空格加"enter"  
或者两个“enter”(间距会更大一些，类似换段落)

**加粗**  
使用** **，或者使用__ __  
eg:**加粗示例** __加粗示例__


**斜体**  
使用* *，或者使用_ _  <br>
eg:*加粗示例* _加粗示例_  
注意：如果编辑器会自动删除行尾空格，可以用`<br>`  
1数字旁边的是反引号，可以打出

**又斜体又加粗**  
左右两边都是三个星号  
eg:***示例***

**横杠**  
左右两边两个~~,一个~也可以  
eg:~~示例~~ ~示例~

**分割线**  
enter加上三个连续***或者三个及以上的下划线也可以  
eg:
***
___

**标题**  
一级标题：首位加一个#和空格  
二级标题：两个##和空格  
三级标题：三个###和空格  
同理后续标题，最高六级标题

**空格**

| 写法 | 效果 |
|---|---|
| `&nbsp;` | 一个不换行空格 |
| `&ensp;` | 半个全角空格 |
| `&emsp;` | 一个全角空格，约一个汉字宽 |

## 列表
**无序列表**  
一个*/+/-加上空格（列表内要用相同的）
* 我是第一项任务
* 我是第二项任务

**有序列表**  
数字加上.加上空格
1. 第一行
2. 第二行
3. 第三行

二级列表需要在序号前面加上四个空格,三级列表就8个
1. 第一行
    * 二级列表
        * 三级列表
2. 第二行
    1. 二级列表
        1. 三级列表


序号内换行用第一种，序号外用第二种

**勾选框**  
使用*加上[],框中间空格，框后空格，eg:
*[ ] 我是第一任务

空格替换成x,则变成对勾，eg:
*[x] 我是第一任务

## 代码块
选中程序段按下tab，eg:

    #include <iostream>
    using namespace std;
    int main()
    {
    cout<<"Hello World!"<<endl;
    return 0;
    }

使用三个```可以不用空格和换行，`后面可以写语言类型来声明eg:
```c++
#include <iostream>
    using namespace std;
    int main()
    {
    cout<<"Hello World!"<<endl;
    return 0;
    }
```
## 引用
使用>加空格即可，内部换行按照第一种换行形式，最好每一行前面都加>,嵌套直接加就行 eg:
> 你好

## 超链接
规范形式:`[官网](http)`  
eg:[官网](http)

复杂一点的(`[b]`后面要空两行):
```
[想要][a]购买[商品][b]

[a]: http
[b]: http
```
[想要][a]购买[商品][b]

[a]: http://www.baidu.com
[b]: http://www.baidu.com

## 脚注
使用typora
```
想要购买请前往官网[^1]
[^1]: 我是脚注
```
想要购买请前往官网[^1]  
[^1]: 我是脚注

## 插入
### 图片
```
![图片](https://tse4-mm.cn.bing.net/th/id/OIP-C.yNLxvaCfRGXjL1id4jB_pAHaFj?w=215&h=180&c=7&r=0&o=7&pid=1.7&rm=3)
```  
![图片](https://tse4-mm.cn.bing.net/th/id/OIP-C.yNLxvaCfRGXjL1id4jB_pAHaFj?w=215&h=180&c=7&r=0&o=7&pid=1.7&rm=3)

### 表格
未居中：对齐（：在左边则左对齐，右边右对齐，两边加上则是居中）
```
|姓名|年龄|性别|
|:---:|:---|---:|
|小名111|12|男|
|小红|13|女|
```
|姓名|年龄|性别|
|:---|:---|---:|
|小名1111111|111111112|男11111111|
|小红|13|女|

## 数学公式
空格

|写法|间距|
|---|---|
| `\,` |小空格|
|`\;`|较大空格|
|`\quad`|大空格|
|`\qquad`|两倍大空格|
方程
```
（单行）$x=1+y$
（多行）
***
$$
x+y=10
$$
```
$x=1+y$
***
$$
x+y=10
$$

分数
```
$$
\frac{1+2}{2}
$$
```
$$
\frac{1+2}{2}
$$

上小标  
aligned 会按 & 对齐，每一行单独居中改用 gathered：
```
(上标)x^2
(下标)x_2

换行：
\begin{aligned}
x^2 \\
x_2
\end{aligned}

等号对齐
$$
\begin{gathered}
y = x^2 \\
z = x_2
\end{gathered}
$$
```
$$
\begin{gathered}
x^2 \\
x_2\\
y = x^2 \\
z = x_2
\end{gathered}
$$

根号
```
\sqrt[3]{4}
想要花括号需要转义字符\\
\sqrt[3]{\{4\}}
```
$$
\sqrt[3]{4}\qquad\sqrt[3]{\{4\}}
$$


# Linux
sudo su 超级用户权限

## linux命令基础格式
命令通用格式：`cmd [-options][parameter]`  
command:命令本身 &emsp;-options:可选选项 &emsp;parameter:可选参数

## 常用操作
* ctrl+L 清空终端

## 1. ls
功能：列出当前工作目录下的内容  
语法：`ls [-a -l -h][linux路径]`  
默认加载到home目录

1. -a  
   all:列出所有文件（包含隐藏的文件，文件夹）  
   带 . 的前缀的为隐藏的文件，不用a则查看不了

2. -l  
   以竖向排列的形式展示内容  
   混合使用方法：`ls -a -l` `ls -l -a` `ls -la` `ls -al`

3. -h
   用易于读取的方式列出文件的大小，比如说K，M，G  
   必须要混合使用

## 2. cd
命令格式：`cd [linux路径]`  
cd直接执行表示回到home目录否则切换到哪个目录下

绝对路径：开头必须是/，以根目录为起点  
相对路径：无需/，以当前路径为起点  
特殊路径符：
* . &nbsp;&nbsp;表示当前目录 eg:`cd ./Desktop`
* ..&nbsp;&nbsp;表示上一级目录,eg:`cd ../..切换到上两级，回退两级`
* ~ &nbsp;表示home目录

## 3. pwd
无参数无选项，来查看当前的工作目录  
命令格式:`pwd`

## 4. mkdir
创建新的目录，make dictory  
`mkdir [-p] Linux路径`
1. -p  
   一次性创建多个层级的目录

## 5. touch-cat-more
1. touch  
   创建文件 `touch Linux路径`
2. cat  
   查看文件内容 `cat Linux路径`
3. more  
   查看文件内容,可以翻页查看 `more Linux路径`  
   按空格翻页，按q退出

## 6. cp-mv-rm
1. cp  
   copy,复制文件或者文件夹 `cp [-r] 参数1 参数2`  
   `-r` 复制文件夹需要带上


2. mv  
   表示文件的移动，move，`move 参数1 参数2`  
   改名效果`mv test1 test2`，把新名称给不存在的文件即可


3. rm  
   文件和文件夹的删除，remove  
   `rm [-r -f]参数1 参数2 无限数量的参数`
    1. -r  
       删除文件夹需要

    2. -f  
       force,强制删除，root管理员用户删除内容会有提示

    3. 通配符
   ```
   test*  删除以test开头的内容   
   *test  删除以test结尾的内容  
   *test* 删除包含test的内容
   千万不要使用
   rm -rf/
   rm -rf/*,效果等同于c盘格式化
   ```
## 7. which-find
1. which  
   查看使用的一系列命令的程序文件放在哪里  
   `which 要查找的命令  eg:which cd`

2. find
* 按照文件名查找文件  
  `find 起始路径 -name "被查找的文件名"`

  同样可以使用通配符


* 按照文件大小搜索
  ```
  find 起始路径 -size +或者-n[k M G],k小写
  eg:查找小于10KB的文件 find /-size -10k
  ```  

## 8. grep-wc-管道符
1. grep  
   通过关键字过滤文件行  
   `grep [-n] 关键字 文件路径`  
   -n 表示在结果中显示匹配的行的行号


2. wc  
   统计文件的行数和单词数量  
   `wc [-c -m -l -w] 文件路径`
    * -c  
      统计bytes的数量
    * -m  
      统计字符的数量
    * -l  
      统计行数
    * -w  
      统计单词数量


3. 管道符 |
   将管道左边命令的结果作为右边命令的输入
   ````
   eg:cat test.txt | grep itheima
   左边的结果为查看到的文件的内容，则从内容中找到itheima
   ````

## 9. echo-tail-重定向符
1. echo  
   在命令行中输出指定的内容，复杂的内容可以用“”包围起来  
   `echo 输出的内容`
   ````
   反印号`
   echo `pwd`：pwd作为命令被执行
   ````
2. tail  
   查看文件尾部内容，跟踪文件的最新更改  
   `tail [-f -num] Linux路径`  
   -f 表示持续跟踪  
   -num 表示尾部多少行，不填写默认10行


3. 重定向符
   ````
   > 将左侧命令的结果覆盖写入到符号右侧的文件中（把原本的内容清空）
   eg:echo "hello world" > test1.txt
   
   >> 将左侧命令的结果追加写入右侧文件中
   ````

## 10. vi/vim编辑器
![b1a858f310bde15369cd770eb3b7dddd.png](https://picui.ogmua.cn/s1/2026/10/19/6ad6260da3ee4.webp)

1. 命令模式
   ```
   vi 文件路径
   vim 文件路径
   ```
   vim兼容vi的全部功能  
   如果文件路径不存在则命令用于编辑新的文件，若存在则编辑已经有的文件

|模式|命令|描述|
|---|:---:|---|
|命令|i|在当前光标位置进入输入模式|
|命令|a|光标位置之后进入输入模式|
|命令|I|当前行开头，进入输入模式|
|命令|A|在当前行结尾进入输入模式|
|命令|o|在光标下一行进入输入模式|
|命令|O|在光标上一行进入输入模式|
|输入|esc|回到命令模式|
||键盘上，键盘 k|向上移动光标|
||键盘下，键盘 j|向下移动光标|
||0|移动光标到当前行的开头|
||`$`|移动光标到当前行的结尾|
||pageup|向上翻页|
||pangdown|向下翻页|
||/|进入搜索模式|
||n|向下继续搜索|
||N|向上继续搜索|
||dd|删除光表所在的行|
|    |   ndd |   n是数字，表示删除光标向下n行 |
||yy|复制当前行|
|    |  nyy  |  n是数字，复制当前行和下面的n行  |
||p|粘贴复制的内容|
||u|撤销修改|
||ctrl+r|反向撤销修改|
||gg|跳到首行|
||G|跳到行尾|
||dG|从当前行开始，向下全部删除|
||dgg|从当前行开始，向上全部删除|
||d$|从当前光标开始，删除到本行的结尾|
||d0|从当前光标开始，删除到本行的开头|

2. 输入模式


3. 底线命令模式  
   输入：进入

|命令|描述|
|---|---|
|:wq|保存并退出|
|:q|退出|
|:q!|强制退出|
|:w|保存|
|:set nu|显示行号|
|:set paste|设置粘贴格式|

# git
## git工作流程：

[![e62c31307cc4ff1f2fccd5e8a205ab41.png](https://picui.ogmua.cn/s1/2026/10/20/6ad70cd68260d.webp)](https://picui.ogmua.cn/s1/2026/10/20/6ad70cd68260d.webp)

修改代码 → add (放进暂存区，可以用status查看) → commit (存入仓库) → push 。

## git创建仓库
终端中执行
````
mkdir xx   创建目录
cd  xx     
git init
````

## git常用指令

1. 暂存区
````
git add xxx   （单个文件放进暂存区）
git add .     （把所有工作区文件放进暂存区）
git status    （查看暂存区文件）
````
2. 版本库
````
git commit -m 'xxx'   暂存区放进本地库
git log               查看提交历史
git diff              工作区和暂存区差异
git diff --cached     暂存区和最后一次提交的差异
````
3. 提交历史
````
git log
git log --oneline
````
4. clone
````
git clone git:https:xxx
````
5. 远程操作
````
git remote
git pull

第一次提交
git push -u origin
````

6. 分支管理

创建新分支并切换到该分支：
````
git checkout -b xxx
git switch xxx       (切换分支)
git switch -c xxx
````
查看分支
````
git branch
````
远程分支
````
git branch -r  /  -a（全部本地分支和远程）
````
合并分支
````
git merge xxx
````
删除分支
````
本地 git branch -d xx
删除未合并 git branch -D xx
远程  git push origin --delete xx
````
解决合并冲突
````
git add 'conflict-file'
git commit
````
7. 忽略某些文件
````
touch .gitignore
vim .gitignore
````
````
# 忽略指定文件
config.local.json

# 忽略所有日志文件
*.log

# 忽略目录
node_modules/
build/

# 忽略 .env，但保留示例文件
.env
!.env.example
````
## 远程仓库
1. 添加
```
git remote add origin git@xxx
```

2. 查看
````
git remote
````
3. 提取
````
下载分支和数据   git fetch
合并           git merge
````
4. 推送
````
git push -u origin master
git push origin master
````
5. 删除
````
git remote rm xx
````
# shell

````
#!/bin/bash
echo "hello World!"
````

注释

单行：`#注释内容      `  
多行：
````
:<<!
#xxxxx
#xxxxx
!
````
````
name=张三

echo "$name"    # 输出：张三
echo '$name'    # 输出：$name
````
## 变量
变量的值如果有空格，需要用双引号括起来

1. 局部变量

   变量值的查询
    ````
   $xx
   ${xx}   字符串拼接
    ````
   变量的删除
   ````
   unset xx
   ````
2. 自定义常量（只读变量）  
   `readonly xx`

   自定义全局变量  
   `export var_name1 var_name2=xx var_name3 xxx`


3. 特殊变量
   1. $n
      ````
      接受执行过程中传入的参数
      $0 获取当前文件名称
      $1-$9 获取第一到第九输入参数
      剩下的用${11}
      ````
   2. $#
      ````
      输入参数的个数
      ${#xx}
      ````
   3. `$* $@`  
      获取所有输入参数用于输出所有参数
      ````
      没双引号则功能一样 $1 $2 $3
      “$*” 所有参数拼接成一个字符串，"$1 $2 $3"
      ”$@“ 一组参数列表对象，“$1” "$2" 
      ````
   4. $?  
      用于获取上一个shell命令退出状态码，或者是函数的返回值  
      每一个shell命令的执行都有一个返回值，这个东西用于说明命令执行是否成功  
      `返回0,执行成功，否则失败`
      ````
      
      ````

   5. $$  
      用于获取当前shell环境的进程ID号
      ````
      ps -aux|grep bash
      ````
