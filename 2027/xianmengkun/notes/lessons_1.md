# Notes 1

# 1.Linux

### 目录结构

一切从 `/` 开始，**路径大小写敏感**。

- `/home/user` — 用户文件
- `/etc` — 系统配置
- `/usr` — 程序与库
- `/var` — 日志等可变数据
- `/dev` — 设备文件

三种路径写法：

1. 绝对路径：`/home/user/example`
2. 相对路径：`./hello.txt`
3. 家目录：`~/example`

> `~` 是当前用户的家目录，`.` 是当前目录，`..` 是上一级

## 2. Shell

### 2.1 终端 / Shell / 程序 的分工

```
Terminal（输入输出界面） → Shell（解释命令、展开路径） → 程序 / 系统
```

命令的基本结构是 `命令 [选项] [参数]`

### 2.2 常用命令

| 命令              | 作用                           |
| --------------- | ---------------------------- |
| `pwd`           | 显示当前目录                       |
| `ls` / `ls -la` | 列出目录内容（`-l` 详情，`-a` 含隐藏项）    |
| `cd 目录`         | 进入目录（`cd ..` 上一级，`cd ~` 家目录） |
| `mkdir -p`      | 创建目录，允许补齐父目录                 |
| `touch`         | 创建空文件 / 更新时间                 |
| `cp`            | 复制文件                         |
| `mv`            | 移动或重命名                       |
| `rm`            | 删除文件                         |
| `cat` / `less`  | 直接查看 / 分页查看                  |

### 2.3 四种常用组合

```bash
# 变量
name="RM"
echo "$name"

# 重定向：> 覆盖写入，>> 追加
echo "hello" > hello.txt

# 管道：把输出交给下一个程序
ls -la | grep txt

# 条件连接：前一条成功后执行下一条
mkdir -p demo && cd demo
```

### 2.4 帮助

`ls --help` — 快速看选项

man ls`— 打开手册，按`q\` 退出

`Tab` — 补全命令或路径  
`↑` / `↓` — 调出历史

### 3.1 必备按键

| 按键         | 操作            |
| ---------- | ------------- |
| `i`        | 进入插入模式        |
| `Esc`      | 返回普通模式        |
| `h j k l`  | 左 / 下 / 上 / 右 |
| `dd`       | 删除当前行         |
| `yy` / `p` | 复制当前行 / 粘贴    |
| `u`        | 撤销            |
| `/word`    | 搜索 word       |

### 3.2 保存与退出

`:w` 保存

`:q` 退出  
`:wq` 保存退出  
`:q!` 放弃修改退出

### 4.1 分工

| Git（本地版本控制） | GitHub（远程托管与协作） |
| ----------- | --------------- |
| 记录每次提交      | 托管 Git 仓库       |
| 比较修改、查看历史   | 通过 PR 讨论和评审     |
| 为新功能创建分支    | 合并团队认可的修改       |

### 4.2 四个位置

```
工作区 --git add--> 暂存区 --git commit--> 本地仓库 --git push--> GitHub 远程仓库
```

`commit` 只记录到本地仓库，`push` 才把提交发送到远程。

### 4.3 常用命令

```bash
git clone <仓库地址>          # 下载仓库及提交历史
git status                   # 查看当前分支和文件状态
git diff                     # 查看尚未暂存的修改
git log --oneline            # 简短格式查看提交历史
```

### 4.4 分支提交

```bash
git switch -c feature/hello        # 创建并切换到功能分支
git add hello.txt                  # 暂存指定文件
git commit -m "Add greeting."      # 记录一次本地提交
git push -u origin feature/hello   # 推送并建立跟踪关系
git switch main                    # 切回主分支
git pull                           # 获取并整合远程更新
```

### 4.5  PR

```
Fork → Clone → Feature 分支 → 修改/Add/Commit → Push → PR → Review → Merge
```
