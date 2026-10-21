
# DX培训第一课

## Linux基础

- 机器人开发中的Linux
  开发电脑-Ubuntu Server-ROS节点/控制程序

- Linux目录
  用户文件       /home/user
  系统配置       /etc
  程序与库       /usr
  日志等可变数据 /var
  设备文件       /dev

## Shell与Vim
- 终端 Shell 命令
  Terminal-shell-程序/系统
- Shell命令的结构
  ls   -la  ~/projects 
  命令 选项 参数

- 当前位置与目录切换
  |命令     |作用                    |
  |:---:    |:---:                   |
  |pwd      |显示当前目录            |
  |ls       |列出目录内容            |
  |cd 目录  |进入指定目录            |
  |cd..     |返回上一级              |
  |cd~      |返回家目录              |

- 文件与目录操作
  |命令     |作用                    |
  |:--:     |:---:                   |
  |mkdir -p |创建目录，允许补齐父目录|
  |touch    |创建空文件/更新时间     |
  |cp       |复制文件                |
  |mv       |移动或重命名            |
  |rm       |删除文件                |
  |cat/less |直接查看/分页查看       |

## Vim

- 模式
  普通模式-按i-插入模式-按Esc-普通模式
- 按键
  h j k l  --左下上右
  dd       --删除当前行
  yy/p     --复制当前行/粘贴
  u        --撤销
  /word    --搜索word

## Git
- Git常用命令
  |命令      |作用                    |
  |:---:     |:---:                   | 
  |git clone<仓库地址>|下载仓库及提交历史|
  |git status         |查看当前分支和文件状态|
  |git diff           |查看尚未暂存的修改|
  |git log --oneline  |用简短格式查看提交历史|
 
