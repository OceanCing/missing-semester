date >> "$(dirname "$0")/output.txt" 
#$(dirname "$0")用于保证输入到和脚本同一路径，防止在别的目录输入完整路径执行脚本时生成在输入命令时所在的目录

cat ~/.bash_history | tail -n 10 >> " $(dirname "$0")/output.txt"
#使用history命令在终端可以显示，但在脚本里无输出，因为脚本是使用一个新的子shell运行的，没有记录，要从~/.bash_history 里读取存储的历史记录

cat "$(dirname "$0")/../notes/"*.md | tr -d '[:space:]' | wc -m >> "$(dirname "$0")/output.txt"
#可以使用通配符*来代替名字，这样可以读取所有的.md笔记的内容,新增笔记也不用修改

#如果脚本前加!/bin/bash 运行时不需要bash加脚本名，但是要chmod +x 赋予权限（确保安全），感觉用于作业没必要，不如直接bash
