# 学习笔记（01）

## Linux

### Ubuntu

#### Linux的目录从/开始

1. `/home/user` 用户文件
2. `/etc` 系统配置
3. `/usr` 程序与库
4. `/var` 日志等可变数据
5. `/dev` 设备文件

#### Terminal终端

1. `pwd` 显示当前目录完整路径
2. `ls` 列出目录内容
3. `cd 目录`，进入指定目录；`cd ..`，返回上级目录；`cd ~`，返回home目录
4. `mkdir 创建目录`，参数-p补全父目录，例如：`mkdir -p ~/linux_demo`
5. touch 创建空文件/更新时间，例如：`touch hello.txt`
6. cp 复制文件，例如：`cp hello.txt hello-copy.txt`
7. mv 移动或重命名，例如：`mv hello-copy.txt backup.txt`
8. rm 删除文件，例如：`rm backup.txt`
9. cat/less 查看文件内容/分页查看文件内容，例如：`cat hello.txt` ，`less hello.txt`
10. 帮助入口，命令 --help：快速查看选项和用法，例如：`ls --help`。man 命令：打开命令的参考手册，例如：`man ls`

#### Shell的常用组合

1. 变量： `name="RM"`换行`echo "$name"` 保存一个值，再通过变量引用它
2. 重定向： `echo "hello" > hello.txt`  （> 覆盖写入；>> 追加到文件末尾）
3. 管道： `ls -la | grep txt`  将输出交给下一个程序
4. 条件连接：`mkdir -p demo && cd demo` 前一条成功后，再执行下一条

#### VIM在终端编辑文件

#### Git与Github

1. 首次使用 Git：提交作者，`git config --global user.name "Your Name"`和`git config --global user.email "you@example.com"`
2. SSH 密钥：公钥与私钥，`ssh-keygen -t ed25519 -C "you@example.com"`，查看公钥：`cat ~/.ssh/id_ed25519.pub`；公钥.pub `id_ed25519.pub`，私钥 `id_ed25519`
3. 把 SSH 公钥添加到 GitHub，Settings->SSH and GPG keys->New SSH key，选择 Authentication Key，填写名称，粘贴完整公钥，`ssh -T git@github.com`
4. `git clone <仓库地址>` 下载仓库及提交历史
5. `git status` 查看当前分支和文件状态
6. `git diff` 查看尚未暂存的修改
7. `git log -- oneline` 用简短格式查看提交历史
8. **团队 PR 工作流**：*团队仓库->个人 Fork->`git clone`->修改，`git add` 、`git commit`->`git push`->个人 Fork 接收提交->发起 PR->Review 评审->Merge 到团队仓库*
