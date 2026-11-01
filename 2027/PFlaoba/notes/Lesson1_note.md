# DX控制组第1课笔记

> 第一次使用 Markdown 记录笔记，排版如有不规范之处，请指正

## 目录

- **一、 Linux**
- **二、 Vim**
- **三、 Shell**
- **四、 Git && Github**
- **五、 CPP**
- **总结**

## 一、 Linux

### 0. 什么是Linux

#### 0.1 操作系统(OS，Operating System)

Windows，macOS，Linux都是操作系统，但管理风格不同
而Linux免费，开源，可以给你最高权限(root)

#### 0.2 发行版

Linux通常指内核(Kernel)
发行版(distribution，也称distro) = Linux内核 + 系统工具与软件包 + 默认配置
发行版种类繁多，常见的有 Ubuntu，Debian，Fedora，Arch Linux
团队使用 Ubuntu: ROS兼容性、部署一致性和现有开发生态

### 1. 文件系统

#### 1.1 目录树

Linux目录从`/`开始，所有东西都挂在其下

#### 1.2 目录的用途(FHS标准)

| 目录 | 全称/来源 | 装的内容 |
|---|---|---|
| `/` | root | 整棵树 |
| `/home` | home | 每个普通用户的个人目录 |
| `/etc` | et cetera(等等) | 系统/软件的**配置文件** |
| `/usr` | user system resources | 绝大部分软件、库、文档 |
| `/var` | variable | 日志、缓存、数据库等**可变**数据 |
| `/tmp` | temporary | 临时文件 |
| `/dev` | device | 设备文件(硬盘、U盘、摄像头) |
| `/proc` | process | 内核和进程的实时信息(虚拟文件) |
| `/bin` `/sbin` | binary / system binary | 最基础的命令 |
| `/boot` | boot | 内核、启动引导 |
| `/mnt` `/media` | mount / media | 挂载外接设备的地方 |
| `/opt` | optional | 额外装的独立大软件 |

#### 1.3 一切皆文件

Linux 有个重要设计思想：**everything is a file(一切皆文件)**
硬盘、键盘、屏幕、甚至内存信息，都被"伪装"成文件放在 `/dev`、`/proc` 里
好处是：**操作它们和操作普通文件用同一套命令**

#### 1.4 路径

- 路径大小写敏感
- 绝对路径：从`/`开始
- 相对路径：从当前位置开始
- `~`是当前用户的家目录，例如：`~/example`指`home`目录中的`example`
- `.`是当前目录
- `..`是上一级目录

### 2. 用户、组与权限

#### 2.1 多用户系统

Linux 最初就是给"很多人共用一台大机器"设计的，所以每个文件都记着：谁拥有它、谁属于哪个组、别人能不能碰
- 用户(user)：每个登录的人，敲`whoami`就能看到现在的用户名
- 组(group)：把几个用户归成一队，方便统一授权

```bash
whoami                 # 我是谁
id                     # identity = 我的用户 ID、所属组
cat /etc/passwd        # 所有用户名单(每行一个用户)
cat /etc/group         # 所有组的名单
```

#### 2.2 root

- root 是系统的超级用户(superuser)，权限最大，可随意删改
- 平时应该用普通用户，需要管理员权限时，临时用`sudo`提权

| 命令 | 全称 |
|---|---|
| `sudo <命令>` | superuser do |
| `su` | switch user(不写用户名默认切换成root) |
| `whoami` | who am i |

#### 2.3 `ls -l`权限串

```bash
ls -l 某个文件
# 输出类似：
# -rwxr-xr-- 1 user1 group1 0 Oct 17 10:00 某个文件
#  ↑权限串    ↑所有者 ↑所属组
```
权限串有10个字符，拆开看
```v
-   rwx   r-x   r--
│    │     │     │
│    │     │     └─ 其他人(other)的权限：只读
│    │     └─────── 同组(group)的权限：读+执行
│    └───────────── 所有者(owner)的权限：读+写+执行
└─ 文件类型（- 普通文件，d 目录，l 链接）

```

三个字母一组，分别执行对应内容

| 字母 | 全称 | 对文件 | 对目录 |
|---|---|---|---|
| `r` | read 读 | 能看内容 | 能列出里面有什么 |
| `w` | write 写 | 能改内容|能在里面建/删文件 |
| `x` | execute 执行 | 能当程序运行 | 能进入这个目录(`cd` 进去) |

#### 2.4 改权限：`chmod` 与数字权限

`chmod`(**ch**ange **mod**e，改模式)有两种写法，**数字写法最常用**

| 数字 | 二进制 | 权限 | 解释 |
|---|---|---|---|
| 4 | 100 | r-- | 只能读 |
| 2 | 010 | -w- | 只能写 |
| 1 | 001 | --x | 只能执行 |
| 6 | 110 | rw- | 读+写 |
| 7 | 111 | rwx | 读+写+执行(全能) |
| 5 | 101 | r-x | 读+执行 |

三个数字，依次代表 **所有者 / 同组 / 其他人**  
权限用法见 Shell 笔记

### 3. 进程

#### 3.1 进程是什么

**进程(process)** = 正在运行的程序  
打开一个程序，系统就分配给它一块内存、一个 CPU 时间片，还发一个**编号 PID**(**P**rocess **ID**，进程号)

```bash
# 看所有进程的快照(ps = process status,进程状态)
ps aux                # a=所有用户 u=详细信息 x=包括没终端的

# 实时看进程、CPU、内存(任务管理器)
top                   # 按 q 退出
htop                  # 更好看,需要先 sudo apt install htop

# 找某个进程,配合 grep
ps aux | grep python
```

#### 3.2 前台、后台，以及 `&` 和 nohup

在终端里跑命令，默认是**前台（foreground）**——命令没跑完，你就不能敲下一条。放到**后台（background）**就不挡你。

```bash
# 在命令末尾加 & ,让它去后台跑
sleep 100 &           # sleep 100 = 睡 100 秒；& 让它后台睡
# 输出会显示一个 [1] 12345 —— [1] 是后台任务号,12345 是 PID

jobs                  # 看当前终端有哪些后台任务
fg %1                 # foreground，把 1 号任务拉回前台
bg %1                 # background，把任务放回后台

# 关掉终端还想让它继续跑？用 nohup(no hang up = 不挂断)
nohup python 训练.py > 训练.log 2>&1 &
# 意思：后台跑、不因关终端而停、输出写进日志文件
```
#### 3.3 结束进程：kill

```bash
kill 12345            # 结束 12345 号进程
kill -9 12345         # 强制结束(最后手段)
pkill -f 关键字        # 按名字批量结束(process kill)
```

### 4. 软件包

#### 4.1 什么是软件包

Linux 装软件不像 Windows 去官网下安装包，而是从**软件源(epository，仓库)**里取  
Ubuntu 用的工具叫 **apt**(**A**dvanced **P**ackaging **T**ool，高级软件包工具)

```bash
sudo apt update                     
sudo apt upgrade                    
sudo apt install 软件                
sudo apt remove 软件                
sudo apt search 关键词              
apt list --installed    
```

#### 4.2 .deb包

网上下载的`.deb` 文件可用 `dpkg` 装：

```bash
sudo dpkg -i 某某.deb      # debian package -i = install
# 报"缺依赖"则补充：
sudo apt install -f         # -f = fix,自动把缺的依赖补上
```

### 5. 磁盘与挂载

#### 5.1 什么是挂载

前面说 Linux 是一棵树，硬盘、U盘、光驱等都是通过**挂载(mount)**进到树里的：  
把一块硬盘"挂"到某个目录上，之后访问那个目录就是在访问那块硬盘

```
硬盘分区 ──mount──► 挂到某个目录(如 / 或 /home 或 /mnt/usb)
```

- 开机时系统自动把各个分区挂到对应位置(记录在 `/etc/fstab`)
- U 盘插上，桌面系统一般会自动把它挂到 `/media/你的名字/...`

