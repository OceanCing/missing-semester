# Linux 介绍

## linux特别的地方
1. 有很多的发行商，对于我们所接触的 **Ubuntu** ，其对ROS的兼容性最好，而且还具有成熟的开发生态。
2. 有非常干净和统一的 **路径** ，相比于其他系统来说，让我觉得能更好上手，而且就是看着非常的清晰呀。
>~   代表家目录
> .    代表当前目录
> ..   代表上一级目录

```cpp
/home/user(ataka)   //用户文件
/etc                //系统配置
/usr                //程序与库
/var                //日志等可变数据
/dev                //设备文件
```

3. 整个系统原生 **依赖** 命令行界面，由于开发的机器人不太能放个屏幕在外并且整个图形化界面让我们操作，用linux开发机器人，能让我们更适应将来的纯命令行开发。
4. 原生自带的 **SSH** 能让我们超级方便的连接上机器人并进行调试。

---

# Shell

## 介绍
Shell是命令行界面的**核心**组件，用户在终端输入指令，Shell将其翻译成机器能懂的底层指令再输入给内核去执行。

## 解释器
在之后写的.sh文件中，我们都要在第一行加上  `#!/bin/bash` ，表示的是这个脚本文件使用的是什么解释器。
```cpp
echo $SHELL          //打印存放默认解释器的环境变量
```

## Shell的基本语法

### 运行脚本
有三种运行脚本的方法
```cpp
1.
bash script.sh          //直接用bash去读目标文件，忽略shebang，所以不需要权限

2.
chmod +x script.sh      //给目标文件加上权限，r/w/x 分别表示读/写/执行 权限
./script.sh             //执行目标文件，解释器从shebang得到，需要先给权限

3.
source script.sh        //不创造子shell，超级容易内存泄漏的运行

```

### 变量的使用

- 定义与使用
```cpp
name="ataka"    //赋值，不用写类型，等号两边千万不要有空格！
echo ${name}      //输出一个变量，$是取值的意思，加大括号是为了隔离

readonly name    //只读
local name       //局部变量
unset name       //删除

now=$(date)      //将可执行命令的结果赋给
```

- 特殊变量
```cpp
位置参数是在打开脚本的时候，在路径后面加上的几个用空格隔开的字符串

$0        //返回脚本文件名
$1~9      //返回第几个参数
$#        //返回参数个数
```

### 输入输出
```cpp
 
//输出
echo "hello"
echo -n "不换行"          //-n不换行
echo -e "a\nb"           //-e解释转义字符

printf "名字: %s, 年龄: %d\n" "ataka" 19   //说是比echo更加严谨

//输入
read name                     //读一行到 name
read -p "请输入名字: " name    //-p可以带提示
read -s -p "密码: " pwd       //-s可以隐藏你的输入
read a b c                   //可以一次性读多个

```

### 引号的区别
```cpp
name="ataka"
echo "hello $name"        //会输出hello ataka
echo 'hello $name'        //会输出hello name  
```
感觉大多数时候都是加双引号更实在

### 条件判断

- if的结构
```cpp
if [ 条件 ]; then
    ...
elif [ 条件 ]; then
    ...
else
    ...
fi
```
重要的是中括号内的两边都需要有**空格**

- 判断比较
```cpp
if [ "$a" -eq "$b" ]; then   //-eq 等于
if [ "$a" -ne "$b" ]; then   //-ne 不等于
if [ "$a" -gt "$b" ]; then   //-gt 大于
if [ "$a" -lt "$b" ]; then   //-lt 小于
if [ "$a" -ge "$b" ]; then   //-ge 大于等于
if [ "$a" -le "$b" ]; then   //-le 小于等于

if [ "$a" = "$b" ]; then     // 相等
if [ "$a" != "$b" ]; then    // 不等
if [ -z "$a" ]; then         //-z  判断是否为空
if [ -n "$a" ]; then         //-b 判断是否非空

if [ "$a" -gt 0 ] && [ "$b" -gt 0 ]; then   //逻辑判断示例一下，以防看不懂
if [ "$a" -gt 0 ] || [ "$b" -gt 0 ]; then   
if [ ! -f file ]; then    

if[[ "$a" -gt 0 && "$b" -gt 0 ]];        //这个双[[方便           
```

- case的使用
```cpp
case "$x" in
    start)  echo "启动";;       //逻辑和之前的一样，只是结构变化好多，很难记
    stop)   echo "停止";;
    *)   echo "未知参数";;
esac
```

