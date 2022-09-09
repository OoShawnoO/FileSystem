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
    
    
    user_c* user = new user_c("root");

    users.push_back(*user);

    bool right = true;

    while(1){
        cout << "\e["<< HIGHTLIGHT << ";" << F_CYAN << "m" << user->get_name() << ":" << "\e[0m";
        getline(cin,cmd);
        split(cmd,params);
        cmd.clear();

        string first = params[0];
        params.erase(params.begin());

        if(first == "pwd"){
            user->pwd();
            
        }
        else if(first == "ls"){
            user->ls(params);
        }
        else if(first == "cd"){
            user->cd(params);
        }
        else if(first == "mkdir"){
            right = user->mkdir(params);
        }
        else if(first == "rmdir"){
            right = user->rmdir(params);
        }
        else if(first == "touch"){
            right = user->touch(params);
        }
        else if(first == "cp"){
            user->cp(params);
        }
        else if(first == "rm"){
            right = user->rm(params);
        }
        else if(first == "mv"){
            right = user->mv(params);
        }
        else if(first == "cat"){
            right = user->cat(params);
        }
        else if(first == "more"){
            user->more(params);
        }
        else if(first == "less"){
            user->less(params);
        }
        else if(first == "echo"){
            user->echo(params);
        }
        else if(first == "head"){
            right = user->head(params);
        }
        else if(first == "tail"){
            right = user->tail(params);
        }
        else if(first == "dup2"){
            right = user->dup2(params);
        }else if(first == "ln"){
            right = user->ln(params);
        }
        else if(first == "history"){
            user->history(params);
        }
        else{
            
        }
    }
}