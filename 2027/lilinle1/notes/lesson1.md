# linux系统基础
```
# 常见命令
ls pwd cd
touch #创建文件 && mkdir #创建文件夹
cat #查看文件
# 进阶
name="RM"
echo"$name" #变量
echo"hi">hello.txt #重定向
ls -la | grep txt #管道(过滤)

```
# vim
Normal 模式（按 Esc 回到这里！最常用）

-  h  左， j  下， k  上， l  右
-  w  跳到下一个单词开头
-  b  跳到上一个单词开头
-  0  行首； $  行尾
-  gg  文件最顶部； G  文件最底部； 50G  → 跳到第50行
 
复制 / 删除 / 粘贴
 
-  dd ：删除一整行（同时存到剪贴板）
-  yy ：复制一整行
-  p ：在光标下面粘贴； P  在光标上面粘贴
-  x ：删除光标当前这一个字符
 
撤销、重做
 
-  u  撤销
-  Ctrl + r  重做
 
查找
 
-  /关键词  向下搜索；按  n  下一个； N  上一个
 
保存 & 退出（必须在Normal模式！）
 
-  :w  保存
-  :q  退出（没改动才行）
-  :wq  保存+退出
-  :q!  不保存强制退出（就是你之前问过的！）
 
 
 
 Insert 模式（编辑文字， i 进入， Esc 回Normal）
 
-  i ：在光标前面开始输入
-  a ：在光标后面开始输入
-  o ：新开下面一行并进入输入（很常用！不用回车换行）
-  O ：新开上面一行并进入输入
 
 
🔹 Visual 模式（选中文字：Normal里按  v ）
 
-  v ：自由选文字；移动hjkl扩大选区
-  V ：按行选（一选就是整行，适合复制大块代码）
- 选好后： y 复制， d 删除，之后用 p 粘贴
# git相关
## 一、SSH公私密钥创建

1. **生成密钥

```
ssh-keygen -t ed25519 -C "you@example.com"
```
2. **查看公钥内容**

```
cat ~/.ssh/id_ed25519.pub
```

## 二、Git 常用命令
### 获取与查看类

| 命令                  | 作用           |
| ------------------- | ------------ |
| `git clone <仓库地址>`  | 下载仓库以及全部提交历史 |
| `git status`        | 查看当前分支与文件状态  |
| `git diff`          | 查看还没有暂存的文件修改 |
| `git log --oneline` | 简短格式查看提交历史   |
### Git 常用命令：分支与提交
```
git switch -c feature/hello #创建并切换到功能分支
git add hello.txt #暂存指定文件
git commit -m "Add test" #记录一次本地提交
git push -u origin featurehello #推送并建立跟踪关系
git switch main #切回主分支
git pull # 获取并整合当前分支的远程更新
```




