#ifndef USER_H
#define USER_H

#include "types.h"
#include <functional>
using namespace std;

class user_c
{
private:
    ERROR error;
    unsigned char gid; /* 最多256个组 */
    unsigned char uid; /* 最多256个用户 */
    string name;
    dir_c *current_dir;
    int read_cursor;
    vector<string> his;

public:
    map<string, bool (user_c::*)(vector<string> &)> functions;
    static unsigned char ucount;
    static unsigned char gcount;
    static unsigned char get_ucount();
    static unsigned char get_gcount();
    user_c(string _name);
    ~user_c();

    string get_name() const;
    unsigned char get_uid() const;
    unsigned char get_gid() const;
    ERROR get_error() const;
    dir_c *get_current_dir();
    int get_read_cursor() const;
    vector<string>& get_history();

    void set_name(string);
    bool set_uid(unsigned char);
    bool set_gid(unsigned char);
    bool set_uid(unsigned char, user_c &user);
    bool set_gid(unsigned char, user_c &user);
    void set_error(ERROR);
    void set_current_dir(dir_c *);
    void set_read_cursor(int);

    //（1）pwd - 显示当前目录的绝对路径
    bool pwd(vector<string> &);
    //（2）ls - 显示当前路径下的文件和目录
    bool ls(vector<string> &);
    //（3）cd - 切换至指定目录
    bool cd(vector<string> &);
    // 以下函数重复代码过多 进行进一步封装
    bool TEMP(vector<string>&,function<void(user_c*,string)>,ATTRIBUTE attr = READ);
    
    bool TEMP(vector<string>&,function<filesystem_c*(user_c*,string,filesystem_c*)>,function<void(user_c*,string,string,filesystem_c*)>,ATTRIBUTE attr = READ);
    //（4）mkdir - 创建目录
    bool mkdir(vector<string> &);
    //（5）rmdir - 删除目录(空目录)
    bool rmdir(vector<string> &);
    //（6）touch - 创建空文件
    bool touch(vector<string> &);
    //（7）cp - 拷贝文件或目录到指定文件或目录
    bool cp(vector<string> &);
    //（8）rm - 删除文件或目录
    bool rm(vector<string> &);
    //（9）mv - 移动文件与目录或重命名
    bool mv(vector<string> &);
    //（10）cat - 查看文件内容
    bool cat(vector<string> &);
    //（11）more - 文本过滤器
    bool more(vector<string> &);
    //（12）echo - 输出内容到控制台
    bool echo(vector<string> &);
    //（13）head - 显示文件开头部分
    bool head(vector<string> &);
    //（14）tail - 显示文件尾部的部分
    bool tail(vector<string> &);
    //（15）> / >> - 输出重定向/追加
    bool dup2(vector<string> &);
    //（16）ln - 软链接
    bool ln(vector<string> &);
    //（17）history - 查看执行过的的历史命令
    bool history(vector<string> &);
    // (18) vim - 写文件
    bool vim(vector<string>&);
    // (19) stat - 文件属性
    bool stat(vector<string>&);
};

#endif