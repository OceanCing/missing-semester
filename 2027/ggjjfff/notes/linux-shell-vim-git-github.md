# 第一课笔记

linux、shell、vim、git 这几样

## Linux

linux 其实只是内核，我们用的是发行版，也就是 ubuntu。老师说这个不用纠结，知道 ubuntu 是 linux 的一种就行。

队里用 ubuntu 是因为 ros 装别的系统很麻烦，开发机和车上的环境也要一样。

写代码不是坐在车上写，是 ssh 连过去改。车上的电脑没有桌面，程序开机自己就跑了。所以终端里得会干活，包括改文件。

### 我在哪

```bash
pwd
```

### 路径怎么写

- `/home/ggjjfff/a.txt` 从头开始写的，叫绝对路径
- `./a.txt` 当前这个目录里的
- `~/a.txt` 家目录，`~` 就是 `/home/ggjjfff`
- `..` 上一级

还有 `.` 是当前目录，这个一开始老和 `..` 搞混。

**大小写不一样！** readme 和 README 是两个文件。我试了一下确实报错说找不到文件。

### 目录

暂时只记住几个：`/home` 是我的东西，`/etc` 放配置，`/usr` 放程序，`/var` 放日志，`/dev` 是设备。其他先不管。

## Shell

终端只是个窗口，真正解释命令的是 shell（bash）。

`cd` 敲完什么都不打印，我一开始以为没成功，其实成功了。因为 cd 是 shell 自己的命令，不启动新程序。

命令一般长这样：

```bash
ls -la ~/demo
```

前面是命令，`-la` 是选项，`~/demo` 是参数。

### 现在会用的

```bash
pwd             # 看我在哪
ls -la          # 列出来，-a 连隐藏文件一起
cd 目录          # 进去
mkdir -p a/b    # -p 才能一次建两层
touch a.txt     # 建个空文件
cp a b          # 复制
mv a b          # 移动，也能改名
rm a.txt        # 删掉，不进回收站！！
cat a.txt       # 看内容
```

rm 那个老师强调了，删了就没了，敲之前先看清路径。

### 四个小技巧

1. 变量 `name=robot`，用的时候 `$name`。等号两边不能加空格，加了就错。
2. `>` 是写进去（会覆盖），`>>` 是加到后面。
3. `|` 管道，`ls | grep txt` 就是把 ls 的结果给 grep。
4. `&&` 前面成功了才执行后面，`mkdir a && cd a` 很常用。

### 忘了参数怎么办

`命令 --help` 看简短的，`man 命令` 看详细的，按 q 退出。

Tab 能补全，上下键能翻之前敲的命令，这两个一定要用。

## Vim

vim 有模式，同一个键在不同模式下干的事不一样，所以一开始特别容易乱。

三个模式：

- 普通模式：打开就是。移动、删除、复制都在这。
- 插入模式：按 `i` 进来，才能打字。
- 命令行模式：按 `:` 进来，保存和退出。

不知道自己在哪个模式就按 Esc。

完整流程：

```bash
vim hello.txt
i          # 打字
Esc
:wq        # 保存退出
```

会用这几个就差不多了：`hjkl` 左下上右（不是方向键），`dd` 删一行，`yy` 复制，`p` 粘贴，`u` 撤销，`/word` 搜索。

保存退出：
- `:w` 保存
- `:q` 退出（没保存会不让退）
- `:wq` 保存并退出
- `:q!` 不保存，硬退

改坏了就用 `:q!` 跑掉。

## Git 和 GitHub

git 是本地的，管版本；github 是把仓库放网上，还能开 PR 让人看。

### 四个区（这块最容易乱）

- 工作区：我改的文件
- 暂存区：`git add` 之后放这，等于挑出来这次要提交哪些
- 本地仓库：`git commit` 之后，有记录了
- 远程仓库：`git push` 之后，别人才能看到

**commit 完不 push，队友那边什么都没有。** 这个我老是忘。

### 第一次要配置

```bash
git config --global user.name "ggjjfff"
git config --global user.email "邮箱"
```

这只是署名，不是登录 github，别搞混。

### ssh

```bash
ssh-keygen -t ed25519 -C "邮箱"
cat ~/.ssh/id_ed25519.pub
ssh -T git@github.com
```

`.pub` 结尾的是公钥，可以给别人。另一个私钥谁都不能给，也别截图发群里。

`ssh -T` 成功的时候也会显示一串提示，只要能认出 Hi 你的用户名就说明通了。

### 常用

```bash
git clone 地址              # 下载下来
git status                  # 随时看一眼
git switch -c lesson1       # 开分支再改
git add 文件名               # 别 git add .，会把乱七八糟的也加进去
git commit -m "Add xxx."    # 首字母大写，最后带句号
git push -u origin lesson1
git log --oneline
```

### fork 和 PR

流程是：先把团队的仓库 fork 一份到自己账号下，然后 clone 自己那份，改完 push 回自己的，再开 PR 让团队合。

fork 是复制出一个新仓库，branch 是同一个仓库里分出来的线，不一样。

`origin` 指的是自己的 fork。

PR 开完还能继续改。老师说要改就往同一个分支再 push，PR 会自己更新，不要关掉重开，关掉重开要扣分。
