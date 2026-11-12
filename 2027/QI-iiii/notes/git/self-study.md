存档  
git config --global user.name " "  
git config --global user.email " "  
cd ~/  
git innit  
git status (untracted files（工作区 changes not staged for commit/work tree/directory))  
git add (暂存区changes to be committed)  
git commit -m " "（ 提交历史并附带说明）  
git commit 文件名 -m " "（提交至暂存区并提交至本地仓库并附带说明）  
q （退出）

回滚  
git diff  
补充：  
假设修改了 `main.cpp` 和 `test.cpp`。

|命令|查看什么|
|---|---|
|`git diff`|查看所有未暂存的修改|
|`git diff main.cpp`|只查看 `main.cpp` 中未暂存的修改|
|`git diff --staged`|查看所有已暂存、尚未提交的修改|
|`git diff --staged main.cpp`|只查看 `main.cpp` 中已暂存的修改|
|`git diff HEAD`|查看工作区和暂存区相对于最近一次提交的综合差异|

git status（stage（暂存区），unstage（工作区））  
工作区：  
git restore 文件名/.(当前目录所有文件)  
暂存区：  
git restore --staged 文件名/.（移出暂存区）  
git restore 文件名/.  
本地仓库：  
1.只回滚提交中的其中一个文件：  
先查看历史：

```
git log --oneline
```

补充：`git log`  
查看详细提交历史  
`git log --oneline`  
快速浏览提交历史  
`git log --oneline --graph`  
用简洁形式和字符图展示分支、合并关系  
`git log -5 --oneline`  
只查看最近 5 条提交

假设输出如下：

```
b2c3d4e update main and test
a1b2c3d initial commit
```

然后从指定的历史版本恢复文件：

```
git restore --source=a1b2c3d -- main.cpp
```

2.撤销最近一次修改：  
方法一：`git revert`（更适合已推送的提交）

```
git revert HEAD
```

创建一条新的提交，反向撤销指定提交的修改。原提交仍保留在历史中，适合团队协作和已经推送到远程仓库的情况。

方法二：`git reset`（调整当前分支历史）

git reset --soft HEAD~1（head:最新一次提交，head~1：第二新的一次提交，以此类推）

将当前分支指针退回上一次提交，但保留被撤销提交中的修改，并将它们保留在暂存区  
--soft  
暂存区，修改保留  
--mixed  
工作区，修改保留但取消暂存  
--hard  
工作区和暂存区都恢复目标提交状态，相关未提交修改可能丢失

文件忽略：  
touch .gitignore(创建)  
ctrl + o(保存)  
enter(确认文件名)  
ctrl + x(退出)

分支与合并：  
git switch -c 分支名（等同于git branch 分支名 +git switch）  
.......  
git switch master  
git merge 分支名(合并)

删除分支：  
git branch -d 分支名

合并冲突：  
git status  
......(解决冲突)  
git add  
git commit

rebase:  
相当于把分支的流程变为主线流程，使得流程更加干净，线性  
git branch  
git switch 分支名  
git rebase master  
git log --oneline --graph --all

冲突时：  
git status  
.......  
git add  
git rebase --continue

stash:  
git stash(用于存放写了一半的代码)  
git stash pop（取出stash里的代码）

worktree：  
独立的工作区和暂存区（并线开发）

git worktree add 分支名 目录名 master  
......  
git switch master  
git merge 分支名

eg：  
git worktree add -b feature-login ../feature-login main  
-b feature-login  
创建名为 feature-login的新分支  
../feature-login  
新工作区的目录路径  
main  
以main分支当前的提交作为新分支的起点

删除：  
git worktree remove 目录名

