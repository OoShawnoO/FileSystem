#include "main.h"

//设计项目三： （内存）文件系统设计
//内容：
//以内存作为存储区，模拟 UNIX 的文件系统结构，设计一个简单的文件系统，
//并提供以下 的文件和目录操作接口：创建文件、删除文件、打开文件、关闭文件、读文件、写文件。
//要求：
//设计文件系统的数据结构和数据结构间的关系，设计文件的基本操作算法，
//完成系统的 总控模块编程和各个操作模块编程。上机测试各个模块，没执行一个操作，打印有关数据结构的内容
//判断操作的正确性。

void split(const string& str,char splitchar,vector<string>& res){
    string strs = str + splitchar;
    int pos = strs.find(splitchar);
    while(pos != string::npos){
        string temp = strs.substr(0,pos);
        res.push_back(temp);
        strs = strs.substr(pos+1);
        pos = strs.find(splitchar);
    }
}

void color_cout(int layout,int color,string s){
    cout << "\e["<< layout << ";" << color << "m" << s << "\e[0m";
}

void uerror(ERROR e){
    switch (e)
    {
        case PERMISSION : {
            color_cout(HIGHTLIGHT,F_RED,"ERROR : Permission denied .");
            cout << endl;
            break;
        }

        case NOTDIR : {
            color_cout(HIGHTLIGHT,F_RED,"ERROR : Its not a dir .");
            cout << endl;
            break;
        }
        
        case NOTFOUND : {
            color_cout(HIGHTLIGHT,F_RED,"ERROR : File not found .");
            cout << endl;
            break;
        }
        
        case CMD : {
            color_cout(HIGHTLIGHT,F_RED,"ERROR : Unknown command .");
            cout << endl;
            break;
        }

        case NOUSER : {
            color_cout(HIGHTLIGHT,F_RED,"ERROR : No Such User .");
            cout << endl;
            break;
        }

        default : {
            break;
        }

    }
    
}
map<string, bool (user_c::*)(std::vector<std::string> &)> user_c::functions;
void InitFunctions()
{
    user_c::functions["ls"] = &user_c::ls;
    user_c::functions["pwd"] = &user_c::pwd;
    user_c::functions["cd"] = &user_c::cd;
    user_c::functions["mkdir"] = &user_c::mkdir;
    user_c::functions["rm"] = &user_c::rm;
    user_c::functions["cp"] = &user_c::cp;
    user_c::functions["touch"] = &user_c::touch;
    user_c::functions["rmdir"] = &user_c::rmdir;
    user_c::functions["mv"] = &user_c::mv;
    user_c::functions["cat"] = &user_c::cat;
    user_c::functions["more"] = &user_c::more;
    user_c::functions["echo"] = &user_c::echo;
    user_c::functions["head"] = &user_c::head;
    user_c::functions["tail"] = &user_c::tail;
    user_c::functions["dup2"] = &user_c::dup2;
    user_c::functions["ln"] = &user_c::ln;
    user_c::functions["history"] = &user_c::history;
    user_c::functions["vim"] = &user_c::vim;
    user_c::functions["stat"] = &user_c::stat;
    user_c::functions["myid"] = &user_c::myid;
    user_c::functions["mygid"] = &user_c::mygid;
    user_c::functions["login"] = &user_c::login;
    user_c::functions["quit"] = &user_c::quit;
    #ifdef __Qt__
    user_c::functions["paste"] = &user_c::paste;
    #endif
}

user_c* ROOT = new user_c("root");

user_c* user = ROOT;

dir_c* root = new dir_c(user,"/",NULL);



int main()
{
    
    InitFunctions();

    user->set_current_dir(root);

    root->get_contents()["file1"] = dynamic_cast<filesystem_c*>(new file_c(user, "file1", dynamic_cast<filesystem_c*>(root)));

    root->get_contents()["dir1"] = dynamic_cast<filesystem_c*>(new dir_c(user, "dir1", dynamic_cast<filesystem_c*>(root)));

    users.push_back(user);

    user_c* customer = new user_c("customer");
    
    users.push_back(customer);

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
        if (user_c::functions.count(first))
        {
            auto func = user_c::functions[first];
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