```bash
df -h                 # disk free,看每个挂载点还剩多少空间(-h 人类可读)
lsblk                 # list block devices,看有几块硬盘、怎么分区的
du -sh 某个目录        # disk usage,看这个目录总共占多大
mount                 # 看当前的挂载情况
```

### 6. 环境变量与 PATH

#### 6.1 命令为什么敲了就能跑：PATH

敲 `ls`后，系统是靠一个叫 **PATH** 的环境变量找到这个程序的  
它是一串目录，系统会**挨个目录去找你要跑的命令**

```bash
echo $PATH            # 打印 PATH，一串目录用冒号 : 隔开
# 类似：/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:...

which ls              # which = "哪个"，告诉你 ls 到底在哪个目录
# 输出：/usr/bin/ls
```

- 因为 `/usr/bin` 在 PATH 里，所以在任何地方敲 `ls` 都能跑
- 写了个脚本 `hello.sh`，如果它不在 PATH 里，就得写全路径 `./hello.sh` 才能跑

#### 6.2 环境变量与 export

**环境变量**就是"系统里所有程序都能看到的变量"  
比如 `HOME`(家目录)、`USER`(用户名)、`PATH`(找命令的路径) 
用 `export` 可以把自己的变量"升级"成环境变量，让子程序也能看到

```bash
echo $HOME            # /home/用户名
echo $USER            # 用户名
```

## 二、 Vim

### 0. 基础使用

#### 0.1 模式
```text
        i a o I A O...            Esc(或 Ctrl-[)
  ┌──────────────┐  ─────────────►  ┌──────────────┐
  │  插入模式     │                   │  普通模式     │
  │  (Insert)    │  ◄─────────────   │  (Normal)    │
  │  按键=打字    │        Esc         │  按键=下命令   │
  └──────────────┘                   └──────┬───────┘
                                            │  v V Ctrl-v  : / ?
                                            ▼
                                   ┌────────────────────┐
                                   │ 可视模式 / 命令行模式 │
                                   └────────────────────┘

```

| 模式 | 操作 | 进入方法 |
|---|---|---|
| **Normal**(普通) | 按键 = 命令 | 指挥模式，打开文件的默认模式 |
| **Insert**(插入) | 按键 = 打字 | 输入模式，按 `i` 进入 |
| **Command**(命令行) | `:` 开头 | 下长命令(保存、替换、退出) |
| **Visual**(可视) | 选中 | 先选中一段再操作 |

#### 0.2 必备按键

| 键 | 英文全称 | 作用 |
|---|---|---|
| `i` | **i**nsert | 光标前进入插入模式 |
| `Esc` | escape | 回到普通模式 |
| `:w` | **w**rite | 保存 |
| `:q` | **q**uit | 退出 |
| `:wq` | write + quit | 保存并退出 |
| `:q!` | quit + !(强制)| 不保存强制退出(后悔药) |
| `h j k l` | — | 左 下 上 右 |
| `x` | — | 删除光标处一个字符 |
| `dd` | **d**elete | 删除整行 |
| `yy` | **y**ank | 复制整行(yank = 拽出) |
| `p` | **p**aste | 粘贴到光标后 |
| `u` | **u**ndo | 撤销 |
| `/关键词` | search | 往下搜 回车跳到下一个 |
| `n` | **n**ext | 跳到下一个匹配 |
| `gg` / `G` | go | 跳到文件首 / 尾 |

### 1. 移动

#### 1.1 字符与单词

| 键 | 全称 | 作用 |
|---|---|---|
| `w` | **w**ord | 下一个单词开头 |
| `b` | **b**ack | 上一个单词开头 |
| `e` | **e**nd | 当前/下一个单词结尾 |
| `0` | — | 行首 |
| `^` | — | 行首第一个非空字符 |
| `$` | — | 行尾 |

小写 `w b e` 按"单词"算(连字符、下划线都算分隔符)
**大写 `W B E` 按"一串连续非空字符"算**改代码里的路径、`--flag=value` 这种带符号的参数，用大写版一步到位

#### 1.2 文件与屏幕

| 键 | 作用 |
|---|---|
| `gg` / `G` | 文件开头 / 结尾 |
| `:42` 回车 | 跳到第 42 行 |
| `Ctrl-d` / `Ctrl-u` | 向下 / 向上翻半屏 |
| `Ctrl-f` / `Ctrl-b` | 向下 / 向上翻整屏(f=forward / b=backward) |
| `zz` | 把当前行滚到屏幕中间 |

#### 1.3 结构跳转与行内查找

| 键 | 全称 | 作用 |
|---|---|---|
| `%` | match | 括号 `()[]{}` 之间来回跳(改嵌套代码救命) |
| `f<字符>` | **f**ind | 行内向右找到该字符并停上去 |
| `F<字符>` | find 反向 | 行内向左找 |
| `t<字符>` | **t**ill | 同上，但停在字符**前**一个位置 |
| `;` / `,` | — | 重复 / 反向重复上一次 f/t 查找 |
| `Ctrl-o` / `Ctrl-i` | — | 回到上一个 / 下一个跳转位置 |

> 示例：`f(` 跳到本行的 `(`，`%` 跳到匹配的 `)`——找函数参数范围两秒钟  
>`dt;` 是"删到分号为止"，`ct)` 是"改到右括号前"

### 2. 进入插入模式的几种方式

| 键 | 全称 | 从哪儿开始插 | 什么时候用 |
|---|---|---|---|
| `i` | **i**nsert | 光标前 | 最常用 |
| `a` | **a**ppend | 光标后 | 想在字后面补东西 |
| `I` | Insert 行首 | 行首(首个非空字符) | 行首加注释 `#` |
| `A` | Append 行尾 | 行尾 | 行尾加分号、参数 |
| `o` | **o**pen | 下方新开一行 | 最常用 |
| `O` | Open 上方 | 上方新开一行 | — |
| `s` | **s**ubstitute | 删掉当前字符再插入 | 改错字 |
| `S` | — | 删掉整行再插入 | 重写一行 |

### 3. 删除、复制、粘贴(operator + motion)

#### 3.1 主要 operator

| operator | 全称 | 作用 |
|---|---|---|
| `d` | **d**elete | 删除 |
| `c` | **c**hange | 修改 = 删除 + 进入插入模式 |
| `y` | **y**ank | 复制 |

#### 3.2 组合示例

| 命令 | 拆解 | 含义 |
|---|---|---|
| `dw` | delete word | 删到下一个单词 |
| `d$` / `D` | delete 到行尾 | 删到行尾 |
| `dd` | delete line | 删整行 |
| `3dd` | 3 × delete | 删 3 行 |
| `cw` | change word | 改一个单词 |
| `cc` | change line | 改整行(保留缩进) |
| `yy` / `5yy` | yank | 复制 1 行 / 5 行 |
| `p` | paste | 粘贴到光标后 |
| `P` | Paste 前 | 粘贴到光标前 |

**口诀**：`d`/`c`/`y` + 任何移动方式 = 那个范围的操作。

#### 3.3 缩进与其他 operator

| 命令 | 全称 | 作用 |
|---|---|---|
| `>>` / `<<` | indent | 当前行右/左缩进一格 |
| `3>>` | — | 3 行一起缩进 |
| `=` / `gg=G` | format | 自动缩进 / 全文件重排缩进 |
| `J` | **J**oin | 把下一行并进当前行(`3J` 并 3 行) |
| `Ctrl-a` / `Ctrl-x` | add / subtract | 光标处数字 +1 / -1 |

### 4. 文本对象

语法：`d`/`c`/`y` + `i` 或 `a` + 对象
- `i` = **i**nner，只含内容本身
- `a` = **a**round，连引号/括号一起

