#第一堂课笔记
## Linux
### 命令通用格式
command [-options] [parameter]
###文件和目录操作
`mkdir -p` - 创建目录，允许补齐父目录
`touch`  - 创建空文件/更新时间
`cp`- 复制文件
`mv` - 移动或重命名
`cat/less` - 直接查看/分页查看
`rm` - 删除（通常不进回收站）

### Shell的四种常见组合
**变量**    eg:`name=“RM”  echo”$name“`     保存一个值，再通过变量引用它
**重定向**  eg:`echo “hello”> hello.txt`   >覆盖写入；>>追加到文件末尾
**管道**    eg：`ls -la | grap txt `        将输出交给下一个程序
**条件连接**eg:mkdir -`p demo && cd demo`  前一条成功后，再执行下一条

## Vim
### 模式
普通模式：移动，删除，复制
插入模式：输入文本
命令行：保存，退出
### 模式切换
普通模式————（`i`）————>插入模式（esc：返回普通模式）
普通模式————（`:`）————>命令行输入{:`w`(保存) ：`q`（退出）：`wq`（保存退出）：`q!`（放弃修改退出）
### 按键
- `h j k l`: 左/下/右/上
- `i`: 插前
- `a`: 附后
- `dd`: 删除当前行
- `yy `/`p`: 复制当前行/粘贴
- `/word`: 搜索word
- `u`: 撤销
- `gg`: 到第一行
- `G`: 到最后一行
- `.`: 重复前次操作
- `ctrl+r`: 恢复前次操作
- `ctrl+V`: 可视化块
- `shift+V`: 可视化行
- `:%s/旧/新/g`: 全局替换

## Git
本地版本控制
### Git的四个位置
工作区————(git add)————>暂存区————（git commit）————>本地仓库————（git push）————> GitHub远程仓库 ps:commit只记录到本地；push 才把提交发送到远程
### Git常用命令
- `git clone`:<仓库地址> 下载仓库及提交历史
- `git status`: 查看当前分支和文件状态
- `git diff`: 查看尚未暂存的修改
- `git log --oneline`: 用简短格式查看提交历史
- `git switch -c frature/hello`: 创建并切换到功能分支
- `git add hello.txt`: 暂存指定文件
- `git commit -m“Add greeting`: 记录一次本地提交
- `git push -u origin feature/hello`: 推送并建立跟踪关系
- `git switch main`: 切回主分支
- `git pull`: 获取并整合当前分支的远程更新


          
