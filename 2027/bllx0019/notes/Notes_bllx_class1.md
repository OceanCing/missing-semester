# Linux&Git_Note



## 服务部署流程

开发/个人电脑——>车载NUC（Linux Server）——>ros节点

- 开发电脑和NUC通过SSH/Internet连接
- CPP ToolKit
- Linux Server各项服务开机自启



## Linux Part

### Linux 目录
- / ：根目录
- /home/user ：用户文件
- /usr ：相关库和应用程序
- /dev ： 设备文件（各个硬件part的驱动）

### Shell常用操作命令

| 常用操作 | 对应功能 |
|:-------------|:------------------|
|`mkdir -p`|创建目录，允许补齐父目录|
|`ls` |查看当前文件夹下目录-0 |
|`touch`|创建空文件 / 更新时间|
|`cp`|复制文件|
|`mv`|移动或重命名|
|`rm` |删除文件|
|`cat` / `less` / `bat` |直接查看 / 分页查看 / 类IDE查看 |
|`name="RM"`&`echo "$name"` |会话中保存变量与调用变量 |
|`>` & `>>` |>重写；>>在文尾追加 |
|A `\|` B`|将上一步A指令输出传参传入下一步指令B|
|A `&&` B |if上一条A执行成功else执行下一条B|
### Vim

| 模式       | 进入方式         |
| ---------- | ---------------- |
|普通模式|`Esc` / `Ctrl+[`|
|插入模式|`i a o ...`|
|可视模式|`v V Ctrl+v`|
|命令行模式|`: / ?`|



## Git Part

### 协作流程

团队仓库（online，github）——>fork到个人——>clone到本地仓库——>feature——>修改/add/commit——>提交更新个人fork——>从fork发起团队PR(github)——>review——>Merge

### Git系统结构

- 工作区：workspce，台前修改文件

- 暂存区：将要提交（本地）仓库的内容，内含物为这一个版本修改过的内容       `git add`

- 本地仓库：参考本地文件夹，其中还包含有各个版本的提交与更新记录              `git commit`

- 远程仓库（github）：团队项目托管，类似代码云盘；合并需经过团队成员审核   `git push`

- 分支（branch）：主线外的其他开发线，保证主线分支（branch main）不被轻易修改

### git常用操作指令

| 常用操作 | 对应功能 |
|:-------------|:------------------|
|`git clone <仓库地址>` |下载仓库|
|`git status` |查看当前分支和文件状态 |
|`git diff` |查看尚未`git add`暂存的修改|
|`git log --oneline`|查看提交历史|
|`git switch -c NewBranch/NewType`|创建并切换到功能分支|
|`git add Yourfile.type`|暂存指定文件|
|`git commit -m "Add greeting"`|记录一次本地提交|
|`git push -u origin feature/hello`|推送并建立跟踪关系|
|`git switch main`|回到主分支|
|`git pull`|同步远程更新|

### SOMETHING ABOUT 协同开发规范

- 不要轻易关闭PR，PR提交以后可以修改其内部的一系列内容，出现conflic冲突修改即可；随意关闭PR容易造成版本跟踪紊乱
- commit消息注意英文大写开头带句号，PR标题英文注意大写开头，PR描述可以使用中文 但是要描述清晰你的PR提交了什么东西

