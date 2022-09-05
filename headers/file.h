#ifndef FILE_H
#define FILE_H
#include <string>
#include <time.h>
#include <vector>
#include <list>
#include "user.h"
#include "types.h"

using namespace std;

class file_c{
private:
    friend class user_c;

    attribute_u attr_u; // 文件属性联合体
    string name;        // 文件名
    string creater;     // 创建者
    long size;          // 文件大小
    tm* create_time;    // 创建时间
    tm* update_time;    // 修改时间
    tm* visit_time;     // 访问时间
    vector<list<char>> memlist; //存储

    bool permission_read(const user_c user) const;
    bool permission_write(const user_c user) const;
    bool permission_exec(const user_c user) const;
    bool permission_del(const user_c user) const;

    
public:
    file_c(user_c user,string name,unsigned char permission);
    ~file_c();

    bool vi();
    bool cat() const;
    bool stat() const;
    bool write();
    bool read();
    

};

#endif