| 命令 | 拆解 | 含义 |
|---|---|---|
| `ciw` | change inner word | 改掉光标所在的**整个单词** |
| `diw` | delete inner word | 删掉整个单词 |
| `ci"` | change inner quotes | 改掉引号**里面**内容，引号保留 |
| `da"` | delete around quotes | 连引号一起删 |
| `ci(` / `ci{` | change inner | 改掉括号里的内容 |
| `di{` | delete inner | 删掉花括号里的内容 |
| `cit` | change inner tag | 改掉一对 XML/HTML 标签里的内容 |
| `yip` | yank inner paragraph | 复制一整段 |

> **`ciw` 值得刻意练** 改一个变量名、参数值，两下按键搞定，还不依赖光标精度

### 5. 搜索与替换

#### 5.1 搜索

```vim
/pattern          " 向下搜(回车后 n = next 下一个,N 上一个)
?pattern          " 向上搜
*                 " 直接搜光标下的单词
:noh              " no highlight,关掉搜索高亮
```

#### 5.2 替换

基本形式：`:范围s/旧/新/标志`

```vim
:s/foo/bar/          " 当前行第一个 foo
:s/foo/bar/g         " g = global，当前行所有 foo
:%s/foo/bar/g        " % = 整个文件
:%s/foo/bar/gc       " c = confirm，每个都问一下 y/n
:10,20s/foo/bar/g    " 只改第 10-20 行
```

**可视模式选中后按 `:` 会自动补范围** `:'<,'>`：

```vim
:'<,'>s/^/# /        " 给选中每一行加 # 注释
```

其它：

```vim
:%s/\s\+$//e         " 删掉全文行尾空格（e = 没匹配也别报错）
:%s/Vim//gn          " n = number，只统计出现次数，不真替换
:g/^$/d              " 删掉所有空行（g = global，见下）
:g/TODO/d            " 删掉所有含 TODO 的行
```

#### 5.3 批量操作：`global` 和 `normal`

| 命令 | 拆解 | 作用 |
|---|---|---|
| `:g/关键词/d` | global / delete | 按内容删行 |
| `:g/关键词/normal 动作` | global + normal | 按内容改行 |
| `:5,20normal 动作` | normal | 按行号批量做同一个动作 |

```vim
:%normal I#           " 给全文每行行首加 #
:g/sensor/normal A,   " 所有含 sensor 的行，行尾补逗号
:g/^#/normal x        " 所有注释行，删掉第一个字符
```

分工记法：**`global` 负责"挑行"(按内容)，`normal` 负责"对行做什么"(按普通模式)**  
组合起来 = 按内容筛选 + 任意编辑

> 和 `.`、宏配合有奇效——**只需手工做对一次**：先手动改一行确认结果对，再 `:%normal .` 全文重复

### 6. 撤销、重做与「.」万能重复

| 键 | 全称 | 作用 |
|---|---|---|
| `u` | **u**ndo | 撤销 |
| `Ctrl-r` | **r**edo | 重做(取消撤销) |
| `.` | repeat | **重复上一次修改操作**  |

**`.` 的用法**：做了"删一行"后，`j.j.j.j.` 就是往下逐行删  
做了"改一个值"后，`n.n.n.` 就是找到下一个匹配就改一样的——**改名、改常量、批量修正参数的首选**

### 7. 可视模式(批量改多行)

| 键 | 进入哪种可视模式 |
|---|---|
| `v` | 按字符选 |
| `V` | 按行选 |
| `Ctrl-v` | **块选择(矩形)** |

选中后：`d` 删、`y` 复制、`>` `<` 缩进、`:` 进入命令行(自动带范围)

>例：给 20 行加注释
>1. 光标放到第一行行首
>2. `Ctrl-v` 进入块选
>3. `20j` 往下拉出纵向块
>4. `I`（大写 i）→ 输入 `# ` → 按 `Esc`
>5. **所有选中行同时加上了 `# `**

### 8. 寄存器与系统剪贴板

| 寄存器 | 装什么 |
|---|---|
| `""` | 默认寄存器(`y`/`d` 都往这放) |
| `"a`~`"z` | 你指定的命名寄存器(大写 `"A` = **追加**，不覆盖) |
| `"+` | **系统剪贴板**(`"+y` 复制出去、`"+p` 粘进来) |
| `"_` | 黑洞(删进去就没了，不污染默认寄存器) |

用法：先敲 `"` 再敲寄存器名再敲操作

```
"+yy        复制当前行到系统剪贴板
"+p         把系统剪贴板内容粘进来
"ayy        复制到 a 抽屉        "ap  从 a 抽屉粘
"_dd        删掉这行,但不覆盖你刚复制的东西
```

### 9. 宏

场景：把 40 行 `key = value` 改成 YAML 的 `key: value`

```
qa          " 开始录制，存到寄存器 a
(做一次完整操作：比如 0 f= r: 然后 j)
q           " 停止录制
@a          " 播放一次
40@a        " 播放 40 次
@@          " 重复上一次播放
```

**关键技巧——让宏"对连续重放友好"**：宏 = 【编辑动作】 + 【跳到下一个要编辑的位置】

```
qa
I"          " 行首插入 "
Esc
A"          " 行尾插入 "
Esc
j           " ← 关键：结尾顺手跳到下一行
q
```

### 10. 保存、退出与急救

| 命令 | 全称 | 作用 |
|---|---|---|
| `:w` | **w**rite | 保存 |
| `:q` | **q**uit | 退出(有未保存改动会拒绝) |
| `:wq` / `:x` | write+quit | 保存并退出(`:x` 没改动就不写盘) |
| `ZZ` | — | 普通模式直接保存退出 |
| `:q!` / `ZQ` | quit 强制 | 丢弃改动强制退出 |
| `:w !sudo tee %` | — | 忘加 sudo 时用 root 保存 |

| 症状 | 原因 | 解决 |
|---|---|---|
| 打字打不进去 / 按 `j` 一直跳 | 在普通模式 | 按 `i` 再打字 |
| 打出一堆乱字母 | 以为在插入模式其实是普通模式 | `Esc` → `u` 撤销 → 按 `i` |
| **退不出来** | 有未保存改动，`:q` 被拒 | `:wq`(保存退出)或 `:q!`(丢弃退出) |
| 底部显示 `:` 或 `/` 卡住 | 进了命令行/搜索模式 | `Esc` |
| 按 `Ctrl-s` 后终端像死机 | 终端流控被挂起 | 按 **`Ctrl-q`** 恢复 |
| 想放弃所有改动重来 | — | `:e!` 从磁盘重新加载 |


### 11. .vimrc

```vim
" ── 基础 ──────────────────────────────
syntax on
filetype plugin indent on

" ── 行号：绝对 + 相对(跳转距离一眼可见)──
set number
set relativenumber

" ── 缩进：ROS 的 YAML / CMakeLists 习惯 2 空格 ──
set tabstop=2
set shiftwidth=2
set expandtab
set autoindent

" ── 搜索 ──────────────────────────────
set hlsearch        " 高亮所有匹配
set incsearch       " 边输入边跳
set ignorecase      " 忽略大小写
set smartcase       " 但输入带大写就区分

" ── 手感 ──────────────────────────────
set backspace=indent,eol,start
set mouse=a                " 允许鼠标点选、滚轮
set nowrap                 " 长行不折行
set cursorline             " 高亮当前行
set showcmd                " 右下角显示未完成命令
set undofile               " 撤销历史跨重启保留

" ── 用 jj 代替 Esc (可选,打字快可能误触发)──
inoremap jj <Esc>
```

创建撤销目录：

```bash
mkdir -p ~/.vim/undo
```

### 12. 速查表

#### 12.1 模式

| 键 | 全称 | 作用 |
|---|---|---|
| `i` | **i**nsert | 光标前开始打字 |
| `a` | **a**ppend | 光标后开始打字 |
| `I` / `A` | Insert/Append 行首/行尾 | 行首 / 行尾打字 |
| `o` / `O` | **o**pen | 下 / 上方新开一行 |
| `Esc` | escape | 回到普通模式 |

