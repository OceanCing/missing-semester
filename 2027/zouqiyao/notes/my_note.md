## vim: 应用
    :q 退出 :q!强制退出（不保存）
    :wq 保存后退出
	/name  向下查找name
	?name 向上查找name
	按 n 键上方下一个name所在地
	shift + n 下方下一个 name 所在地
	yy 复制这一行的内容
	3yy复制本行与下两行的内容
	p在光标后换行粘贴
	dd 删除本行
        dw 删除这个当前单词和后面空格
	:1,3s/name1/name2/ 在1到3行找到行内第一个name1 换成 name2
	:1,3s/name1/name2/g 在1到3行找到所有name1 换成 name2
	按esc 退出 插入模式
	.vimrc 配置vim 
	:set number 显示所有行数
	:set nonumber 不显示

## 终端常用命令:
  ###
	ls 列出目录下的内容
	ls -a 列出隐藏的文件 (.name) 文件都是隐藏的
	ls -l 列出详细信息 
	ls -h 显示文件的大小
	ls -t 按照修改时间排序
	ls -r 逆序显示
	ls -lha 详细 显示文件大小的 打印所有文件 (参数可以组合)
   ###
	链接： 软连接(类似于快捷方式)  硬连接（类似于拷贝）
	ln -s name1 name2  生成一个软链接文件name2 指向 name1(name2 指向name1)
	ln name1 name2 有点像引用 name1 和 name2 同时指向同一个i节点(也就是文件真正存在的地方)
	修改name2 的同时name1 也会改 (他们都指向同一个i节点)
	软连接 可以指向目录或者文件  硬连接只能指向文件
   ###
	文件权限 -rwx-rw-rwx  r(read) w(write) x(可执行) 第一个‘-’不看三个一组  分别为user(文件所有者)  group（同组用户） other(其他用户)  的权限
	chmod +x name  给文件name 的other 加一个x权限 
	chmod -x name  给文件name 的other 去除一个x权限
	chmod u+x name 给文件name 的user 加一个x权限 
	chmod g+x
	chmod o+x 一样的
	chmod ou+x  o u g 可以一起用
	数字表示权限 r(4) w(2) x(1) 三个一组组成一个 0～7的数字
	chmod 776 name  name的权限就是   -rwxrwxrw-
	chmod 744 name  name的权限就是   -rwx-w--w- 
   ###
	touch 创建新文件 或 更新文件修改时间
	touch name  文件不保存在 新建
	touch hello.txt 碰一下 hello.txt 更新hello.txt的修改时间为现在
   ###	
	echo 用来输出内容
	echo "hello" 在终端里 打印 hello
	echo "hello" > hello.txt 将文件里的内容换成 "hello" （>重定向符号）
   ### 
	pwd 显示当前所在的位置
	cd bin 跳转到目录bin
	cd ..  跳转到父目录
	cd ../.. 上一个目录的 上一个
   ###	
	目录是树状结构的   根目录为 root -> home -> yao 这里是终端一般的入口
   ###
	cp file2.txt file3.txt 将file2.txt 复制一份 叫 file3.txt
   ###	
	mv file3.txt /home/bin/   移动file3.txt 到bin目录
	mv file3.txt file4.txt  将file3.txt 移动到当前目录 并且重命名为file4.txt 效果就是重名命
	mv file3.txt /home/bin/ file4.txt 将file3.txt 移动到bin目录 并重命名为file4.txt
   ###
	mkdir(make directory) 
	mkdir Folder 创建一个目录
	mkdir -p Folder1/Folder2/Folder3 创建Folder1->Folder2->Folder3 两两为父子关系
   ###
	cp -r 递归复制
	cp -r Folder Folder_copy  将会递归Folder 将它复制给 Folder_copy 
   ###	
	tree Floder 以数状图显示
   ### 	
	rmdir(rm directory)
	rmdir Floder 但是如果这个目录非空 会报错
   ###	
	rm -r Floder_copy 用递归删除 Folder_copy 里的所有文件
	Dangerous!!!  rm 的文件是找不回来的 

## GIT
  ###
	git config --global user.name ""  初始化用户名
	git config --global user.email "" 初始化email
	git config --list 查看配置  
###
	git init  将当前文件夹目录初始化为仓库
	注意！：.git文件保存着所有版本快照..不要删除
###
	git 组成

* git 仓库有三个区 工作区 暂存区 仓库
* 工作区： 修改文件内容
* 暂存区： 暂存已经修改好的文件
* 仓库： 给管理的文件拍快照 生成一个新的版本
###
	git add. 将所有文件上传至暂存区
	git add name1.txt 将name1.txt上传到暂存区
