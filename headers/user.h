#ifndef USER_H
#define USER_H

#include <string>
#include "types.h"
using namespace std;

class user_c{
private:
    ERROR error;
    unsigned char gid; /* 最多256个组 */
    unsigned char uid; /* 最多256个用户 */
    string name;
    dir_c* current_dir;
public:
    static unsigned char ucount;
    static unsigned char gcount;
    static unsigned char get_ucount();
    static unsigned char get_gcount();
    user_c(string _name);
    ~user_c();

    inline string get_name() const;
    inline unsigned char get_uid() const;
    inline unsigned char get_gid() const;
    inline ERROR get_error() const;

    void set_name(string);
    bool set_uid(unsigned char);
    bool set_gid(unsigned char);
    bool set_uid(unsigned char,user_c& user);
    bool set_gid(unsigned char,user_c& user);
    void set_error(ERROR);

    
    //（1）pwd - 显示当前目录的绝对路径

    //（2）ls - 显示当前路径下的文件和目录

    //（3）cd - 切换至指定目录

    //（4）mkdir - 创建目录

    //（5）rmdir - 删除目录(空目录)

    //（6）touch - 创建空文件

    //（7）cp - 拷贝文件或目录到指定文件或目录

    //（8）rm - 删除文件或目录

    //（9）mv - 移动文件与目录或重命名

    //（10）cat - 查看文件内容

    //（11）more - 文本过滤器

    //（12）less - 分屏查看文件内容

    //（13）echo - 输出内容到控制台

    //（14）head - 显示文件开头部分

    //（15）tail - 显示文件尾部的部分

    //（16）> / >> - 输出重定向/追加

    //（17）ln - 软链接

    //（18）history - 查看执行过的的历史命令
    

};

#endif