#### 12.2 移动

| 键 | 全称 | 作用 |
|---|---|---|
| `h j k l` | — | 左 下 上 右 |
| `w` / `b` / `e` | word / back / end | 下个词首 / 上个词首 / 词尾 |
| `0` / `^` / `$` | — | 行首 / 首个非空 / 行尾 |
| `gg` / `G` | go | 文件首 / 尾 |
| `%` | match | 括号配对跳 |
| `f<char>` / `t<char>` | find / till | 行内找字符 / 停在它前一个 |
| `Ctrl-o` / `Ctrl-i` | — | 回上个 / 下个跳转位置 |

#### 12.3 编辑

| 键 | 全称 | 作用 |
|---|---|---|
| `d` / `c` / `y` | delete / change / yank | 删 / 改 / 复制(后面接范围) |
| `dd` / `yy` / `cc` | — | 删 / 复制 / 改整行 |
| `p` / `P` | paste | 粘到后 / 前 |
| `x` | — | 删一个字符 |
| `u` / `Ctrl-r` | undo / redo | 撤销 / 重做 |
| `.` | repeat | 重复上一次修改 ★ |
| `ciw` | change inner word | 改整个单词 ★ |
| `ci"` / `di{` | inner quotes / braces | 改引号内 / 删花括号内 |
| `>>` / `<<` | indent | 缩进 / 反缩进 |
| `J` | join | 下一行并上来 |
| `Ctrl-a` / `Ctrl-x` | add / subtract | 数字 +1 / -1 |

#### 12.4 搜索替换与批量

| 命令 | 全称 | 作用 |
|---|---|---|
| `/词` → `n` / `N` | search → next | 搜索 / 下一个 / 上一个 |
| `:%s/旧/新/g` | substitute global | 全文替换 |
| `:%s/旧/新/gc` | + confirm | 逐个确认替换 |
| `:%s/词//gn` | + number | 只统计出现次数 |
| `:g/^$/d` | global delete | 删所有空行 |
| `:g/词/d` | global delete | 按内容删行 |
| `:g/词/normal 动作` | — | 按内容改行 |
| `:%normal .` | — | 全文重复刚才那次修改 |

#### 12.5 文件窗口与退出

| 命令 | 全称 | 作用 |
|---|---|---|
| `:e 文件` | **e**dit | 打开文件 |
| `:sp` / `:vsp` | split / vsplit | 水平 / 垂直分屏 |
| `Ctrl-w h/j/k/l` | window | 在窗口间跳 |
| `:w` / `:q` / `:wq` / `:q!` | write / quit | 保存 / 退出 / 存退 / 强制退 |
| `:e!` | edit 强制 | 放弃改动重新加载 |

#### 12.6 救命

| 键 / 命令 | 大白话 |
|---|---|
| `Esc` | 任何时候都先按它 |
| `Ctrl-q` | `Ctrl-s` 卡住终端后恢复 |
| `:set paste` | 粘贴代码前(防止缩进乱) |
| `:noh` | no highlight，关搜索高亮 |

## 三、 Shell

### 0. 基础与执行

#### 0.1 什么是 Shell

```
终端 Terminal  -->  Shell (Bash/Zsh)  -->  程序/系统
  (输入输出)         (解释命令)              (真正执行)     
```

常见的 Shell 有 bash(最主流)、zsh(好看)、sh(老旧)

#### 0.2 `#!/bin/bash`

学名 **shebang**(释伴，即`#!`的读音)  
作用：让系统用bash跑命令

#### 0.3 `chmod`

初学只需记住：`chmod +x`给予程序执行权限

#### 0.4 `/`与`source`

在 Shell 中运行脚本，主要分为“子进程执行”和“当前进程执行”两种流派：

| 执行方式 | 命令示例 | 底层原理 | 变量与环境影响 |
|---|---|---|---|
| **子进程执行** | `./script.sh` <br> `bash script.sh` | 开启一个全新的子 Shell 运行脚本 | **不保留** 脚本内的变量和 `cd` 等操作，脚本结束后失效 |
| **当前进程执行** | `source script.sh` <br> `. script.sh` | 在当前 Shell 中直接逐行执行代码 | **保留** 脚本内的变量、环境修改会直接影响当前终端 |

### 1. 变量与输入输出

#### 1.1 变量的存取

```bash
#!/bin/bash

# ===== 赋值：注意等号两边【不能有空格】! =====
name="装甲板"           # 存字符串
num=100                 # 存数字(其实是字符串)
pi=3.14159              # 存小数(其实也是字符串)

# ===== 取值：用 $ 前缀,或者 ${} 包起来 =====
echo $name                       # 输出：装甲板
echo "变量 num 的值是：${num}"     # 推荐用 ${},边界更清晰

# ===== 为什么要用 ${} =====
echo "$name团队"        # 系统会以为变量叫 name团队，结果为空
echo "${name}团队"      # 正确：输出 装甲板团队
```

> **三个铁律**：
> 1. 等号两边不能有空格(`name = "x"` 是错的!)
> 2. 取值加 `$`(`$name`)
> 3. 字符串最好加引号(`name="x"`)，防止里面有空格时出问题

#### 1.2 环境变量与自定义变量

自定义变量（普通变量）
- **定义方式**：`name="laoba"`
- **作用域**：仅在当前 Shell 进程中有效
- **继承性**：**无**   通过 `bash` 创建的子进程无法读取该变量
- **使用方式**：`echo $name`

环境变量
- **定义方式**：`export name="laoba"`(通过 `export` 将普通变量升级为环境变量)
- **作用域**：当前 Shell 及其所有子进程中均有效
- **继承性**：**有**   子进程(如脚本)可以读取并继承该变量
- **查看方式**：`env` 或 `printenv`
- **经典应用**：`PATH`(定义系统查找命令的路径)、`HOME`(用户主目录)、代理配置 `http_proxy`

#### 1.3 read

相当于C++里面的`cin >>`

#### 1.4 echo 与 printf

echo 默认自动换行
printf 沿用C语言风格，排版整齐

### 2. 命令速查表

#### 2.1 文件与目录操作

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `ls -l` | list | 列出当前目录的详细清单 |
| `cd` | change directory | 切换目录(去哪个房间) |
| `pwd` | print working directory | 显示我现在在哪个目录 |
| `mkdir -p` | make directory | 建文件夹，-p 可以一次建多层 |
| `touch` | touch | 新建空文件，或更新文件的"最后修改时间" |
| `cp -r` | copy | 复制，-r 连文件夹一起复制 |
| `mv` | move | 移动 / 重命名(搬家或改名都靠它) |
| `rm -rf` | remove | 删除，-r 递归 -f 强制(慎用！删了找不回) |
| `find` | find | 按条件满世界找文件 |
| `tar -czf / -xzf` | tape archive | 打包压缩 / 解压(.tar.gz) |
| `cat` | concatenate | 把文件内容全打印出来 |
| `less` | less | 一页一页翻着看大文件 |
| `head -n` | head | 只看开头几行 |
| `tail -n / -f` | tail | 只看结尾几行；-f 实时盯着文件增长(看日志神器) |
| `du -sh` | disk usage | 看某个文件/文件夹占多大地方 |
| `df -h` | disk free | 看磁盘还剩多少空间 |
| `ln -s` | link (symbolic) | 建一个"软链接"(Windows 里叫快捷方式) |
| `file` | file | 看文件到底是啥类型(文本/图片/可执行文件) |
| `stat` | status | 看文件的详细元信息(大小、权限、修改时间) |
| `tree` | tree | 用树状图展示目录结构(可能要先 `apt install tree`) |
| `rmdir` | remove directory | 删空目录(目录里有东西就删不掉，会提示你) |

