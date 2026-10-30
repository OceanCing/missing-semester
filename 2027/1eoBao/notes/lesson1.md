# 27赛季控制视觉招新培训第一课

## Linux 基础
- Linux 是内核，Ubuntu 是发行版；团队使用 Ubuntu（ROS 兼容、部署一致）
- 开发机通过 SSH 连接机器人主机，程序可在无桌面环境运行
- 目录从 / 开始；路径区分大小写；~ 家目录、. 当前、.. 上级
- 流程：终端输入 → Shell 解析 → 程序执行

## Shell 常用
- 结构：命令 [选项] [参数]，如 `ls -la ~/projects`
- `pwd` / `ls` / `cd 目录` / `cd ..` / `cd ~`
- `mkdir -p`、`touch`、`cp`、`mv`、`rm`（不进回收站）、`cat`/`less`
- 变量：`name="RM"; echo "$name"`
- 重定向 `>` 覆盖、`>>` 追加；管道 `|`；`&&` 条件连接
- 帮助：`--help`、`man`、Tab 补全、↑ 历史

## Vim
- 普通模式（`i` 进插入）/ 插入模式（Esc 返回）/ 命令行（`:`）
- `hjkl` 移动、`dd` 删行、`yy`/`p` 复制粘贴、`u` 撤销、`/word` 搜索
- `:w` 保存、`:q` 退出、`:wq` 保存退出、`:q!` 放弃退出

## Git & GitHub
- 四区：工作区 →`add`→ 暂存区 →`commit`→ 本地仓库 →`push`→ 远程
- 配置作者：`git config --global user.name/user.email`
- SSH：`ssh-keygen -t ed25519`，公钥 `.pub` 添加到 GitHub，私钥绝不上传
- 查看：`clone`、`status`、`diff`、`log --oneline`
- 提交：`switch -c feature/xxx`、`add`、`commit -m`、`push -u origin`、`pull`
- 团队流程：Fork → Clone → 功能分支 → Commit → Push → PR → Review → Merge

# git 状态查看
- git log命令显示从最近到最远的提交日志，看得眼花缭乱的，可以试试加上--pretty=oneline参数


- git status

# 版本提交
- 第一步，用命令git add告诉Git，把文件添加到仓库
- 第二步，用命令git commit -m告诉Git，把文件提交到仓库
- -m后面输入的是本次提交的说明，可以输入任意内容

# 版本回退
- 把当前版本append GPL回退到上一个版本add distributed，就可以使用git reset命令：

``` 
$ git reset --hard HEAD^  
HEAD is now at e475afc add distributed 
```
- hard参数有啥意义？--hard会回退到上个版本的已提交状态，而--soft会回退到上个版本的未提交状态，--mixed会回退到上个版本已添加但未提交的状态
- git checkout -- file可以丢弃工作区的修改,--很重要，没有--，就变成了“切换到另一个分支”的命令
# git 状态查看
- git log命令显示从最近到最远的提交日志，看得眼花缭乱的，可以试试加上--pretty=oneline参数


- git status

# 版本提交
- 第一步，用命令git add告诉Git，把文件添加到仓库
- 第二步，用命令git commit -m告诉Git，把文件提交到仓库
- -m后面输入的是本次提交的说明，可以输入任意内容

# 版本回退
- 把当前版本append GPL回退到上一个版本add distributed，就可以使用git reset命令：

``` 
$ git reset --hard HEAD^  
HEAD is now at e475afc add distributed 
```
- hard参数有啥意义？--hard会回退到上个版本的已提交状态，而--soft会回退到上个版本的未提交状态，--mixed会回退到上个版本已添加但未提交的状态
- git checkout -- file可以丢弃工作区的修改,--很重要，没有--，就变成了“切换到另一个分支”的命令

# PR（Pull Request，拉取请求） 的作用。

一句话总结：PR 就是你向项目所有者（老师/原仓库）提交代码修改的“申请信”。

在你的作业场景中，PR 的作用主要体现在以下几个方面：

1. 核心作用：请求合并（交作业）
你不能直接修改老师的仓库（OceanCing/missing-semester）。你只能在自己的仓库（Fork 出来的）里完成作业。
PR 就是告诉老师：“我在我的仓库里写完了作业，请求你把我写的这部分内容，合并到你的主仓库里。” 这是你最终“交作业”的正式动作。


# ssh命令
- git remote set-url origin git@github.com:1eoBao/missing-semester.git    

# push
- git push -u origin master 