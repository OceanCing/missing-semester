# Git 学习笔记

> Git 是本地版本控制工具，GitHub 是托管 Git 仓库的网站。前者管版本，后者管协作。

## 0. 先看懂四个区

```
工作区            暂存区            本地仓库           远程仓库
(正在改的文件) --add--> (待提交) --commit--> (历史版本) --push--> (GitHub)
              <--restore--       <--reset--        <--pull--
```

| 区域 | 放什么 | 常用命令 |
| --- | --- | --- |
| 工作区 | 你正在编辑的文件 | 直接改文件 |
| 暂存区 | 本次准备提交的内容 | `git add` |
| 本地仓库 | 已经提交的历史快照 | `git commit` |
| 远程仓库 | GitHub 上的副本 | `git push` / `git pull` |

一个比喻：写文档（工作区）→ 装进书包（`add`）→ 存档（`commit`）→ 上传云盘（`push`）。

## 1. 开始之前：配置身份

```bash
git config --global user.name "你的名字"
git config --global user.email "你的邮箱"
git config --global --list      # 检查配置
```

每次提交都会记录作者信息，所以一台机器配置一次即可，`--global` 表示对所有仓库生效。

## 2. 拿到仓库：init 与 clone

```bash
# 情况一：从零新建一个仓库
mkdir myproject && cd myproject
git init

# 情况二：把远程仓库复制到本地
git clone git@github.com:<你的用户名>/<仓库名>.git
```

`clone` 会自动把远程地址记成 `origin`，不需要再手动添加。

## 3. 查看状态：status / diff / log

### git status

看当前哪些文件被改了、哪些已经 `add`。

```bash
git status          # 详细版
git status -s       # 简洁版
```

常见三种状态：

- **untracked**：新文件，Git 还没开始管理
- **modified**：改过，但还没 `add`
- **staged**：已经 `add`，等着被 `commit`

### git diff

看具体改了什么。

```bash
git diff            # 工作区 vs 暂存区（还没 add 的改动）
git diff --staged   # 暂存区 vs 上一次提交（已经 add 的改动）
git diff HEAD       # 工作区 + 暂存区 vs 上一次提交
```

### git log

```bash
git log                                  # 完整日志
git log --oneline                        # 一行一条，最常用
git log --pretty=oneline                 # 同样一行一条，显示完整哈希
git log --oneline --graph --all          # 看分支合并图
git log -n 3                             # 只看最近 3 条
git show <提交哈希>                       # 看某次提交的细节
```

`git log` 默认从最近到最远显示，信息很多容易看花眼；日常用 `--oneline` 就够了。

## 4. 提交：add → commit 两步

```bash
git add git_study.md        # 添加单个文件
git add .                   # 添加当前目录的所有改动
git add -A                  # 添加整个仓库的所有改动

git commit -m "Add Git study notes."
```

- `-m` 后面是本次提交的说明，写清楚“做了什么”。
- 提交信息建议：**英文、首字母大写、句号结尾**，例如 `Fix typo in README.`。
- `git commit -am "..."` 可以省掉 `add`，但只对**已被 Git 跟踪**的文件有效，新文件不适用。
- 一次 `commit` 就是一个快照，出问题可以回到任意一个快照。

`add` 的意义在于“挑选”：你可以只把部分改动放进暂存区，实现分次提交。

## 5. 版本回退

### 5.1 reset 的三种模式

`HEAD` 指向当前版本，`HEAD^` 是上一个版本，`HEAD~2` 是上两个版本。

| 模式 | 回退后改动在哪 | 会丢代码吗 | 典型场景 |
| --- | --- | --- | --- |
| `--soft` | 保留在**暂存区** | 不会 | 想重新写提交信息、合并几次提交 |
| `--mixed`（默认） | 保留在**工作区**，未暂存 | 不会 | 想重新挑选文件再 `add` |
| `--hard` | **直接丢弃** | **会** | 确定这些改动不要了 |

```bash
git reset --soft HEAD^      # 撤销 commit，改动仍在暂存区
git reset HEAD^             # 等于 --mixed，改动回到工作区
git reset --hard HEAD^      # 连改动一起清掉
git reset --hard e475afc    # 也可以直接回到某个提交
```

> 注意：原笔记里对 `--soft` / `--mixed` 的描述不准确。记住一句话：**soft 留在暂存区，mixed 留在工作区，hard 全丢。**

`reset` 会改写历史，只适合**还没 push** 的本地提交。已经 push 的提交不要用 `reset`。

### 5.2 revert：不改历史的回退

```bash
git revert <提交哈希>
```

它不会删除原提交，而是**新建一个提交**把那次改动反向抵消，所以对团队共享的分支更安全。

### 5.3 丢弃工作区的修改

```bash
git checkout -- file.md     # 老写法
git restore file.md         # 新写法，推荐
```

`--` 很重要：没有它，`git checkout file.md` 可能被理解成“切换到某个分支”。如果文件已经 `add`，先取消暂存再恢复：

