# Lesson1:从Linux到第一次Pull Request

## 一个流程：进入Linux-->终端操作-->Vim编辑-->Git提交-->GitHub pr

### 1.linux基础：

(1)从/开始：

　　/home/user   用户文件
　　/etc   系统配置
　　/usr   程序与库
　　/var   日志等可变数据
　　/dev   设备文件

　　~  家目录
　　.  当前目录
　　.. 上一级目录

　　绝对路径  从/开始         /home/user/example
　　相对路径  从当前位置开始   ./hello.txt

　　注意：linux区分大小写，README.md 和 readme.md 是两个文件


(2)终端、shell与命令：在终端输入命令，由shell解释，再由程序执行

　　Terminal终端 --> Shell(Bash/Zsh) --> 程序/系统
　　输入输出界面    解释命令、展开路径      ls、git这些

　　cd一般是shell内建的命令


(3)shell命令结构：命令 [选项] [参数]

　　ls -la ~/projects

　　命令    ls          做什么
　　选项    -la         怎样执行，-l详细信息，-a含隐藏项
　　参数    ~/projects  操作哪个对象


(4)当前位置与目录切换：

　　pwd       显示当前目录
　　ls        列出目录内容
　　cd 目录    进入指定目录
　　cd ..      返回上一级
　　cd ~       返回家目录

　　例子：pwd -> cd example -> cd .. -> cd ~

　　cd执行成功也可能没输出，这是正常的
　　相对路径以当前位置为起点
　　Tab键自动补全目录名


(5)文件与目录操作：

　　mkdir -p   创建目录，可补齐父目录    mkdir -p ~/linux_demo
　　touch      创建空文件/更新时间       touch hello.txt
　　cp         复制文件                 cp hello.txt hello-copy.txt
　　mv         移动或重命名             mv hello-copy.txt backup.txt
　　rm         删除文件                 rm backup.txt
　　cat/less   直接查看/分页查看         cat hello.txt

　　**注意：rm通常不进回收站，删之前确认路径和文件名
　　less里按q退出**


(6)shell四种常用组合：

　　1.变量
　　　　name="RM"
　　　　echo "$name"
　　　　等号两边不能有空格

　　2.重定向
　　　　echo "hello" > hello.txt     覆盖写入
　　　　echo "world" >> hello.txt    追加到末尾

　　3.管道
　　　　ls -la | grep txt
　　　　把前一条的输出交给下一个程序，grep txt是文本筛选

　　4.条件连接
　　　　mkdir -p demo && cd demo
　　　　前一条成功后，再执行下一条


(7)陌生命令帮助入口：

　　ls --help    快速查看选项和用法
　　man ls       打开手册，按q退出
　　Tab          补全命令或路径
　　↑ / ↓        调出历史命令

　　--help是惯例，不是所有命令都有


### 2.Vim：在终端中编辑

1.模式与必备按键：

　　三种模式：
　　普通模式    移动、删除、复制（默认）
　　插入模式    输入文本
　　命令行模式  保存、退出

　　普通模式 --按i--> 插入模式 --按Esc--> 普通模式
　　普通模式 --按:--> 命令行模式 --回车--> 普通模式

　　普通模式按键：
　　h j k l   左 下 上 右
　　dd        删除当前行
　　yy / p    复制当前行 / 粘贴
　　u         撤销
　　/word     搜索word

　　命令行模式：
　　:w    保存
　　:q    退出
　　:wq   保存退出
　　:q!   放弃修改退出

　　卡住先按Esc
　　:q有未保存修改时会拒绝退出


2.基本使用：

　　1  vim hello.txt    打开练习文件
　　2  按i              进入插入模式，输入文本
　　3  按Esc            返回普通模式
　　4  :wq              保存并退出
　　5  vim hello.txt    再次打开
　　6  dd / u / /word   删除、撤销、搜索


### 3.Git&GitHub

1.两者分工：

　　Git      本地版本控制工具
　　　　　　 记录每次提交、比较修改、查看历史、创建分支

　　GitHub   远程托管与协作平台
　　　　　　 托管Git仓库、通过PR讨论评审、合并修改

　　Git是工具，GitHub是平台
　　没联网也能本地提交，推送才需要远程仓库和认证


2.Git四位置：

　　工作区 --git add--> 暂存区 --git commit--> 本地仓库 --git push--> GitHub远程仓库
　　正在编辑             本次提交              已记录的            远程保存的
　　的文件               内容清单              提交历史            提交历史

　　注意：commit只记录到本地，push才发送到远程
　　git add后又改了文件，要再add一次


3.团队使用：

　　团队仓库 --Fork--> 个人Fork --Clone--> 本地电脑
　　本地电脑 --创建feature分支--> 修改/add/commit --> push
　　push到个人Fork --发起PR--> Review --> Merge到团队仓库

　　有推送权限时可以省略Fork，直接用同仓库分支
　　评审要求修改时，继续往相同分支提交推送，原PR会更新


4.SSH：

　　ssh-keygen -t ed25519 -C "you@example.com"    生成密钥
　　cat ~/.ssh/id_ed25519.pub                     查看公钥

　　公钥  (.pub)   复制到GitHub的SSH设置
　　私钥  id_ed25519       不上传、不截图、不分享

5.公钥添加：

　　GitHub里：Settings --> SSH and GPG keys --> New SSH key
　　选Authentication Key，填名称，粘贴完整公钥

　　测试：ssh -T git@github.com
　　首次连接先核对GitHub官方公布的SSH主机指纹
　　提示"不提供shell访问"不代表失败


6.Git常用命令：

　　首次配置：
　　git config --global user.name "Your Name"
　　git config --global user.email "you@example.com"
　　姓名和邮箱会写进提交记录
　　这个配置不等于登录GitHub

　　获取与查看：
　　git clone <仓库地址>    下载仓库及提交历史
　　git status             查看当前分支和文件状态
　　git diff               查看未暂存的修改
　　git log --oneline      简短格式查看提交历史

　　分支与提交：
　　git switch -c feature/hello       创建并切换功能分支
　　git add hello.txt                 暂存指定文件
　　git commit -m "Add greeting"      记录一次本地提交
　　git push -u origin feature/hello  推送并建立跟踪关系
　　git switch main                   切回主分支
　　git pull                          获取并整合远程更新

　　切分支前先看git status
　　分支名main以仓库实际名称为准


7.分支：

　　main始终稳定
　　feature/hello从main拉出来单独开发
　　改完 --> 提交 --> 发起PR --> 评审 --> 合并回main
　　分支让同一仓库中的开发互不打扰
