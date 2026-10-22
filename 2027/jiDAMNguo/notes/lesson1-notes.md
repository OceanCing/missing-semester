# Ubuntu 学习笔记

> 培训课第一课：Linux、Vim && Shell、Git && GitHub

## 目录

- [文件系统](#文件系统)
- [命令行基础](#命令行基础)
- [文件与权限](#文件与权限)
- [查看文件内容](#查看文件内容)
- [重定向与管道](#重定向与管道)
- [SSH 远程连接](#ssh-远程连接)
- [Vim 编辑器](#vim-编辑器)
- [终端常用快捷键](#终端常用快捷键)
- [apt 软件包管理](#apt-软件包管理)
- [Git 版本控制](#git-版本控制)
- [GitHub](#github)

## 文件系统

Linux 使用统一的目录树结构，所有文件和目录都挂在这棵树上。

### 用户目录

1. Linux 支持多个用户，每个用户有一个自己的目录，例如 `/home/ghx`。
2. 普通用户通常只能操作自己的用户目录；`root` 用户可以操作任意文件和目录。

### 创建目录和文件

1. **目录操作**：可以进入主目录，右键新建文件夹，也可以右键文件夹进行删除或重命名。
2. **文件操作**：使用文本编辑器创建和编辑文件。

## 命令行基础

### 常用命令

1. **`ls`**（list）：列出目录内容。

    ```bash
    ls /home/XXX
    ls -l /home/XXX        # 详细查看
    ```

2. **`pwd`**（print working directory）：显示当前工作目录。

    ```bash
    pwd
    ```

3. **`cd`**（change directory）：切换目录。

    ```bash
    cd /home/XXX/123/
    cd ~/Downloads
    ```

4. **`mkdir`**（make directory）：创建目录。

    ```bash
    mkdir abc
    mkdir -p abc/123/test   # 逐层创建目录
    ```

5. **`rmdir`**（remove directory）：删除空目录。

    ```bash
    rmdir abc
    ```

    目录非空时删除失败。

6. **`rm`**（remove）：删除文件或目录。

    ```bash
    rm -rf abc
    ```

    - `-r`：递归删除
    - `-f`：强制删除

7. **`cp`**（copy）：复制文件或目录。

    ```bash
    cp -rf example example2
    ```

8. **`mv`**（move）：移动文件或目录，也可以用来重命名。

    ```bash
    move hello world
    ```

9. **`tar`**（tape archive）：打包文件。

    ```bash
    tar -cvf example.tar example
    ```

    - `-c`：创建档案
    - `-v`：显示详情
    - `-f`：指定文件名

### 特殊目录

- `~`：当前用户的主目录。
- `.`：当前目录。
- `..`：上一级目录。

例如：

```bash
cd ~/Downloads
```

## 文件与权限

1. Linux 是多用户系统，每个文件都有所有者、所属组和权限。
2. 使用 `ls -l` 查看权限：

    ```text
    -rwxr-xr-x 1 ghx ghx 1234 10月 22 10:00 hello.sh
    ```

    - 第 1 位：`-` 表示普通文件，`d` 表示目录，`l` 表示链接。
    - 后面 9 位：所有者、所属组、其他人的 `rwx` 权限。
    - `r` 读，`w` 写，`x` 执行。

3. **`chmod`**：修改权限。

    ```bash
    chmod +x hello.sh       # 给文件加执行权限
    chmod 755 hello.sh      # 所有者 rwx，其他用户 r-x
    chmod 644 hello.txt     # 所有者 rw-，其他用户 r--
    ```

4. **`sudo`**：以管理员权限执行命令。

    ```bash
    sudo apt update
    ```

    普通用户只在自己的家目录里有写权限，系统目录需要 `sudo`。

5. **`chown`**：修改文件所有者，一般需要 `sudo`。

    ```bash
    sudo chown ghx:ghx hello.txt
    ```

## 查看文件内容

1. **`cat`**：一次性输出文件内容。

    ```bash
    cat hello.txt
    ```

2. **`less`**：分页查看，适合大文件。

    ```bash
    less hello.txt
    ```

    - 空格：下一页
    - `b`：上一页
    - `q`：退出
    - `/word`：搜索

3. **`head` / `tail`**：查看文件开头或结尾。

    ```bash
    head -n 20 hello.txt
    tail -n 20 hello.txt
    tail -f log.txt          # 实时查看日志
    ```

4. **`grep`**：按关键词搜索。

    ```bash
    grep "hello" hello.txt
    grep -n "hello" hello.txt     # 显示行号
    grep -r "hello" ./            # 递归搜索目录
    ```

5. **`wc`**：统计行数、字数等。

    ```bash
    wc -l hello.txt
    ```

## 重定向与管道

1. `>` 覆盖写入，`>>` 追加写入。

    ```bash
    echo "hello" > hello.txt
    echo "world" >> hello.txt
    ```

2. `|` 管道：把前一个命令的输出交给后一个命令。

    ```bash
    ls -la | grep txt
    ps aux | grep python
    ```

3. `&&`：前一条命令成功后，再执行下一条。

    ```bash
    mkdir -p demo && cd demo
    ```

4. 常见组合：

    ```bash
    cat hello.txt | wc -l
    ```

## SSH 远程连接

### SSH 是什么

1. SSH（Secure Shell）是一种加密的远程登录协议，默认端口为 22。
2. 常见用途：登录远程主机、远程开发、传输文件、访问 GitHub。
3. 基本格式：

    ```bash
    ssh 用户名@主机地址
    ssh ubuntu@192.168.1.100
    ssh -p 2222 ubuntu@192.168.1.100    # 指定端口
    ```

    - 第一次连接会提示核对主机指纹，确认无误后输入 `yes`。
    - 退出远程登录：`exit` 或 `Ctrl+D`。

### 使用密钥登录

1. 生成密钥对（公钥 + 私钥）：

    ```bash
    ssh-keygen -t ed25519 -C "you@example.com"
    ```

    - 一路回车即可，默认保存在 `~/.ssh/`。
    - `id_ed25519` 是私钥，`id_ed25519.pub` 是公钥。

2. 查看公钥：

    ```bash
    cat ~/.ssh/id_ed25519.pub
    ```

3. 把公钥复制到远程主机：

    ```bash
    ssh-copy-id ubuntu@192.168.1.100
    ```

    也可以手动把公钥内容追加到远程主机的 `~/.ssh/authorized_keys`。

4. 之后直接使用 `ssh` 即可免密登录：

    ```bash
    ssh ubuntu@192.168.1.100
    ```

5. 私钥安全：

    - 私钥只留在自己的电脑上，不上传、不截图、不分享。
    - 权限必须严格设置：

    ```bash
    chmod 700 ~/.ssh
    chmod 600 ~/.ssh/id_ed25519
    chmod 644 ~/.ssh/id_ed25519.pub
    ```

### 配置免密别名

1. 编辑 `~/.ssh/config`：

    ```bash
    vim ~/.ssh/config
    ```

2. 写入主机信息：

    ```text
    Host robot
        HostName 192.168.1.100
        User ubuntu
        Port 22
        IdentityFile ~/.ssh/id_ed25519
    ```

3. 之后直接登录：

    ```bash
    ssh robot
    ```

### 用 SSH 访问 GitHub

1. 生成密钥（方法同上），把公钥完整粘贴到 GitHub：
   `Settings → SSH and GPG keys → New SSH key → Authentication Key`
2. 测试连接：

    ```bash
    ssh -T git@github.com
    ```

    出现 `Hi 用户名! You've successfully authenticated` 即表示成功。

3. 克隆仓库时使用 SSH 地址：

    ```bash
    git clone git@github.com:用户名/仓库名.git
    ```

### scp 传输文件

1. 本地传到远程：

    ```bash
    scp hello.txt ubuntu@192.168.1.100:~/
    scp -r mydir ubuntu@192.168.1.100:~/   # 传整个目录
    ```

2. 远程下载到本地：

    ```bash
    scp ubuntu@192.168.1.100:~/result.txt ./
    ```

## Vim 编辑器

### 三种模式

1. **普通模式**：默认模式，用于移动光标、删除、复制等操作。
2. **插入模式**：输入文本，按 `i` 等键进入，按 `Esc` 返回普通模式。
3. **命令行模式**：在普通模式下按 `:` 进入，用于保存、退出、替换等操作。

> 记住：`Esc` 永远回到普通模式。

### 打开与退出

1. 打开文件：

    ```bash
    vim hello.txt
    ```

2. 保存与退出（命令行模式）：

    | 命令 | 作用 |
    | --- | --- |
    | `:w` | 保存 |
    | `:q` | 退出 |
    | `:wq` | 保存并退出 |
    | `:q!` | 放弃修改，强制退出 |
    | `:x` | 保存并退出（同 `:wq`） |

3. 打开多个文件：

    ```bash
    vim a.txt b.txt
    ```

    - `:n`：下一个文件
    - `:N`：上一个文件

### 进入插入模式

| 按键 | 作用 |
| --- | --- |
| `i` | 在光标前插入 |
| `a` | 在光标后插入 |
| `I` | 行首插入 |
| `A` | 行尾插入 |
| `o` | 在下一行插入 |
| `O` | 在上一行插入 |

按 `Esc` 返回普通模式。

### 光标移动（普通模式）

| 按键 | 作用 |
| --- | --- |
| `h` `j` `k` `l` | 左、下、上、右 |
| `w` | 下一个单词开头 |
| `b` | 上一个单词开头 |
| `e` | 当前单词末尾 |
| `0` | 行首 |
| `$` | 行尾 |
| `gg` | 文件第一行 |
| `G` | 文件最后一行 |
| `:n` | 跳到第 n 行 |
| `Ctrl+f` | 下一页 |
| `Ctrl+b` | 上一页 |

### 编辑操作（普通模式）

| 按键 | 作用 |
| --- | --- |
| `x` | 删除光标处字符 |
| `dd` | 删除当前行 |
| `ndd` | 删除 n 行 |
| `dw` | 删除一个单词 |
| `yy` | 复制当前行 |
| `nyy` | 复制 n 行 |
| `p` | 粘贴到光标后 |
| `P` | 粘贴到光标前 |
| `u` | 撤销 |
| `Ctrl+r` | 重做 |
| `r` | 替换一个字符 |
| `cw` | 修改一个单词 |
| `>>` | 当前行右缩进 |
| `<<` | 当前行左缩进 |

### 搜索与替换

1. 搜索：

    | 命令 | 作用 |
    | --- | --- |
    | `/word` | 向下搜索 word |
    | `?word` | 向上搜索 word |
    | `n` | 下一个匹配 |
    | `N` | 上一个匹配 |

2. 替换：

    | 命令 | 作用 |
    | --- | --- |
    | `:s/old/new/` | 替换当前行第一个匹配 |
    | `:s/old/new/g` | 替换当前行全部匹配 |
    | `:%s/old/new/g` | 替换全文 |
    | `:%s/old/new/gc` | 替换全文并逐个确认 |

### 可视模式与分屏

1. 可视模式：

    | 按键 | 作用 |
    | --- | --- |
    | `v` | 字符可视 |
    | `V` | 行可视 |
    | `Ctrl+v` | 块可视 |

    选中后按 `d` 删除，按 `y` 复制。

2. 分屏：

    | 命令 | 作用 |
    | --- | --- |
    | `:sp` | 水平分屏 |
    | `:vsp` | 垂直分屏 |
    | `Ctrl+w w` | 切换窗口 |
    | `:q` | 关闭当前窗口 |

### 常用配置 ~/.vimrc

1. 编辑配置文件：

    ```bash
    vim ~/.vimrc
    ```

2. 常用设置：

    ```vim
    syntax on            " 语法高亮
    set number           " 显示行号
    set tabstop=4        " 制表符宽度为 4
    set shiftwidth=4     " 自动缩进宽度为 4
    set expandtab        " 用空格代替 Tab
    set mouse=a          " 允许鼠标
    set cursorline       " 高亮当前行
    ```

3. 保存后重新打开 Vim 生效。

### Vim 练习流程

1. 打开练习文件：

    ```bash
    vim hello.txt
    ```

2. 按 `i` 输入一行文字。
3. 按 `Esc` 回到普通模式。
4. 输入 `:wq` 保存退出。
5. 再次打开文件，练习 `dd`、`u`、`/word` 等操作。

## 终端常用快捷键

| 快捷键 | 作用 |
| --- | --- |
| `Ctrl+C` | 终止当前命令 |
| `Ctrl+D` | 结束输入 / 退出终端 |
| `Ctrl+L` | 清屏 |
| `Ctrl+R` | 搜索历史命令 |
| `Ctrl+A` / `Ctrl+E` | 光标移到行首 / 行尾 |
| `Tab` | 补全命令或路径 |
| `↑` / `↓` | 调出历史命令 |

## apt 软件包管理

1. 更新软件源：

    ```bash
    sudo apt update
    ```

2. 安装软件：

    ```bash
    sudo apt install vim git
    ```

3. 卸载软件：

    ```bash
    sudo apt remove 软件名
    ```

4. 升级已安装的软件：

    ```bash
    sudo apt upgrade
    ```

5. 查看软件信息：

    ```bash
    apt show 软件名
    which 命令名        # 查看命令所在路径
    ```

## Git 版本控制

### Git 是什么

1. Git 是一个分布式版本控制系统，用来记录文件的修改历史，方便回退和多人协作。
2. 每个参与者的电脑上都有一份完整的仓库，不依赖中央服务器。
3. **工作区**：当前正在编辑的文件。
4. **暂存区**：执行 `git add` 之后、等待提交的内容。
5. **本地仓库**：执行 `git commit` 之后，保存在 `.git` 目录里的历史记录。
6. **远程仓库**：托管在 GitHub 等平台上的仓库。

### 安装与配置

1. 安装 Git：

    ```bash
    sudo apt install git
    ```

2. 查看版本：

    ```bash
    git --version
    ```

3. 设置用户名和邮箱。不设置的话，`commit` 会失败或者作者信息不对：

    ```bash
    git config --global user.name "your-name"
    git config --global user.email "you@example.com"
    ```

4. 查看全部配置：

    ```bash
    git config --global --list
    ```

### 创建和克隆仓库

1. 在当前目录初始化一个新仓库：

    ```bash
    git init
    ```

2. 克隆远程仓库到本地：

    ```bash
    git clone https://github.com/user/repo.git
    ```

3. 克隆时自己指定文件夹名：

    ```bash
    git clone https://github.com/user/repo.git myrepo
    ```

### 基本操作

1. 查看当前状态，例如哪些文件被修改、哪些已经 `add`：

    ```bash
    git status
    ```

2. 把文件加入暂存区：

    ```bash
    git add hello.txt
    git add .
    ```

3. 把暂存区的内容提交到本地仓库：

    ```bash
    git commit -m "Add hello note."
    ```

4. 只提交已经被 Git 跟踪过的文件时，可以省略 `add`：

    ```bash
    git commit -am "Update hello note."
    ```

5. 查看提交历史：

    ```bash
    git log
    git log --oneline --graph --all
    ```

6. 查看工作区里还没有 `add` 的修改：

    ```bash
    git diff
    ```

7. 查看已经 `add`、但还没有 `commit` 的修改：

    ```bash
    git diff --staged
    ```

### 分支与合并

1. 查看本地分支，前面带 `*` 的是当前分支：

    ```bash
    git branch
    ```

2. 创建分支：

    ```bash
    git branch dev
    ```

3. 切换分支：

    ```bash
    git switch dev
    git checkout dev
    ```

4. 创建并切换分支：

    ```bash
    git switch -c dev
    git checkout -b dev
    ```

5. 合并分支（先切回目标分支再 `merge`）：

    ```bash
    git switch main
    git merge dev
    ```

6. 删除已经合并的分支：

    ```bash
    git branch -d dev
    ```

7. 两个分支修改了同一行时会产生冲突，需要手动解决。
8. 打开冲突文件，会看到 `<<<<<<<`、`=======`、`>>>>>>>` 这些标记。
9. 保留正确的内容，把标记全部删掉，然后重新 `add` 和 `commit`：

    ```bash
    git add hello.txt
    git commit -m "Resolve merge conflict."
    ```

### 远程仓库

1. 查看远程仓库地址：

    ```bash
    git remote -v
    ```

2. 添加远程仓库，`origin` 是默认名字：

    ```bash
    git remote add origin https://github.com/user/repo.git
    ```

3. 第一次推送用 `-u`，把本地分支和远程分支关联起来：

    ```bash
    git push -u origin main
    ```

4. 之后直接 `push` 即可：

    ```bash
    git push
    ```

5. 拉取远程更新并合并到当前分支：

    ```bash
    git pull
    ```

6. 只下载远程更新，不自动合并：

    ```bash
    git fetch
    ```

7. 克隆自己 fork 的仓库后，可以再加一个 `upstream` 指向原仓库：

    ```bash
    git remote add upstream https://github.com/OceanCing/missing-semester.git
    git fetch upstream
    git merge upstream/main
    ```

### 撤销与回退

1. 撤销工作区里某个文件的修改（还没有 `add` 时）：

    ```bash
    git restore hello.txt
    ```

2. 把文件从暂存区撤回到工作区：

    ```bash
    git restore --staged hello.txt
    ```

3. 回退最近一次提交，修改保留在暂存区：

    ```bash
    git reset --soft HEAD~1
    ```

4. 回退最近一次提交，修改保留在工作区（默认模式）：

    ```bash
    git reset HEAD~1
    ```

5. 回退并丢弃所有修改（危险，慎用）：

    ```bash
    git reset --hard HEAD~1
    ```

6. 已经推送到远程的提交，用 `revert` 生成一个反向提交更安全：

    ```bash
    git revert <commit-id>
    ```

### .gitignore 和 README

1. 在仓库根目录创建 `.gitignore`，用来忽略不需要提交的文件。
2. 常见写法：

    ```text
    *.log
    build/
    .env
    ```

3. 已经被 Git 跟踪的文件，后来写进 `.gitignore` 不会自动失效：

    ```bash
    git rm --cached secret.txt
    ```

4. 每个仓库最好写一个 `README.md`，说明项目是做什么的、怎么运行。
5. 开源项目一般还会添加一个 `LICENSE` 文件。

### 标签

1. 给重要的提交打标签，一般用来标记版本号：

    ```bash
    git tag v1.0
    ```

2. 创建带说明的标签：

    ```bash
    git tag -a v1.0 -m "First release."
    ```

3. 把标签推送到远程：

    ```bash
    git push origin v1.0
    git push origin --tags
    ```

## GitHub

### GitHub 是什么

1. GitHub 是基于 Git 的代码托管平台，可以存放远程仓库。
2. 除了代码，还提供 Issue、Pull Request、Actions、Wiki 等功能。
3. 既可以把 GitHub 上的仓库克隆到本地，也可以把本地仓库推送到 GitHub。

### Fork 和 Pull Request

1. Fork 就是把别人的仓库复制一份到自己账号下，得到自己的远程仓库。
2. 克隆自己的 fork：

    ```bash
    git clone https://github.com/your-name/repo.git
    ```

3. 新建开发分支，不要直接在 `main` 上修改：

    ```bash
    git switch -c dev
    ```

4. 提交并推送到自己的远程：

    ```bash
    git add .
    git commit -m "Add lesson one notes."
    git push -u origin dev
    ```

5. 在 GitHub 页面上点击 `Compare & pull request`，向原仓库发起 PR。
6. PR 标题用英文，首字母大写；描述可以写中文，但要讲清楚改了什么。
7. PR 提交后继续往这个分支 `push`，新的 commit 会自动更新到 PR。
8. 看到 review comments 后，在本地改完再 `commit`、`push` 即可。
9. 不要用关闭 PR 再重新开一个的办法来躲避冲突，应该认真解决冲突。

### 常见错误

1. 改完文件忘记 `git add`，直接 `commit`，结果提交是空的。
2. 没有配置 `user.name` 和 `user.email`，`commit` 报错或者作者信息不对。
3. 第一次 `push` 忘记加 `-u`，之后每次都要写完整的分支名。
4. `push` 之前没有先 `pull`，远程有新提交时会推送失败。
5. 解决冲突时只改了内容，没有删掉 `<<<<<<<` 和 `>>>>>>>` 标记。
6. 把密码、token、`.env` 这类敏感文件提交到了仓库。
7. 一直在 `main` 分支上开发，没有新建分支。
8. 对已经推送到远程的提交使用 `reset --hard`，影响其他人。
9. Windows 文件名不区分大小写，Linux 区分，改名时要注意。

### 常用命令

| 操作 | 命令 |
| --- | --- |
| 查看状态 | `git status` |
| 添加文件 | `git add <file>` |
| 提交 | `git commit -m "Message."` |
| 查看历史 | `git log --oneline` |
| 新建并切换分支 | `git switch -c <branch>` |
| 合并分支 | `git merge <branch>` |
| 添加远程 | `git remote add origin <url>` |
| 推送 | `git push -u origin <branch>` |
| 拉取 | `git pull` |
| 克隆 | `git clone <url>` |
