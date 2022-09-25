#include "main.h"

//设计项目三： （内存）文件系统设计
//内容：
//以内存作为存储区，模拟 UNIX 的文件系统结构，设计一个简单的文件系统，
//并提供以下 的文件和目录操作接口：创建文件、删除文件、打开文件、关闭文件、读文件、写文件。
//要求：
//设计文件系统的数据结构和数据结构间的关系，设计文件的基本操作算法，
//完成系统的 总控模块编程和各个操作模块编程。上机测试各个模块，没执行一个操作，打印有关数据结构的内容
//判断操作的正确性。

int main()
{

    user->set_current_dir(root);

    root->get_contents()["file1"] = dynamic_cast<filesystem_c*>(new file_c(user, "file1", dynamic_cast<filesystem_c*>(root)));

    root->get_contents()["dir1"] = dynamic_cast<filesystem_c*>(new dir_c(user, "dir1", dynamic_cast<filesystem_c*>(root)));

    users.push_back(user);

    bool right = true;

    vector<string>& vec = user->get_history();

    while (1)
    {
        
        color_cout(HIGHTLIGHT, F_CYAN, user->get_name() + ":");
        getline(cin, cmd);
        vec.push_back(cmd);
        split(cmd, ' ', params);
        cmd.clear();

        string first = params[0];
        params.erase(params.begin());
        // 这里使用到的是 策略模式 这种设计模式，原因是最开始的思路 if elseif 嵌套太多 可读性差 且 实用性不高。
        // 将函数指针与cmd 进行分组 封装在一个 map 对象中，C++ map底层是红黑树 查询时间复杂度 O(log(n))
        if (user->functions.count(first))
        {
            auto func = user->functions[first];
            (user->*func)(params);
        }
        else
        {
            user->set_error(CMD);
        }
        uerror(user->get_error());
        user->set_error(NO);
        params.clear();
    }
}