# 27赛季控制视觉1 笔记

## 1.Linux基础
- Linux(Ubuntu) & Windows11 & macOS 之间的对比
  -Linux开发环境友好
- Linux下机器人开发的流程
  - 开发电脑->（通过SSH/网络）机器人的Ubuntu Server -> Ros节点/控制程序
- Linux的目录结构
  - /home  /~
  - /etc 系统配置
  - /user 程序与库
  - /var 日志等可变信息
  - /dev 设备文件
## 2.Vim&&Shell
- **Shell命令**
  - 文件与目录操作
    - pwd 显示当前目录
    - ls 列出目录内容
    - cd name 进入目录
    - cd.. 返回上一级
    - cd ~ 返回home
    - mkdir -p 创建目录，允许补齐父目录
    - touch 创建空文件/更新时间
    - cp 复制
    - mv 移动/重命名
    - rm 删除文件（不进回收站）
    - cat/less 直接查看/分页查看
  - Shell的四种常见组合
    - 变量name="RM" \n echo"$name"
    - 重定向
      - echo "hello" > hello.tet 覆盖写入
      - echo "hello" >> hello.txt 追加到末尾
    - 管道 ls -a | grep txt将输出移交给下一个程序
    - 条件连接 mkdir -p demo && cd demo
  - 陌生命令的帮助入口
    - ls --help快速查看
    - man ls 打开手册
    - Tab 补全命令/路径
    - ↑ /↓ 调出历史命令
- **Vim**
  - 模式
    - 普通模式 Esc
    - 插入模式 i
    - 命令模式 :w :q :wq :q!
  - 普通模式按键
    - hjkl 左上下右
    - dd 删除整行
    - yy/p 复制当行/粘贴
    - u 撤销
    - /word 搜索word
  - vim基本用法
    1.  vim hello.txt 打开练习文件
    2. i 插入模式
    3. Esc 返回普通模式
    4. :wq 保存并退出
    5. vim hello.txt
    6. dd u /word 删除一行，撤销，搜索 
## 3.Git&&GitHub
- 简介
  - Git 本地版本控制
  - GitHub 远程托管与协作
- Git 的四个位置
  - 工作区 -> git add
  - 暂存区 -> git commit
  - 本地仓库 -> git push
  - GitHub远程仓库
- Git常用命令：获取与查看
  - git clone <仓库地址> 下载仓库及提交历史
  - git status 查看分支和文件状态
  - git diff 查看尚未保存的修改
  - git log --oneline 简短形式查看提交历史
- Git常用命令：分支与提交
  - git switch -c feature/hello 创建并切换到功能分支
  - git add hello.txt 暂存指定文件
  - git commit -m "Add greeting" 记录一次本地提交
  - git push -u origin feature/hello 推送并创建跟踪关系
  - git switch main 切回主分支
  - git pull 获取并整合当前分支的远程更新