#### 2.2 文本处理

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `grep` | global regular expression print | 从文本里"搜"出想要的行 |
| `sed` | stream editor | 批量"改"文本(替换、删除、打印指定行) |
| `awk` | Aho， Weinberger & Kernighan(三位作者姓氏) | 按列"切分 + 计算"文本 |
| `sort` | sort | 排序 |
| `uniq` | unique | 去重(常配合 sort 用) |
| `wc -l` | word count | 数行数(-w 数单词，-c 数字节) |
| `cut -d -f` | cut | 按分隔符切列，取指定列 |
| `diff` | difference | 比较两个文件哪里不一样 |
| `tr` | translate | 字符转换 / 删除(比如小写转大写) |
| `xargs` | eXtended ARGuments | 把前一命令的输出，转成后一命令的参数 |
| `tee` | tee(T 形三通管) | 输出一边显示到屏幕，一边存进文件 |
| `paste` | paste | 把两个文件按列并排合并 |
| `strings` | strings | 从二进制文件里"抠"出可读的文本 |

#### 2.3 权限管理

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `chmod +x` | change mode | 给文件加执行权限(发"可执行驾照") |
| `chmod 755` | change mode | 用数字设置权限(7=读写执行，5=读+执行) |
| `chown` | change owner | 把文件的所有者改给别人 |
| `chgrp` | change group | 改文件所属的组 |
| `sudo` | superuser do | 以管理员(root)身份跑命令 |
| `whoami` | who am i | 我是谁(当前用户) |
| `id` | identity | 查自己的用户 ID 和所属组 |
| `passwd` | password | 改自己的密码(root 可改别人的) |
| `umask` | user mask | 设置新建文件/目录的默认权限 |
| `su -` | switch user | 切换到另一个用户(`su -` 切到 root) |

#### 2.4 系统状态查看

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `top` / `htop` | table of processes / Hisham's top | 实时看 CPU、内存、进程占用(任务管理器) |
| `free -h` | free | 看内存还剩多少 |
| `uname -a` | unix name | 看内核版本、系统架构 |
| `lsb_release -a` | Linux Standard Base release | 看 Ubuntu 具体版本号 |
| `uptime` | uptime | 开机多久了 + 平均负载 |
| `dmesg` | display message | 看内核日志(插拔设备、驱动报错都在这) |
| `lscpu` | list cpu | 看 CPU 型号、核数 |
| `who` | who | 看现在谁登录在这台机器上 |
| `date` | date | 看/设置系统时间(`date +%Y%m%d` 取日期很常用) |
| `hostname` | host name | 看主机名(`hostname -I` 直接看本机 IP) |
| `nproc` | number of processors | 看 CPU 有几个核 |
| `lspci` / `lsusb` | list pci / usb | 看 PCI / USB 硬件设备 |
| `vmstat` | virtual memory statistics | 看内存 / CPU / IO 的实时统计 |

#### 2.5 进程管理

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `ps aux` | process status | 列出所有进程的详细快照 |
| `kill <pid>` | kill | 礼貌地让某个进程结束 |
| `kill -9 <pid>` | kill | 强制干掉某个进程 |
| `pkill -f 名字` | process kill | 按名字批量结束进程 |
| `jobs` | jobs | 看后台任务列表 |
| `bg` / `fg` | background / foreground | 把任务放后台 / 拉回前台 |
| `&` | ampersand(后台运行) | 命令末尾加它，让命令在后台跑 |
| `nohup cmd &` | no hang up | 关掉终端也继续跑(挂后台不挂) |
| `systemctl status` | system control | 看某个服务是否在运行 |
| `pgrep -f 名字` | process grep | 按名字查进程的 PID(比 `ps + grep` 更快) |
| `killall 名字` | kill all | 按名字批量结束所有同名进程 |
| `service` | service | 老式服务管理(`service ssh restart`) |

#### 2.6 网络与传输

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `ping` | Packet InterNet Groper | 测到某台机器通不通、延迟多少 |
| `ifconfig` / `ip addr` | interface configuration / internet protocol | 看本机 IP 地址 |
| `ss -tlnp` | socket statistics | 看哪些端口正在监听 |
| `netstat` | network statistics | 看网络连接情况(老命令，逐渐被 ss 取代) |
| `curl` | client URL | 命令行发 HTTP 请求、下载文件 |
| `wget` | world wide web get | 下载文件(比 curl 简单粗暴) |
| `scp` | secure copy | 两台机器之间安全传文件 |
| `rsync` | remote sync | 高效同步文件/目录(可断点续传) |
| `ssh` | secure shell | 远程登录到另一台 Linux 机器 |
| `nmap` | network mapper | 扫描目标主机开放了哪些端口 |
| `dig` | domain information groper | 查域名解析(DNS)，比 nslookup 好用 |
| `traceroute` | trace route | 看数据包到目标机器一路经过了哪些节点 |
| `nc` | netcat | 测端口、传数据、简易聊天 |

#### 2.7 压缩与打包

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `gzip 文件` | gnu zip | 压缩成 `.gz`(原文件会被替换掉) |
| `gunzip 文件.gz` | gnu unzip | 解压 `.gz` |
| `bzip2 文件` | block zip | 压缩成 `.bz2`(比 gzip 更小，但更慢) |
| `bunzip2` | block unzip | 解压 `.bz2` |
| `xz 文件` | xz | 压缩成 `.xz`(压缩率最高) |
| `zip -r x.zip 目录` | zip | 打包压缩成 `.zip`(Windows 通用) |
| `unzip x.zip` | unzip | 解压 `.zip` |
| `tar -czf` / `tar -xzf` | tape archive | 打包压缩 / 解压 `.tar.gz` |

> `tar.gz` 是 Linux 主流；`zip` 是跨平台通用；`gzip`/`bzip2`/`xz` 只能压缩**单个文件**，多文件得先用 `tar` 打包

#### 2.8 用户与组管理

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `useradd 名字` | user add | 新建一个用户 |
| `userdel -r 名字` | user delete | 删除用户，-r 连家目录一起删 |
| `usermod -aG 组 用户` | user modify | 把用户追加到某个组(-aG 追加，别漏 a) |
| `groupadd 组名` | group add | 新建一个用户组 |
| `groupdel 组名` | group delete | 删除用户组 |
| `passwd 名字` | password | 设置 / 修改用户密码 |
| `su - 用户` | switch user | 切换到别的用户(`su -` 切到 root) |
| `sudo 命令` | superuser do | 临时以管理员身份执行一条命令 |

#### 2.9 帮助与命令查找

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `man 命令` | manual | 命令的官方说明书(`q` 退出，`/` 搜索) |
| `命令 --help` | help | 命令自带的简明帮助(比 man 短) |
| `help 命令` | help | 查 bash 内建命令的帮助(如 `help cd`) |
| `whatis 命令` | what is | 一句话说清这个命令是干嘛的 |
| `apropos 关键词` | apropos | 按关键词反查命令(忘了名字用它) |
| `which 命令` | which | 这个命令的"真身"在哪个路径 |
| `whereis 命令` | where is | 命令本体 + 手册 + 源码都在哪 |
| `type 命令` | type | 看命令是内建 / 别名 / 外部程序 |

#### 2.10 其他工具

| 命令 | 英文全称 | 作用 |
|---|---|---|
| `alias` | alias | 给命令起"小名"(`alias ll='ls -l'`) |
| `unalias` | un-alias | 取消别名 |
| `history` | history | 看敲过的历史命令(`!数字` 重复执行某条) |
| `sleep 秒数` | sleep | 让脚本"睡"一会儿再继续 |
| `watch 命令` | watch | 每隔几秒自动重复执行一条命令(盯监控用) |
| `seq 1 10` | sequence | 生成一串数字序列 |
| `date` | date | 看 / 设时间(`date +%Y%m%d` 很常用) |
| `clear` | clear | 清空屏幕(或按 Ctrl+L) |
| `exit` | exit | 退出当前终端 / 脚本 |
| `source 文件` | source | 在当前进程执行文件(`source` = `.`，见 1.4) |