### 各种循环
- for循环
```cpp
for ((i=1; i<=5; i++)); do        //总之只记了很像C语言格式的
    echo $i
done

i=1                              //总是就是写一个示例，不然完全忘了
while [ $i -le 5 ]; do
    echo $i
    i=$((i+1))
done

```

### 重定向

| 描述符 | 名称          | 默认去向 | 用途    |
| --- | ----------- | ---- | ----- |
| 0   | stdin 标准输入  | 键盘   | 程序读数据 |
| 1   | stdout 标准输出 | 终端   | 正常输出  |
| 2   | stderr 标准错误 | 终端   | 错误信息  |
```cpp
echo "clear" > test.txt        //覆写,先清空，后写入，所以如果不存在的东西，会清空加报错
echo "new" >>test.txt          //追加，加到文本最后处

wc -l < test.txt               //统计文件的行数-l，单词数-w，字节数-c,字符数-m
ls 报错 2>err.txt              //故意找错一个文件，然后将接收到的2：stderr覆盖到err中

ls > all.txt 2>&1      //两个效果一样，从左到右运行，> xx覆写stdout，2>&1覆写stderr
ls &> all.txt       

ls > /dev/null 2>&1           //丢弃所有输出
```

### 管道

管道只传 **stdout**
```cpp
ls | grep ".sh"          //把前一个命令的输出 流 给后一个命令的输入，筛选
cat test | sort | uniq   //打开test，将文件 流 给sort排序，在 流 给uniq删除重复行
```

### 函数

和之前没有什么太大区别，在调用的时候不用加（）了？
```cpp
test() {
    local name="$1"       
    echo "Hello, $name"
    return 0             
}

test "Alice"

result=$(test "Bob")
echo "$result"

```

### 数组

```cpp
arr=(apple banana cherry)

echo ${arr[0]}          //apple
echo ${arr[@]}          //所有元素
echo ${#arr[@]}         //元素个数 3
echo ${arr[@]:1:2}      //切片：从索引1取2个

arr[3]="date"           //追加/赋值
arr+=("elderberry")     //追加

for ((i = 0; i < 3; i++)); do    //打印arr里的每一个元素
    echo ${arr[i]}
done
```

### 常用小指令

下面是写脚本的时候常用的一些
```cpp
命令的退出状态
if command; then echo "成功"; fi

条件连接
cmd1 && cmd2     //cmd1 成功才执行 cmd2
cmd1 || cmd2     //cmd1 失败才执行 cmd2

默认值
echo ${var:-默认值}      //var 未设置时用默认值返回，但不会给var赋值
echo ${var:=默认值}      //var 未设置时用默认值赋值并返回

字符串操作
str="hello world"
echo ${#str}             //读长度，输出11
echo ${str:0:5}          //截取 hello 输出
echo ${str/world/bash}   //把str 里面的 world 变成 bash

# 一行执行多条
echo a;echo b;echo c     //依次执行，无论面前命令执行是否成功
```

然后是一些经常调用的命令
```cpp
ls/ls -l   //列出目录内容/长格式/块占用
cd         //切换目录
pwd        //显示当前路径

mkdir/rmdir/rm/rm- r  //创造文件夹/删除空文件夹/删除文件/删除此下所有
touch test.txt ; touch tmp/     //创造空文件 ; 更新目录时间戳
cp     //复制
mv a.txt b.txt ;mv a.txt /tmp/  //重命名 ; 移动 ; 或者两者一起

cat ; bat              //大白字输出指向文件 ; 优美的输出指向文件
clear        //清屏清显示
history xx   //列出xx条历史命令
exit 0/1     //退出shell，非0代表异常
alias xx='yy'   //在当前shell整一个别名

find [目录] [条件/测试] [动作]
/*按条件找文件
-name "*.txt"
-type f/d   文件/目录
-size +10M   大于10M*/

grep "Target" test.txt   //在文本里面搜索内容
sort test.txt   //排序，默认按升序、整行、字典序来

sudo   //以管理员身份执行
ps     //视奸进程
kill   //杀死进程！
ping   //后接IP或者域名，判断是否连接上
ssh    //远程登录
```

就算在用的时候卡了，也可以通过`xx --help` 的方式来查看其作用         

---

# Vim

##  介绍

Vim常用于**SSH和无桌面环境中**的配置、代码编辑。

Vim有三种模式，他们的名字和大致作用分别是：
- 命令行模式 ——保存、退出、搜索等
- 正常模式——浏览、宏观上处理文本 
- 编辑模式——添加、修改文本

