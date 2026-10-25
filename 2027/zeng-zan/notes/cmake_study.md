# 1.对于大模型使用
由于我在windows一直用的是workbuddy，学习c++，安装ubantu都是。但linux上workbuddy无法操作本地文件，所以我就让workbuddy指导我如何使用deepseek harness。ds harness通过操作本地文件成功帮我尝试，并解决了clash加速github异常的问题。

# 2.代码的执行阶段

cat > hello.c <<'EOF'
#include <stdio.h>

int main(void) {
    printf("hello, world\n");
    return 0;
}
EOF
我先是输入了这串代码作为开始，然后下一步
gcc -E hello.c -o hello.i     # 预处理
gcc -S hello.i -o hello.s     # 编译
gcc -c hello.s -o hello.o     # 汇编
gcc hello.o -o hello          # 链接

./hello      验证结果，这是手动将代码转化为可执行程序 

# 3.makefile
我先是输入这段printf 'hello: hello.o\n\tgcc hello.o -o hello\n\nhello.o: hello.c\n\tgcc -c hello.c -o hello.o\n\nclean:\n\trm -f hello hello.o\n' > Makefile
作为makefile，然后直接输入make，再运行./hello。就成功执行了程序。vim也可以直接手写程序。

# 4.cmake
cmake会生成一份make，读取CMakeLists.txt,然后据此产生对应的Makefile。
输入了这一串
mkdir -p bulid
cd build    
cmake .. 
输出
-- Configuring done
-- Generating done
-- Build files have been written to: .../compile/build
然后产生了一份makefile
最后
cmake --build .         # 编译
./hello                 # 输出 hello, world

# 5.catkin
由于我还没搞定ros，catkin仅限理论，还没实践。
我只知道catkin相当于进阶版的cmake，可以安装多种功能包以此适应不同场景需求

# 6.大模型和agent
模型相当于脑子，平时网页版聊天只提供知识。agent相当于躯干，可以用工具操作电脑，接触我的文件。