### 3. 选项与参数速查表

#### 3.1 文件与目录类

| 命令 | 常用选项 / 参数 | 说明 |
|---|---|---|
| `ls` | `-l` | 详细列表(权限、大小、时间) |
| `ls` | `-a` | 连隐藏文件(`.` 开头)一起列 |
| `ls` | `-h` | 大小转成人话(K / M / G) |
| `ls` | `-t` / `-r` | 按时间排序 / 倒序 |
| `ls` | `-S` | 按文件大小排序 |
| `cd` | `~` / `-` / `..` | 家目录 / 上一个目录 / 上级目录 |
| `mkdir` | `-p` | 一次建多层目录(父目录不存在也自动建) |
| `cp` | `-r` | 递归复制(复制整个目录必须加) |
| `cp` | `-i` | 覆盖前先问一句 |
| `cp` | `-u` | 只复制"更新"的文件(增量备份) |
| `mv` | `-i` | 覆盖前先问一句 |
| `rm` | `-r` | 递归删除目录 |
| `rm` | `-f` | 强制删除，不提示 |
| `touch` | `-c` | 文件不存在也不创建 |
| `cat` | `-n` / `-b` | 显示行号 / 只给非空行编号 |
| `head` / `tail` | `-n 行数` | 看开头 / 结尾几行(默认 10 行) |
| `tail` | `-f` | 实时跟踪文件增长(看日志神器) |
| `ln` | `-s` | 创建软链接(符号链接) |
| `stat` | `-c 格式` | 按格式输出元信息(`stat -c %s 文件` 看大小) |

#### 3.2 查找与压缩类

| 命令 | 常用选项 / 参数 | 说明 |
|---|---|---|
| `find` | `-name "模式"` | 按文件名找(支持通配符) |
| `find` | `-iname "模式"` | 同 `-name`，忽略大小写 |
| `find` | `-type f` / `-type d` | 只找文件 / 只找目录 |
| `find` | `-size +10M` | 按大小找(`+` 大于，`-` 小于) |
| `find` | `-mtime -7` | 按修改时间找(`-7` = 7 天内) |
| `find` | `-maxdepth 2` | 限制搜索深度(别挖太深) |
| `find` | `-exec ... {} \;` | 对找到的每个文件执行命令 |
| `tar` | `-c` / `-x` | 创建归档 / 解出归档 |
| `tar` | `-z` / `-j` | 用 gzip / bzip2 压缩 |
| `tar` | `-f 文件` | 指定归档文件名 |
| `tar` | `-v` | 显示处理过程 |
| `tar` | `-t` | 只查看归档里有什么，不解压 |
| `tar` | `-C 目录` | 解压到指定目录 |

#### 3.3 文本处理类

| 命令 | 常用选项 / 参数 | 说明 |
|---|---|---|
| `grep` | `-i` | 忽略大小写 |
| `grep` | `-n` | 显示行号 |
| `grep` | `-v` | 反向匹配(找不含关键词的) |
| `grep` | `-c` | 只统计匹配的行数 |
| `grep` | `-r` | 递归搜索整个目录 |
| `grep` | `-l` | 只列出"有匹配"的文件名 |
| `grep` | `-w` | 整词匹配(不会把 abc 匹配成 abcdef) |
| `grep` | `-o` | 只输出匹配到的部分 |
| `grep` | `-E` | 支持扩展正则(`|`、`+` 等) |
| `grep` | `-A/-B/-C 行数` | 同时显示匹配行后 / 前 / 前后各几行(上下文) |
| `sed` | `-i` | 原地修改文件(不加它只打印不落盘) |
| `sed` | `-n` | 安静模式，配合 `p` 只打印指定行 |
| `sed` | `-e` | 一条命令里执行多个 sed 操作 |
| `sed` | `-r` / `-E` | 使用扩展正则 |
| `awk` | `-F 分隔符` | 指定列分隔符(默认空格 / Tab) |
| `awk` | `-v 变量=值` | 从外部传入变量 |
| `sort` | `-n` | 按数字排序(不加会按字符串排) |
| `sort` | `-r` | 倒序 |
| `sort` | `-k 列` | 按第几列排序(`-k3` 第 3 列) |
| `sort` | `-t 分隔符` | 指定列分隔符 |
| `sort` | `-u` | 排序并去重 |
| `uniq` | `-c` | 统计每行出现次数 |
| `uniq` | `-d` / `-u` | 只显示重复的 / 只显示不重复的 |
| `wc` | `-l` / `-w` / `-c` | 数行 / 数单词 / 数字节 |
| `cut` | `-d 分隔符` | 指定分隔符 |
| `cut` | `-f 列` | 取第几列(`-f1,3` 取 1、3 列) |
| `cut` | `-c 位置` | 按字符位置切(`-c1-5` 取前 5 个字符) |

#### 3.4 权限 / 进程 / 系统类

| 命令 | 常用选项 / 参数 | 说明 |
|---|---|---|
| `chmod` | `-R` | 递归改整个目录下所有文件 |
| `chmod` | `+x` / `-x` | 加 / 减执行权限 |
| `chmod` | `755` / `644` | 数字设权限 |
| `chown` | `-R 用户:组 文件` | 递归改所有者(冒号后是组) |
| `ps` | `aux` | 列出所有进程(BSD 风格，最常用) |
| `ps` | `-ef` | 列出所有进程(System V 风格) |
| `kill` | `-9` | 强制杀进程(不商量) |
| `kill` | `-15` | 温和终止(默认，给进程留收拾的时间) |
| `kill` | `-l` | 列出所有信号 |
| `top` | `-p 进程号` | 只盯着某个进程看 |
| `df` | `-h` | 磁盘空间转成人话 |
| `du` | `-sh` | 汇总目录总大小(`-s` 汇总，`-h` 人性化) |
| `free` | `-h` | 内存转成人话 |
| `uname` | `-a` / `-r` | 全部信息 / 只看内核版本 |

#### 3.5 网络 / 服务类

| 命令 | 常用选项 / 参数 | 说明 |
|---|---|---|
| `ssh` | `-p 端口` | 指定端口(默认 22) |
| `ssh` | `-i 密钥文件` | 用密钥登录(免密登录) |
| `ssh` | `-X` | 图形界面转发 |
| `scp` | `-r` | 递归传整个目录 |
| `scp` | `-P 端口` | 指定端口(注意是大写 P) |
| `curl` | `-o 文件` | 下载并保存为指定文件名 |
| `curl` | `-O` | 按服务器上的原名保存 |
| `curl` | `-L` | 自动跟随重定向 |
| `curl` | `-s` | 静默模式，不显示进度 |
| `curl` | `-I` | 只看响应头(测接口用) |
| `curl` | `-X POST -d 数据` | 指定请求方法 + 发送数据 |
| `wget` | `-O 文件` | 保存为指定文件名 |
| `wget` | `-c` | 断点续传 |
| `ss` | `-t` / `-u` / `-l` | TCP / UDP / 只显示监听中的 |
| `ss` | `-n` / `-p` | 不解析域名 / 显示进程名 |
| `systemctl` | `start/stop/restart` | 启动 / 停止 / 重启服务 |
| `systemctl` | `status` | 看服务状态 |
| `systemctl` | `enable/disable` | 开机自启 / 取消自启 |

#### 3.6 Shell 特殊变量速查

| 变量 | 含义 | 大白话 |
|---|---|---|
| `$0` | 脚本名 | 脚本自己叫啥 |
| `$1` ~ `$9` | 第 1~9 个参数 | 你递进去的第几个东西 |
| `${10}` | 第 10 个参数 | 两位以上必须加 `{}` |
| `$#` | 参数个数 | 一共递了几个 |
| `$@` | 所有参数(各自独立) | 一个个的清单(遍历用) |
| `$*` | 所有参数(合成一个) | 全部捆成一坨 |
| `$?` | 上一条命令退出码 | 0 成功，非 0 失败 |
| `$$` | 当前进程 PID | 这个脚本的"工号" |
| `$!` | 最近后台进程 PID | 最后放到后台那位的"工号" |
| `$-` | 当前 shell 选项 | 如 `set -x` 的状态 |

