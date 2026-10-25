## 基础位置指令

pwd 显示所在目录名

ls /ls-la 显示目录下所有内容名 -a显示隐藏文件 -l显示详细信息

cd xx 进入xx目录 如：cd ~(进入根目录，~代表/home)

cd ..返回上一级

## 修改文件和目录指令

mkdir -p ~/aa/xx      建xx目录 注：加-p会自动补齐上级目录，如果不加，路径必须从home开始一路完整

touch xx.格式 创建新文件（从当前位置，也就是用cd 进入的目录）

cat 文件名 在终端显示文件内容

tail -n  <数字><文件名>  查看文件最后几行内容

wc -m <文件名> 统计文件的字符数 -m换成-w则显示单词数

date 显示系统时间

## 语法与符号

echo “文字”  print

\>  覆盖重定向    echo “hello” > a.txt 会清空a原先内容

\>> 追加重定向  不覆盖在原本结尾加上

| 管道 把前一个命令的输出内容作为后一个命令的输出

&& 先后执行

## git和github使用

#### 获取仓库

**1.git clone <SSH地址>**

**2.git status 查看状态，改了哪些文件，把哪些放进了暂存区**

**3.git switch -c <分支名> 创建一个新分支**

保存与提交

4.git add <文件名> 把修改的文件暂存

5.git commit -m "Add homework script"

同步到仓库

6.git push origin <分支名>

7.git pull 把github最新代码拉到本地并合并，防止修改过程其他人更新

