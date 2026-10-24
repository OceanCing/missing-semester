#lecture 1  Linux  Vim&&Shell Git&&Github
**一.linux**
1.目录结构
~当前用户的家目录
.当前目录
..上一级
**二.Shell**
1.结构 
ls 命令   -选项（-la显示详细信息隐藏文件）  参数
例ls -la ~/project查看project目录的详细信息
2.命令
pwd 显示当前目录
ls  列出目录内容
cd  进入指定目录
cd..返回上一级
cd~ 返回家目录
3.文件和目录操作
makdir -p 创建目录    makdir -p ~/linux_demo
touch     创建空文件
cp        复制文件  cp hello.txt hello-copy.txt
mv        移动或重命名    mv hello-copy.txt backup.txt
rm        删除文件     rm backup.txt
cat       查看
4.Shell常用组合
变量 name="Rm"  echo "$name"
重定向 echo "hello">hello.txt
追加 >>
管道  ls -la | grep txt
条件连接  mkdir -p demo && cd demo(前一条成才执行下一条）
5.帮助命令
ls --help  快速查看选项
man ls     打开手册（按q退出）
**三.Vim**
h/j/k/l  左/下/上/右
dd       删除当前行
yy / p   复制当前行
u        撤销
/word    搜索word
:wq      保存并退出
：q!     强制退出
**四.Git GitHub**mak
1.Git = 本地版本控制（记录每次提交、比较修改、查看历史、创建分支）
GitHub = 远程托管与协作（通过 PR 讨论和评审、合并团队认可的修改）
2.首次使用git
git config --global user.name "name
"
git config --global user.email "e@"
3.SSH密钥认证
ssh-keygen -t ed25519 -C "@.com"
cat ~/.ssh/id_ed25519.pub
4.Git常用命令
git clone<地址>
git status  查看当前分支和文件状态
git diff    查看尚未保存的修改
git log --oneline  简短格式查看提交历史
git switch -c feature/hello创建并切换到功能分支
git add hello.txt  暂存指定文件
git commit -m"Add greeting"记录一次本地提交
git push -u origin feature/hello 推送并建立跟踪关系
git switch main 切换主分支
git pull   获取并整合远程更新
Fork 团队仓库 → Clone 到本地 → 创建 Feature 分支 → 修改/暂存/提交 → Push → 
GitHub 发起 PR → Review 评审 → Merge 到团队仓库




