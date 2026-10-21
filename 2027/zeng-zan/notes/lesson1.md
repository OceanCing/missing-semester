# 1.Linux 常用命令
  
pwd 显示路径,直接输入即可
ls 显示文件夹
ls -la 显示所有文件
cd 切换目录  如：cd ~/linux_course/missing-semester/2027 
mkdir 创建文件目录

# 2.vim的操作
i 首行直接点击i即可进入插入模式
esc 回到普通模式，普通模式无法插入文字，只能查阅或者复制，删除
：wq 保存并退出
dd 删除整行
u 撤销

操作实例，先vim 文件，再i插入，：wq是在esc之后才能用

# 3.git and github
git是本地操作离线提交任务，github是在线提交，存git的网站
流程：最最开始要git switch -c 分支名 创建分支， 先git add到暂存区，再git commit -m"adding"提交到本地仓库并且说明，最后git push -u origin 分支名

id_ed25519.pub才是公钥，id_ed25519是私钥
cat ~/.ssh/id_ed25519.pub查看公钥