#### 3.7 printf 格式符速查

| 格式符 | 含义 | 示例 |
|---|---|---|
| `%d` | 整数 | `printf "%d" 42` → 42 |
| `%s` | 字符串 | `printf "%s" "hi"` → hi |
| `%f` | 浮点数 | `printf "%.2f" 3.14159` → 3.14 |
| `%c` | 单个字符 | `printf "%c" 65` → A |
| `%x` / `%o` | 十六进制 / 八进制 | `printf "%x" 255` → ff |
| `%%` | 百分号本身 | `printf "100%%"` → 100% |
| `%10s` | 右对齐，占 10 格 | `printf "%10s" "ab"` |
| `%-10s` | 左对齐，占 10 格 | `printf "%-10s" "ab"` |
| `%03d` | 整数补零到 3 位 | `printf "%03d" 5` → 005 |
| `%.2f` | 保留 2 位小数 | `printf "%.2f" 3.14159` → 3.14 |

#### 3.8 权限数字对照表(chmod)

| 数字 | 权限 | 含义 |
|---|---|---|
| 4 | `r` | 读 read |
| 2 | `w` | 写 write |
| 1 | `x` | 执行 execute |
| 0 | `---` | 啥都不能干 |
| 7 | `rwx` | 4+2+1，读写执行全有 |
| 6 | `rw-` | 4+2，可读可写 |
| 5 | `r-x` | 4+1，可读可执行 |
| 4 | `r--` | 只读 |

> 权限是**三位一组**：`chmod 754 文件` 里的 `7` 是所有者(user)、`5` 是所属组(group)、`4` 是其他人(other)
> **常用组合**：`755`(可执行程序 / 目录)、`644`(普通文件)、`777`(谁都能读写执行，慎用)

## 四、 Git && Github

### 0. 概念

```
工作区 ──git add──► 暂存区 ──git commit──► 本地仓库 ──git push──► GitHub 远程仓库
正在改的文件     本次提交清单         本地提交历史           云端提交历史
```

|-|概念|作用|
|---|---|---|
| **Git** | 装在本机的**工具**(命令行、VS Code 调用) | 记录提交、比较修改、开分支 |
| **GitHub** | Git 仓库的云端 | 存仓库、开 PR 评审、合并团队修改 |

### 1. 配置SSH

---**生成密钥**---

```bash
ssh-keygen -t ed25519 -C "你的邮箱"
```
> 参数：
> - `-t` = **type**，密钥类型。`ed25519`是推荐类型(比老式RSA更快更短)
> - `-C` = **comment**，注释(通常写邮箱)，方便在Github上认出钥匙所属者

交互过程(三段，前两段直接回车即可)：

```
Enter file in which to save the key (/home/你/.ssh/id_ed25519):   ← 回车,用默认路径
Enter passphrase (empty for no passphrase):                       ← 回车 = 不设密码
Enter same passphrase again:                                      ← 再回车确认
```

生成后得到两个文件：

| 文件 | 是什么 | 能不能给别人 |
|---|---|---|
| `~/.ssh/id_ed25519` | 私钥(钥匙) | ❌ 绝不能 |
| `~/.ssh/id_ed25519.pub` | 公钥(锁) | ✅ 可以 |

查看公钥：

```bash
cat ~/.ssh/id_ed25519.pub
# 输出一行,形如：
# ssh-ed25519 AAAAC3NzaC1l...xxxx 你的邮箱
```

---**将公钥添加至Github**---

1. 打开 **https://github.com/settings/ssh/new**
2. **Title** 随便填
3. **Key type** 选 `Authentication Key`
4. **Key** 把上一步 `cat` 输出的**一整行**粘进去
5. 点 **Add SSH key**(可能要输一次 GitHub 密码确认)

---**测试连接**---

```bash
ssh -T git@github.com
```

`-T` = **no TTY**(不分配终端。因为 GitHub 不提供 shell，这里只做认证，`-T` 避免无效操作)

首次连接会问：

```
The authenticity of host 'github.com (20.205.243.166)' can't be established.
ED25519 key fingerprint is SHA256:+DiY3wvvV6TuJJhbpZisF/zLDA0zPMSvHdkr4UvCOqU.
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```

看清楚指纹后输 `yes` 回车——把 GitHub 的指纹记进 `~/.ssh/known_hosts`，以后不会再问

成功输出：

```
Hi 你的用户名! You've successfully authenticated, but GitHub does not provide shell access.
```

> `does not provide shell access` 是正常的，GitHub 本来就不允许你登录它的 shell

### 2. 命令

#### 2.1 `git status` -- 看状态

```bash
git status          # 完整版
git status -sb      # -s = short(简略)+ -b = branch(带分支信息)
```

输出解读：

```
## main...origin/main [ahead 1]          ← 本地比远程多 1 个提交
 M src/main.cpp                          ← 已修改,未暂存
A  docs/git-usage.md                     ← 新增,已暂存
?? notes.txt                             ← 未跟踪(Git 从来没见过它)
```

两列的含义(左=暂存区状态，右=工作区状态)：

| 符号 | 含义 |
|---|---|
| `M ` | 已暂存的修改 |
| ` M` | 未暂存的修改 |
| `MM` | 暂存后又改了(暂存的是旧版本) |
| `A ` | 新文件已暂存(**A**dded) |
| `D ` | 已暂存的删除(**D**eleted) |
| `??` | 未跟踪(Untracked) |
| `UU` | 合并冲突，等你解决 |

#### 2.2 `git add` -- 加入暂存区

```bash
git add file.txt            # 一个文件
git add src/                # 一个目录(递归)
git add .                   # 当前目录所有变化
git add -A                  # -A = All,整个仓库所有变化(含删除)
git add -p                  # -p = patch,逐块选择要暂存的部分(神器)
```

#### 2.3 `git commit` -- 提交

```bash
git commit -m "修复里程计的时间戳漂移"           # -m = message(提交信息)
git commit                                   # 打开编辑器写多行信息(默认vim)
git commit -am "信息"                         # -a = all(跳过 add,但只对已跟踪文件生效)+ -m
git commit --amend                           # --amend = 修正,改写最后一次提交
```

#### 2.4 `git log` -- 看历史

```bash
git log                              # 完整
git log --oneline                    # --oneline = 一行一条
git log --oneline --graph --all      # --graph=分支图 --all=所有分支(建议配成 git lg)
git log -3                           # 最近 3 条
git log -p                           # -p = patch,带每次的 diff
git log --stat                       # 只看改了哪些文件 + 增删行数
git log --author="你的名字"           # 按人过滤
git log --oneline -- src/odom.cpp    # 只看某个文件的历史
git log -S "foo(" --oneline          # -S = 搜"哪个提交引入了/删掉了这串内容"
git show 3fa1c8e                     # 看某个提交具体改了什么
```

#### 2.5 `git diff` -- 看未提交的改动

```bash
git diff                  # 工作区 vs 暂存区(还没 add 的)
git diff --staged         # --staged = 暂存区 vs 上次提交(马上要提交的)
git diff HEAD             # 工作区 vs 上次提交(全部未提交改动)
git diff main dev         # 两个分支比
git diff --stat           # 只看统计
```
#### 2.6 `git push`与`git pull`

```bash
git push                                  # 推到已绑定的上游
git push -u origin main                   # -u = set-upstream,第一次推并绑定上游
git push origin dev                       # 推到指定分支

git pull                                  # 拉取 + 合并(= fetch + merge)
git pull --rebase                         # --rebase = 变基,拉取 + 把提交排成一条线
git fetch                                 # 只下载,不动你的工作区(安全)
```

`fetch` vs `pull`：

