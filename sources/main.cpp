#include "main.h"

//设计项目三： （内存）文件系统设计 
//内容：
//以内存作为存储区，模拟 UNIX 的文件系统结构，设计一个简单的文件系统，
//并提供以下 的文件和目录操作接口：创建文件、删除文件、打开文件、关闭文件、读文件、写文件。 
//要求：
//设计文件系统的数据结构和数据结构间的关系，设计文件的基本操作算法，
//完成系统的 总控模块编程和各个操作模块编程。上机测试各个模块，没执行一个操作，打印有关数据结构的内容
//判断操作的正确性。 


int main(){
    // printf("\e[32m%s\e[0m\n", "hello world");
    // cout << "\e["<< HIGHTLIGHT << ";" << F_CYAN << "m" << "hello world" << "\e[0m" << "123" <<endl;

    attribute_u at;
    at.attr_s.read = 1;
    at.attr_s.write = 1;
    printf("123 %u\n",at.attr);
}