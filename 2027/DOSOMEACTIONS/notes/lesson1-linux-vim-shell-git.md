# 培训课第一课学习笔记：Linux · Vim & Shell · Git & GitHub

> 课程时间：2026-10-17 19:30–21:30
> 主讲：陈海洋
> 作者：DOSOMEACTIONS

---

## 一、Linux 基础

### 1.1 一切皆文件

Linux 把设备也当作文件，控制组的串口、飞控都在 `/dev` 下：

| 路径 | 内容 |
| --- | --- |
| `/` | 根目录，一切都挂在它下面 |
| `/etc` | 配置文件 |
| `/home` | 用户家目录 |
| `/dev` | 设备文件（串口 `/dev/ttyUSB0`）|
| `/proc` | 内核与进程的虚拟文件系统 |
| `/var/log` | 日志 |

### 1.2 必会命令

```bash
pwd                    # 我在哪
ls -lah                # 详细列表（-l 详细 -a 隐藏文件 -h 人类可读）
cd ~                   # 回家
cd -                   # 回上一个目录
find . -name "*.py"    # 按名字找
grep -rn "关键词" .     # 递归搜内容
```

**我现在用得最多的三条：**

| 命令 | 我的备注 |
| --- | --- |
| `pwd` | 认路 |
| `git status` | 看仓库状态 |
| `gnome-text-editor` / `vim` | 改文件 |

### 1.3 权限与串口坑

```bash
# 接飞控报 Permission denied: '/dev/ttyUSB0'
sudo usermod -aG dialout $USER    # 加进 dialout 组
# ⚠️ 必须重新登录才生效
```

---

## 二、Vim

### 2.1 两种模式

```
普通模式 ──(按 i)──▶ 插入模式
   ▲                    │
   └─────(按 Esc)───────┘
```

**核心口诀：不知道怎么办就按 `Esc`。**

### 2.2 最小生存命令

| 命令 | 作用 |
| --- | --- |
| `i` | 进入插入模式 |
| `Esc` | 回普通模式 |
| `:w` | 保存 |
| `:wq` | 保存并退出 |
| `:q!` | 不保存强制退出 |
| `jk` | 代替 Esc |
| `dd` | 删行 |
| `u` | 撤销 |

---

## 三、Shell

### 3.1 三个核心概念

**管道 `|`** —— 把上一个命令的输出喂给下一个：

```bash
ps aux | grep ros
cat 日志 | grep ERROR | head -20
```

**重定向** —— 把输出写进文件：

```bash
命令 > 文件            # 覆盖
命令 >> 文件           # 追加
命令 > 全部.log 2>&1   # 正确 + 错误都写进去
```

**变量与命令替换**：

```bash
NAME="世界"
echo "你好，$NAME"
echo "今天是 $(date +%F)"
```

### 3.2 脚本要加执行权限

```bash
chmod +x 脚本.sh
./脚本.sh
```

---

## 四、Git & GitHub

### 4.1 四个阶段

```
① 工作区  ──git add──▶  ② 暂存区  ──git commit──▶  ③ 本地仓库  ──git push──▶  ④ 远程仓库
```

| 阶段 | 存什么 | 命令 |
| --- | --- | --- |
| 工作区 | 你改的文件 | `git diff` |
| 暂存区 | 下次提交的快照 | `git add` / `git add -p` |
| 本地仓库 | 历史提交 | `git commit` |
| 远程仓库 | GitHub 上的备份 | `git push` |

### 4.2 日常流程

```bash
git status -sb              # 我在什么状态
git diff                    # 改了什么
git add .                   # 挑进暂存区
git commit -m "说明"        # 存进本地历史
git push                    # 传到 GitHub
```

### 4.3 两个遥控器

| 名字 | 指向 | 用途 |
| --- | --- | --- |
| `origin` | 自己的 fork | push |
| `upstream` | 团队仓库 | pull 团队更新 |

### 4.4 push 被拒怎么办

看到 `! [rejected] (fetch first)` 说明远程有本地没有的提交：

```bash
git pull --rebase origin main
git push
```

**⚠️ 绝对不要 `git push -f`** —— 会覆盖别人的提交。

---

## 五、本次课的收获与疑问

### 收获

1. 没事多按 `Esc`
2. `Ctrl+R` 看历史命令
3. `Ctrl+L` 清屏

### 还没搞懂的

1. GitHub 有没有中文插件？

---

## 参考

- [菜鸟教程](https://www.runoob.com/)
- [Git 官方文档](https://git-scm.com/doc)
- [GitHub 辅助插件 B 站介绍](https://www.bilibili.com/video/BV1GJ411x7h7)
