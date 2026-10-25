ls 命令 做什么
-la 选项 怎样执行  -l 详细信息 -a 含隐藏项
~/projects 参数 操作那个对象
mkdir -p  创建目录，允许补齐父目录
touch 创建空文件/更新时间
cp 复制文件
mv 移动或重命名
rm 删除文件 
cat/less 直接查看/分页查看
变量  name="RM" echo "$name"  保存一个值，再通过变量引用它
重定向 echo "hello" > hello.txt  > 覆盖写入；>>追加到文件末尾
管道  ls -la | grep txt  将输出交给下一个程序
条件连接 mkdir -p demo && cd demo  前一条成功后，再执行下一条


vim编辑器
i -编译模式   esc -到正常模式   ：-命令行模式
：q -仅退出   :q! -不保存退出   :wq -保存退出


git常用指令
获取与查看
git clone <仓库地址>  下载仓库及提交历史
git status  查看当前分支和文件状态
git diff  查看尚未暂存的修改
git log --oneline  用简短格式查看提交历史
分支与提交
git switch -c feature/hello  创建并切换到功能分支
git add hello.txt  暂存指定文件
git commit -m "Add greeting"  记录一次本地提交
git push -u origin feature/hello  推送并建立跟踪关系
git switch main  切回主分支
git pull  获取并整合当前分支的远程更新
