#ifndef FILE_H
#define FILE_H
#include <string>
#include <time.h>
#include <vector>
#include <list>
#include "user.h"
#include "types.h"

using namespace std;

class filesystem_c{
protected:
    FILETYPE type;              // 属性
    attribute_u owner_attr_u;   // 所有者的权限
    attribute_u group_attr_u;   // 所有者组的权限
    attribute_u other_attr_u;   // 其他用户的权限
    string name;                // 名称
    unsigned char createrid;    // 创建者id
    unsigned char ownerid;      //所有者id
};

class file_c:filesystem_c{
private:
    friend class user_c;
    long size;                  // 文件大小
    tm* create_time;            // 创建时间
    tm* update_time;            // 修改时间
    tm* visit_time;             // 访问时间
    vector<list<char>> memlist; //存储

    bool permission_read(const user_c user) const;
    bool permission_write(const user_c user) const;
    bool permission_exec(const user_c user) const;
    bool permission_chread(const user_c user) const;
    bool permission_chwrite(const user_c user) const;
    bool permission_chexec(const user_c user) const;
    
public:
    file_c(user_c user,string name,unsigned char permission);
    ~file_c();

    bool vi();
    bool cat() const;
    bool stat() const;
    bool write();
    bool read();
};

class dir_c:filesystem_c{
private:

public:

};


#endif