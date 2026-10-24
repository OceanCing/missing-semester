
## 一.Linux
### 1.Linux目录和路径
～是家目录  .是当前目录  ..是上一级目录
| /home/user | 用户文件 |
| /etc | 系统配置 |
| /usr | 程序与库 |
| /var | 日志等可变数据 |
| /dev | 设备文件 |
绝对路径：从/开始  相对路径：从当前位置开始
**路径的大小写敏感**

## 二.Shell
### 1.shell命令结构
命令 [选项][参数]
命令：要做什么  选项：怎么去做(eg：-a是显示隐藏内容)  参数：操作的对象

## 文件目录操作
```shell
ls     #查看
cd ~/dir 

cd ..   #退回上级
cp file file_cp_name
mv na1 na2       #重命名 或移动
rm   # 删
cat    # 查看
less   # 分页查看

touch   #创建空文件或更新文件时间戳
```
#### 组合
```
name= "aa"    #创建量
echo "$name"  #显示量
|    #管道
&&    #条件
```

#### 概念
###### 文件时间
Linux 文件有三种时间：

| 时间        | 含义              |
| --------- | --------------- |
| **atime** | 最后访问时间 (access) |
| **mtime** | 最后修改时间 (modify) |
| **ctime** | 状态改变时间 (change) |
```shell
touch existing.txt           # 把 atime 和 mtime 都更新为当前时间
```

#### 限定
```
-n num # 数量
-a    #一般的全部
```

#### 其他
按方向键上下选择历史命令（~/.bash_history存放)


## 三.Vim
#### 模式
| 模式        | 作用            | 如何进入                         | 如何退出             |
| --------- | ------------- | ---------------------------- | ---------------- |
| **普通模式**  | 移动光标、删除、复制、粘贴 | 启动默认；或按 `Esc`                | —                |
| **插入模式**  | 输入文本          | 按 `i`（光标前）、`a`（光标后）、`o`（下一行） | 按 `Esc`          |
| **命令行模式** | 保存、退出、替换      | 普通模式按 `:`                    | 执行后自动返回，或按 `Esc` |
#### 基础模式
- :w    保存
- :q     退出
- :wq  保持并退出

- `h` `j` `k` `l`：分别对应左、下、上、右
- `gg`：跳到文件开头
- `G`：跳到文件末尾
- `0`：跳到行首；`$`：跳到行尾
- `w`：跳到下一个单词开头；`b`：跳到上一个单词开头
#### 编辑
- `x`：删除光标所在字符
- `dd`：删除整行（也可当作剪切）
- `yy`：复制整行
- `p`：粘贴到光标后
- `u`：撤销；`Ctrl + r`：重做

## 四.Git和Github

#### Git的四个位置
**工作区**：正在编辑的文件

**暂存区**：工作区git add之后的内容清单

**本地仓库**：暂存区git commit之后的提交历史

**Github远程仓库**：本地仓库git push之后保存的提交历史

### Git常用命令
```sh


cd my-folder
git init
git add .
git commit -m "init"
git remote add origin https://github.com/用户名/仓库名.git
git branch -M main   //改分支名为main
git push -u origin main
```

查看暂存区文件名
```
git diff --staged --name-only  
```

#### 分支
```
git switch branch_name
git switch -c branch_name            #顺便创建了
```

#### 远程仓库
#### 添加网址
```
git remote add orgin http
```
#### 查看
```
git remote -v
```
### 4.Github的PR工作流

团队仓库 --> 个人Fork--> 在本地电脑进行clone--> 创建分支--> 修改/add/commit--> Push--> 个人Fork接受提交 --> 发起PR --> Review评审 --> Merge到团队仓库

*分支：main保持稳定，分支相当于副本，在里面进行修改评审后再合并*