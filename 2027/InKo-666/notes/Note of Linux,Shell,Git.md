# 一. Linux
>- 对于服务器，机器人，嵌入式更爽用的系统

Linux通常指内核，是发行版提供安装和使用的系统
>团队用Ubuntu： Ros兼容性，部署一致性，现有开发生态
***
* #### 开发电脑 == <u>SSH/网络</u> ==> Ubuntu Sever ==> ROS节点/控制程序
***

## 目录
>* 对字母大小写敏感， ~ 是当前用户的家目录， . 是当前目录， .. 是上一级

        /home/user 用户文件         /etc 系统配置
        /usr       程序与库         /var 日志等可变数据
        /dev       设备文件
*  ==绝对路径： 从 / 开始== /home/user/example
*  ==相对路径：从当前位置开始== ./hello.txt
*  ==home目录中的 example== ~/example

***
# 二. 终端，Shell与命令
>  1. 终端：输入输出的界面
>  2. Shell: 解释命令，展开路径，组织执行
>  3. 程序或系统： 执行命令

### 1. Shell命令结构
*命令[选项][参数]

        ls              -la             ~/projects
        命令             选项                参数
       {做什么}         {怎样执行}         {操作哪个对象}
                 -l详细信息,-a含隐藏选项

* #### 位置和目录指令
| 命令 | 作用 |
|:--: | :--: |
|pwd | 显示当前目录|
|ls |列出目录内容|
|cd目录|进入指定目录|
|cd ..|返回上一级|
|cd ~|返回家目录|

* #### 文件和目录操作
|命令|作用|实例|
|:--:|:--:|:--|
|mkdir -p|创建目录|mkdir -p ~/linux_demo|
|touch|创建空文件/更新时间|touch hello.txt|
|cp|复制文件|cp hello.txt hello-copy.txt|
|mv|移动/重命名|mv hello-copy.txt backup.txt|
|rm|删除文件|rm backup.txt|
|cat/less|直接查看/分页查看|cat hello.txt|

### 2. Shell的四种常用组合

        变量           name="RM"                  以变量形式保存（关闭终端会消失）
                      echo "$name"
>                       
        重定向         echo "hello" > hello.txt     > 覆盖写入； >> 在末尾加
>
        管道           ls -la | grep txt            将输出交给下一个程序
>
        条件连接        mkdir -p demo && cd demo     前一条成功就下一条       

>* 不懂就 ls --help，或者man ls（按q退出）
>* Tab 快速补充指令，键盘上下箭头调出历史命令

### 3. Vim
|普通模式按键|操作|
|:--:|:--:|
|h / i / j / k| 左 / 下 / 上 / 右 |
|dd|删除当前行|
|yy / p|复制当前行 / 粘贴|
|u|撤销|
|/|搜索|
      :w 保存         :q 退出        :wq 保存退出         :q! 不保存退出
***

# 三. Git与GitHub
 Git：记录每次提交，比较修改和查看历史，为新功能创建分支
 GitHub：托管Git仓库，通过PR讨论和评审，合并修改

           git add            git commit               git push
    工作区 ----------> 暂存区 ---------------> 本地仓库 ------------> 远程仓库

### 1. 获取与查看
|命令|作用|
|:--:|:--:|
|git clone <仓库地址> | 下载仓库和提交历史|
|git status|查看当前分支和文件状态|
|git diff|查看尚未暂存的修改|
|git log --oneline| 查看提交历史|

### 2. 分支与提交
|命令|作用|
|:--:|:--:|
|git switch -c feature/hello|创建并切换到分支|
|git add hello.txt|暂存指定文件|
|git commit -m "Title"|记录本地提交|
|git push -u origin feature/hello|推送并建立跟踪关系|
|git switch main|切回主分支|
|git pull|获取并整合当前分支的远程更新|
***