- `git fetch`：把远程的新东西**下载**到本地，但**不改你的工作区和分支**。之后可以 `git log HEAD..origin/main` 查看别人改的内容
- `git pull`：`fetch` + **立刻合并**进当前分支。**容易冲突**

#### 2.7 `git branch`与`git switch` -- 分支

```bash
git branch                      # 列出本地分支(* 是当前分支)
git branch -a                   # -a = all,含远程分支
git branch dev                  # 创建分支(不切换)
git switch dev                  # 切换分支(新命令,推荐)
git switch -c feature/odom      # -c = create,创建 + 切换
git switch -                    # 切回上一个分支(像 cd -)
git branch -d dev               # -d = delete,删除已合并的分支
git branch -D dev               # 强制删除(未合并也删)
git branch -m old new           # -m = move,重命名分支
```

> `git checkout` 是旧命令，既能切分支又能还原文件，容易搞混。**新代码用 `git switch`(切分支)和 `git restore`(还原文件)**，语义清晰

#### 2.8 `git merge` -- 合并

```bash
git switch main
git merge dev                   # 把 dev 合进 main
git merge --no-ff dev           # 强制生成合并提交(保留"曾经是分支"的信息)
git merge --abort               # 冲突搞不定,放弃整个合并
```

两种结果：

- **快进合并(Fast-forward)**：`main` 没自己的新提交，指针直接挪过去，不产生合并提交。
- **三方合并(3-way merge)**：两边都有新提交，Git 生成一个"合并提交"，可能冲突

#### 2.9 撤销与回退

##### 2.9.1 `git reset`

`git reset` 移动的是"分支指针 + 暂存区 + 工作区"三者中的一部分：

| 模式 | 分支指针 | 暂存区 | 工作区 | 典型用途 |
|---|---|---|---|---|
| `--soft` | 移动 | 不动 | 不动 | 提交错了，想拆成两个提交重新组织 |
| `--mixed`(默认) | 移动 | **重置** | 不动 | 提交错了，改动全部退回工作区重来 |
| `--hard` | 移动 | 重置 | **重置** | 彻底不要这些提交和改动 |

```bash
git reset --soft HEAD~1     # 撤销 1 次提交,改动还在暂存区
git reset HEAD~1            # 撤销 1 次提交,改动回到工作区(未暂存)
git reset --hard HEAD~1     # 撤销 1 次提交并删掉改动 ← 想清楚再敲
git reset --hard origin/main  # 本地完全对齐远程(本地改动全丢)
```

> `HEAD~1` 表示"当前提交的父提交"，`HEAD~3` 就是往前三个。也可以用哈希

##### 2.9.2 `git revert` -- 改已推送的提交

`reset` 会**改写历史**。一旦别人拉过那些提交，改写会让他们的仓库出问题。

`revert` 不删任何东西，而是**生成一个新提交，内容正好抵消旧提交**：

```bash
git revert 3fa1c8e           # 撤销这一个提交
git revert HEAD              # 撤销最后一次提交
```

#### 2.10 `git stash` -- 暂存改动

```bash
git stash                       # 把当前改动收起来,工作区变干净
git stash -u                    # -u = include-untracked,连未跟踪的新文件一起收
git stash list                  # 看收了几个
git stash pop                   # 取回最近一个并删掉记录
git stash apply stash@{1}       # 取回指定的,但保留记录
```

#### 2.11 `git reflog` -- 引用日志(Reference flog)

> reset后悔可用reflog补救

```bash
git reflog
# 3fa1c8e HEAD@{0}: reset: moving to HEAD~1
# 9b2d0f1 HEAD@{1}: commit: XXXXXXX说明     ← 删去的内容

git switch -c rescue 9b2d0f1        # 从那个提交拉一个新分支出来
# 或直接回到那个状态
git reset --hard 9b2d0f1
```

> ⚠️ Git 默认只会保留 90 天的 reflog 记录

### 3. 与VS Code

| VS Code 按钮 | 实际执行的命令 | 备注 |
|---|---|---|
| 暂存更改(文件旁 `+`) | `git add <file>` | — |
| 暂存所有更改 | `git add -A` | — |
| 放弃更改 | `git restore <file>` | ⚠️ 真的丢掉未提交修改，不可撤销 |
| 提交(✓) | `git commit` | 只提交已暂存的内容 |
| 拉取 | `git pull` | — |
| 推送 | `git push` | — |
| **同步更改** | `git pull` **然后** `git push` | ⚠️ 见下 |
| 贮藏 | `git stash` | 把改动临时收起来 |

> ⚠️「同步更改」的陷阱
>
> 第一步`git pull`——把远程分支强行合并到本地。所以任何"只改本地、远程不动"的操作(`reset`、`revert` 后不推)，点一次「同步更改」，就会被远程历史还原回来
>
> 想回退历史，本地和远程要一起处理(`git reset` + `git push --force-with-lease`)

### 4. Fork 与 PR

```
① Fork      GitHub 上点 "Fork",把团队仓库复制到你名下
② Clone     把你名下的这份下载到本地:  git clone git@github.com:用户名/仓库.git
③ 分支+改   git switch -c feature/xxx  然后改代码
④ 提交+推送 git add → git commit → git push -u origin feature/xxx
⑤ 发 PR     GitHub 上发起 Pull Request,请求合回团队仓库
⑥ 评审+合并 队友 review;要改就继续 commit+push,原 PR 自动更新;通过后 merge
```

要点：

- **`origin` 是你的 Fork，不是团队仓库**。`git push` 推到你自己名下的副本，再靠 PR 请求合回团队仓库
- **评审要求改**：不用重发 PR，在同一个分支继续 `commit` + `push`，原 PR 会自动更新
- **Fork 落后了**：在你 Fork 的 GitHub 页面点 "Sync fork"(或本地把团队仓库加成一个 remote 再拉)
- **有团队仓库推送权限**时，可以省掉 Fork，直接在同仓库开分支、发 PR

> 一次贡献的完整闭环：**Linux 进环境 → Shell 定位文件 → Vim 改内容 → Git 分支/暂存/提交 → GitHub 发 PR 评审合并**
> push 要免密需要配 SSH

### 5. .gitignore

在仓库根目录建 `.gitignore`，写进去的路径 Git 就不再理它：

```gitignore
# 忽略整个目录
build/

# 忽略某类文件
*.log
*.pyc
__pycache__/

# 忽略某个具体文件
secrets.env

# 忽略但保留例外(! 是取反)
build/
!build/keep.txt

# 双星号表示任意层级
**/node_modules/
```

`.gitignore` 对**已经提交过**的文件无效。要让它生效，得先从索引里移除：

```bash
git rm --cached secret.txt      # 从 Git 里删掉,但保留磁盘上的文件
git rm -r --cached build/       # 目录同理(-r = recursive 递归)
git commit -m "停止跟踪 secret.txt"
```

```bash
git check-ignore -v secret.txt   # -v = verbose,可查哪行规则使此文件被忽略
```

### 6. 大文件与 Git LFS

Git 不适合存大文件：每次 `clone` 需要 push **整个历史**，一个大模型权重改一版就翻一倍，仓库会迅速膨胀  
所以大二进制文件(模型权重、数据集、CAD/STL、ROS bag、视频)不该直接 `git add`

**LFS(Large File Storage，大文件存储)**：大文件不存进 Git 历史，只存一个**指针**(几十字节的小文本文件)，真正的文件放在 LFS 服务器上
这样 `git clone` 拉的是指针，仓库永远轻

```bash
sudo apt install git-lfs        # 装 LFS
git lfs install                 # 在 ~/.gitconfig 里挂上 LFS 的钩子(每个用户一次)
```

## 五、 CPP

[C++ 前期学习笔记](https://github.com/PFlaoba/Git-Testing/blob/main/MyNote.pdf)

## 总结

第1课记录了Linux Shell Git Vim等知识
学会用 Markdown 记笔记
有待进步