打开终端，输入`vim xxxx`，即可以正常模式打开或新建文件。

> 需要注意的是，vim对于xxxx不是很严格，无论是相对路径还是绝对路径都能打开对应文件，相对应的，平时要多注意一下路径拼写是否有误，不然会直接新建空文件的。

## 正常模式

```cpp
Esc         //从编辑模式进入正常模式
i/I         //从光标左边进入编辑模式 / 从整行最左边进入
a/A         //从光标右侧进入编辑模式 / 从整行最右边进入
o/O         //新增下一行进入编辑模式 / 新增上一行进入

G/gg        //光标移动到最后一行 / 移动到最顶行
5j          //往下移动 5 行
5k          //往上移动 5 行
w           //移动到下个单词首部
e           //移动到下个单词尾部
b           //移动到上个单词首部

yy          //复制当前行， y 就是复制的意思
yw          //复制单个单词
p           //粘贴到所处行
dd          //删除当前行， d 就是删除的意思
dw          //删除此字符
cw          //删除此字符并进入编辑模式
ci{         //删除括号内所有并进入编辑模式

.           //重复上一次的操作
u           //撤回上一次的操作
ctrl + r    //重做上一次的撤回

ctrl + v    //可视化块，同于鼠标的框选
shift + v   //可视化行

hjkl   //对应着上下左右移动光标
：  //进入命令行模式
```

## 命令行模式

```cpp
：      //进入命令行模式，其实很多时候：直接复合到指令里面去了

ZZ      //在正常模式下直接保存退出
:q      //仅退出
:q!     //不保存退出
:wq     //保存退出

:/xx | n       //搜索xx后定位 / 跳下一个匹配  
:%s/旧/新/g    //直接全局替换，完成后进入正常模式
```

## 配置

如果需要改变vim的一些设置，比如说想要看见行号那么就要到 `user vimrc `里面加东西配置。
```cpp
set number            //显示行号
set relativenumber    //显示相对行号，利于光标移动
set hlsearch          //高亮搜索结果
```



# Git/GitHub

## Git
### 用户和仓库配置

首先我们要设置一个全局用户名和邮箱
```cpp
git config --global user.name "名称"
git config --global user.email "邮箱"
// config  是一个子命令，用于读写Git配置项
// 作用范围 --local/global/system 分别是仓库、用户、系统级
git config --list     //输出所有对于Git的配置
```

然后我们可以弄一个仓库
```cpp
git init              //在当前目录新建一个Git仓库
git clone <url>       //把别人的仓库克隆到此目录下

```

### 常见指令

#### 日常提交

```cpp
git status     //身处一个git处，可以查看此git的文件状态

git add<file>  //将 file 添加到暂存区
git add .      //将当前目录所有更改添加到暂存区
git diff       //查看尚未暂存的修改

git commit -m "说明"   //将暂存区内容提交到本地仓库
git commit --amend -m "新的说明"  //修改最后一次提交，可以改名字追加文件
```

#### 分支管理

```cpp
git branch xx
git branch            //查询分支及所处分支
git branch -v         //同时显示每个分支的最后一次提交
git branch -m yy      //更改所处分支的名字（适用于未提交过的分支）
git branch -d <分支名>  //删除已合并的分支
git merge <分支名>              //将指定分支合并到当前分支
  
git checkout xx yy
//checkout依旧xx的不同有多种功能，yy指 命名，但新版的git里有更易懂的名字

git checkout -b yy             //创造名为yy的新分支
git switch -c yy

git checkout yy                //切换到已有分支yy
git switch yy

git checkout -- yy             //撤销工作区yy文件修改
git restore yy

git checkout <commit> -- yy    //把某文件恢复到某次提交的版本
git restore --source=<commit> yy

```

## GitHub

GitHub就是一个远端仓库，我们通过命令与其交互

```cpp
git remote -v      //查看关联了的远程仓库地址
git remote add origin <url>   //添加远端仓库

git fetch        //只拉取远程更新到本地，但要手动merge合并到当前分支
git pull         //拉取远程更新并自动合并到当前分支

git push origin <分支名>       //将本分支推送到远程仓库
git push -u origin <分支名>    //首次推送并建立跟踪关系,后可直接push
git push origin --delete <分支名>    //删除远程分支

```

## CPP&OpenCV 

[a-Ta-Ka/study-notes: RM学习笔记](https://github.com/a-Ta-Ka/study-notes)