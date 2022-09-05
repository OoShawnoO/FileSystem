#include "main.h"

//设计项目三： （内存）文件系统设计 
//内容：
//以内存作为存储区，模拟 UNIX 的文件系统结构，设计一个简单的文件系统，
//并提供以下 的文件和目录操作接口：创建文件、删除文件、打开文件、关闭文件、读文件、写文件。 
//要求：
//设计文件系统的数据结构和数据结构间的关系，设计文件的基本操作算法，
//完成系统的 总控模块编程和各个操作模块编程。上机测试各个模块，没执行一个操作，打印有关数据结构的内容
//判断操作的正确性。 

// 清除屏幕
#define CLEAR() printf("\033[2J")

// 上移光标
#define MOVEUP(x) printf("\033[%dA", (x))

// 下移光标
#define MOVEDOWN(x) printf("\033[%dB", (x))

// 左移光标
#define MOVELEFT(y) printf("\033[%dD", (y))

// 右移光标
#define MOVERIGHT(y) printf("\033[%dC",(y))

// 定位光标
#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y))

// 光标复位
#define RESET_CURSOR() printf("\033[H")

// 隐藏光标
#define HIDE_CURSOR() printf("\033[?25l")

// 显示光标
#define SHOW_CURSOR() printf("\033[?25h")

//反显
#define HIGHT_LIGHT() printf("\033[7m")
#define UN_HIGHT_LIGHT() printf("\033[27m")


int main(){
    // printf("\e[32m%s\e[0m\n", "hello world");
    // cout << "\e["<< HIGHTLIGHT << ";" << F_CYAN << "m" << "hello world" << "\e[0m" << "123" <<endl;

    attribute_u at;
    at.attr_s.read = 1;
    at.attr_s.write = 1;
    at.attr_s.chdel = 1;
    printf("123 %u\n",at.attr);

    SHOW_CURSOR();
    MOVETO(0,2);
    while(1){
        MOVERIGHT(0);
    }
}