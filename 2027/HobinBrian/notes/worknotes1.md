| 路径 | 说明 | | 示例 | 说明 |
| :--- | :--- | :-- | :--- | :--- |
| `/` | 根目录 | | `/home/user/example` | 绝对路径：从 `/` 开始 |
| `/home/user` | 用户文件 | | `./hello.txt` | 相对路径：从当前位置开始 |
| `/etc` | 系统配置 | | `~/example` | home 目录中的 example |
| `/usr` | 程序与库 | | | |
| `/var` | 日志等可变数据 | | | |
| `/dev` | 设备文件 | | | |

### 终端、Shell 与命令

| 层 |                               |
| :--- | :--- |
| Terminal 终端 | 输入输出界面 |
| Shell | 解释命令、展开路径、组织执行 |
| 程序 / 系统 | `ls` 查看文件，`git` 管理版本 |

### 当前位置与目录切换

| 命令 | 作用 |
| :--- | :--- |
| `pwd` | 显示当前目录 |
| `ls` | 列出目录内容 |
| `cd 目录` | 进入指定目录 |
| `cd ..` | 返回上一级 |
| `cd ~` | 返回家目录 |

### 文件和目录操作

| 命令 | 作用 | 示例 |
| :--- | :--- | :--- |
| `mkdir -p` | 创建目录，允许补齐父目录 | `mkdir -p ~/linux_demo` |
| `touch` | 创建空文件 / 更新时间 | `touch hello.txt` |
| `cp` | 复制文件 | `cp hello.txt hello-copy.txt` |
| `mv` | 移动或重命名 | `mv hello-copy.txt backup.txt` |
| `rm` | 删除文件 | `rm backup.txt` |
| `cat` / `less` | 直接查看 / 分页查看 | `cat hello.txt` |

### Shell 的四种常用组合

| 组合 | 写法 | 作用 |
| :--- | :--- | :--- |
| 变量 | `name="RM"` `echo "$name"` | 保存一个值，再通过变量引用它 |
| 重定向 | `echo "hello" > hello.txt` | `>` 覆盖写入；`>>` 追加到文件末尾 |
| 管道 | `ls -la \| grep txt` | 将输出交给下一个程序 |
| 条件连接 | `mkdir -p demo && cd demo` | 前一条成功后，再执行下一条 |

### 陌生命令的帮助入口

| 方式 | 说明 |
| :--- | :--- |
| `ls --help` | 快速查看选项和用法 |
| `man ls` | 打开手册，按 `q` 退出 |
| `Tab` | 补全命令或路径 |
| `↑` / `↓` | 调出历史命令 |

### Git 的四个位置（41:05）

commit 记录到本地仓库pus 才把提交发送到远程仓库。

### 使用 Git提交作者

```bash
git config --global user.name  "Your Name"
git config --global user.email "you@example.com"
git config --get user.name
```

### SSH 密钥：公钥与私钥

```bash
ssh-keygen -t ed25519 -C "you@example.com"
cat ~/.ssh/id_ed25519.pub
```

### 把 SSH 公钥添加到 GitHub

1. `Settings`
2. `SSH and GPG keys`
3. `New SSH key`

选择 `Authentication Key`，填写名称，粘贴完整公钥；验证 `ssh -T git@github.com`。
**首次连接先核对官方主机指纹。**

### Git 常用命令：获取与查看

| 命令 | 作用 |
| :--- | :--- |
| `git clone <仓库地址>` | 下载仓库及提交历史 |
| `git status` | 查看当前分支和文件状态 |
| `git diff` | 查看尚未暂存的修改 |
| `git log --oneline` | 用简短格式查看提交历史 |

### Git 常用命令：分支与提交

| 命令 | 作用 |
| :--- | :--- |
| `git switch -c feature/hello` | 创建并切换到功能分支 |
| `git add hello.txt` | 暂存指定文件 |
| `git commit -m "Add greeting"` | 记录一次本地提交 |
| `git push -u origin feature/hello` | 推送并建立跟踪关系 |
| `git switch main` | 切回主分支 |
| `git pull` | 获取并整合当前分支的远程更新 |

### 分支：为新功能保留独立开发线

```
main          ●────────●────────●────────●
                       \               /
feature/hello           ●────●────●──
                                    PR / Review / Merge
```

### 团队 PR 工作流

```
团队仓库 ──fork──→ 个人 Fork ──clone──→ 本地电脑 ──Feature 分支──→ 修改/Add/Commit ──→ Push
                                                                                    ↓
团队仓库 ←── Merge ←── Review 评审 ←── 发起 PR ←── 个人 Fork 接收提交 ←──────────────┘
```

**仓库链接：https://github.com/OceanCing/missing-semester**