```bash
git restore --staged file.md
git restore file.md
```

### 5.4 reflog：后悔药

```bash
git reflog
git reset --hard <reflog 里的哈希>
```

即使误用了 `--hard`，`reflog` 也记录了 `HEAD` 的移动历史，通常还能找回来。

## 6. 分支

```bash
git branch                  # 查看本地分支
git switch -c feature/xxx   # 新建并切换（等价于 git checkout -b）
git switch master           # 切换分支
git merge feature/xxx       # 把 feature 分支合并进当前分支
git branch -d feature/xxx   # 删除已合并的分支
```

分支的作用是**隔离开发**：新功能在单独分支上做，`master` / `main` 始终保持可用。

## 7. 远程仓库与推送

```bash
git remote -v               # 查看远程地址
git remote add origin <地址> # 添加远程
git push -u origin master   # 第一次推送，并记住上游分支
git push                    # 之后直接 push
git pull                    # 拉取远程更新并合并
```

- `-u` 的作用是把本地分支和远程分支关联起来，之后只写 `git push` 即可。
- SSH 地址示例：`git remote set-url origin git@github.com:<你的用户名>/<仓库名>.git`
- 如果 `push` 被拒绝（non-fast-forward），说明远程有你本地没有的提交，先 `git pull`（或 `git pull --rebase`）再 push。
- 有的仓库默认分支叫 `main` 而不是 `master`，以仓库实际为准。

## 8. SSH 免密连接 GitHub

```bash
ssh-keygen -t ed25519 -C "你的邮箱"   # 一路回车即可
cat ~/.ssh/id_ed25519.pub             # 复制公钥内容
```

把公钥添加到 GitHub：`Settings → SSH and GPG keys → New SSH key`。

- **公钥**（`.pub`）可以给别人；
- **私钥**（没有 `.pub` 后缀）绝对不能上传或泄露。

测试是否成功：

```bash
ssh -T git@github.com
```

看到 `You've successfully authenticated` 就说明配好了。

## 9. Fork + PR 团队协作

一句话：**PR（Pull Request，拉取请求）就是你向上游仓库的维护者提交代码修改的“申请信”。**

你无法直接修改上游仓库，只能先 Fork 一份到自己账号下，改完再请求维护者合并。

```
上游仓库（upstream）
        │ Fork（复制一份到你账号）
        ▼
你的仓库（origin）
        │ Clone（下载到本地）
        ▼
本地改文件 → add → commit → push
        │
        ▼
GitHub 上 Open Pull Request → Review → Merge
```

完整流程：

1. **Fork**：在上游仓库页面点 Fork，得到自己的副本。
2. **Clone**：`git clone` 自己的仓库到本地。
3. **建分支**：`git switch -c feature/xxx`，不要直接在 `master` 上改。
4. **改文件并提交**：`git add` + `git commit -m "..."`。
5. **Push**：`git push -u origin feature/xxx`。
6. **开 PR**：在 GitHub 上请求把自己的分支合并到上游仓库，写清楚改了什么。

PR 的核心作用：

- **请求合并**：告诉维护者“我在自己的仓库改好了，请合并”。
- **代码审查**：维护者可以在 comments 里指出问题，你修改后**新提交会自动更新到这个 PR**。
- **不要用关掉 PR 再重开的方式逃避冲突**：正确做法是把上游仓库的新内容合并进你的分支，解决冲突后继续 push。

## 10. 常见坑

| 现象 | 原因 / 解决 |
| --- | --- |
| `fatal: not a git repository` | 当前目录不是仓库，`cd` 到仓库目录 |
| `push` 被拒绝 | 远程有新提交，先 `git pull` 再 push |
| 改完发现没进提交 | 忘记 `git add`，改动还在工作区 |
| `--hard` 后代码没了 | 用 `git reflog` 找到哈希再 `reset --hard` |
| 冲突（conflict） | 打开冲突文件，保留正确内容，删掉 `<<<<<<<` 等标记，再 `add` + `commit` |
| 不想让某些文件进仓库 | 写进 `.gitignore`，例如 `build/`、`*.log` |

## 11. 速查表

| 命令 | 作用 |
| --- | --- |
| `git status` | 看当前状态 |
| `git diff` | 看未暂存的改动 |
| `git log --oneline` | 看提交历史 |
| `git add <file>` | 加入暂存区 |
| `git commit -m "..."` | 提交 |
| `git reset --soft HEAD^` | 撤销提交，保留在暂存区 |
| `git reset --hard HEAD^` | 撤销提交并丢弃改动 |
| `git restore <file>` | 丢弃工作区修改 |
| `git revert <hash>` | 反向提交，不改历史 |
| `git switch -c <branch>` | 新建并切换分支 |
| `git push -u origin <branch>` | 首次推送并关联上游 |
| `git pull` | 拉取并合并远程更新 |
| `git reflog` | 查看 HEAD 移动历史（后悔药） |