###
	git commit -m "Discribe" 提交一次版本（快照），并描述修改的内容
	git commit 也可以 弹出vim 在第一行写描述 再:wq就好
###
	git status 查看仓库状态（三个区分别状态）
	git log 查看提交历史
	git log --online 用简短的方式查看历史提交
	git ls-files -c
	git ls-files --catched 列出所有被git追踪的文件
						在一次提交后，也可以理解为，这个版本快照里有什么内容
	git diff 检查工作区 和 暂存区文件的区别 (如果是第二次提交以后，版本一开始的暂存区存放着上一个版本年的所有“追踪文件”) 
###
	git reset --soft 退回到上一个版本，保留上一个版本的暂存区和工作区 (比如你在上一个版本新建了一个文件并且add了，reset --soft之后 你的工作区还有这个文件，暂存区也有)

	git reset --mixed  退回到上一个版本，保留上一个版本的工作区 (比如你在上一个版本新建了一个文件并且add了，reset --soft之后 你的工作区还有这个文件,但是暂存区被清,不知道那些工作区文件被修改，暂存区也没有新建的文件，也就没有被追踪)

	git reset --hard 退回到上一个版本,都不保留(上一个版本作的无论是工作区还是暂存区的修改都没有了) 完全是变成的裸体的上一个版本
###
	分支的意义:
* 将master里面的东西“拷贝一份”出来，成为一个分支
* 对分支进行修改不会对master 有任何影响 ，完全独立，master 不知道它干了啥
* 比如一个游戏 有人要写移动的程序 有人写环境的渲染 就分别创建一个分支 在master的基础上两人分别添加内容 最后合并到master 分支里 master的环境渲染 和 人物的移动 就搞定了

###
	git branch branch_name  新建一个名字为branch_name 的分支
	git branch 查看当前仓库所有分支
	git switch branch_name 跳转到branch_name 分支上
###
	合并分支
* 将分支上的修改内容 和 master 合并 产生一个新版本
* 会出现一个问题， 如果两个分支都修改了master的同一个文件而且修改的内容不同 就产生了冲突
### 
	git merge branch_name  branch_name 是要被合并的分支
	注意： 你要在将合并 branch_name 的分支下写这一行代码
	一般是 master....
###
	分支在合并之后不会消失
###
	git branch -d branch_name 删除已经合并的分支
	注意：你也要在合并了的分支下写这一行代码
	git branch -D branch_name 删除未合并分支
###
	git clone <> 将远程仓库复制一份到本地
	git push 将本地的新增的commit 推送到远程仓库
	git push -u origin branch_name 将本地分支的上游设置为origin
###
	git checkout master  将游离指针HEAD指向master分支
	git checkout -- 文件名  将工作区的文件变成原来没有修改过的样子
## SHELL 脚本
### shell 是啥
* 套在内核外面的壳 接受终端命令 丢给解释器 解释器 丢给内核执行
* 解释器有 bash zsh 等
* 一个shell 脚本一般以.sh 结尾

### 一个shell 脚本
```
#!/bin/bash    （#!是告诉shell 用那个解释器  /bin/bash 是解释器的路径）


echo "Hello" //命令

```
### 终端 与 shell 的关系
* 终端是负责接受键盘输入 和 将命令丢给shell  的  套在外面的壳
* 终端里面 跑着一个 交互式的 shell 
* 每新建一个终端 就会 有一个新的shell 跑在终端里面

### 脚本 与 shell 的关系
* 运行一个脚本 就会在脚本里面 跑一个shell 
* 当然是非交互式的shell
* 脚本结束后 shell 也就结束了

### .bash_history 文件
* 只记录 交互式 shell 的历史命令 
```
终端 A 内存 history
    ls
    cd project
        │
        │ exit 时写入
        ↓
   ~/.bash_history

终端 B 内存 history
    git status
    npm run dev
        │
        │ exit 时写入
        ↓
   ~/.bash_history
   ```
* 在 交互式 shell 结束后 写入 bash_history
* 在脚本里面运行的 命令不会被写入 bash_history

### 脚本读取 .bash_history
```
#!/bin/bash
echo "接下来是终端10条历史命令"
tail -n 10 ~/.bash_history

```

### “tail -n 10 ~/.bash_history“ 解释
1. tail -> 显示文件末尾
2. -n 10 -> number 10 显示10 行
3. ～ ->表示用户的家目录
4. / -> 路径分隔符
5. .bash_history -> 一个文件 
6. ~/.bash_history -> 家目录下的 .bash_history 文件
