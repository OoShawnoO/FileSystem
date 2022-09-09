#ifndef FILE_H
#define FILE_H

#include <time.h>
#include <list>
#include <map>
#include "types.h"
#include "user.h"

using namespace std;


class filesystem_c{
public:
    friend class user_c;
    filesystem_c(
        string _name,
        unsigned char owenr,
        unsigned char group,
        unsigned char other,
        const user_c user,
        FILETYPE type
        );
    ~filesystem_c();

    FILETYPE type;              // 属性
    attribute_u owner_attr_u;   // 所有者的权限
    attribute_u group_attr_u;   // 所有者组的权限
    attribute_u other_attr_u;   // 其他用户的权限
    string name;                // 名称
    unsigned char createrid;    // 创建者id
    unsigned char ownerid;      //所有者id
    unsigned char ownergid;     //所有者组id
    tm* create_time;            // 创建时间
    tm* update_time;            // 修改时间
    tm* visit_time;             // 访问时间
    filesystem_c* parent;       // 上级目录

    virtual inline FILETYPE get_filetype() const;
    virtual inline attribute_u get_owner_permission() const;
    virtual inline attribute_u get_group_permission() const;
    virtual inline attribute_u get_other_permission() const;
    virtual inline string get_name() const;
    virtual inline unsigned char get_createid() const;
    virtual inline unsigned char get_ownerid() const;
    virtual inline unsigned char get_ownergid() const;
    virtual inline tm get_create_time() const;
    virtual inline tm get_update_time() const;
    virtual inline tm get_visit_time() const;
    virtual inline filesystem_c* get_parent();

    virtual void set_filetype(FILETYPE);
    virtual void set_filetype();
    virtual void set_owner_permission(attribute_u);
    virtual void set_owner_permission(unsigned char);
    virtual void set_group_permission(attribute_u);
    virtual void set_group_permission(unsigned char);
    virtual void set_other_permission(attribute_u);
    virtual void set_other_permission(unsigned char);
    virtual void set_name(string);
    virtual void set_createid(unsigned char);
    virtual void set_ownerid(unsigned char);
    virtual void set_ownergid(unsigned char);
    virtual void set_create_time(tm*);
    virtual void set_update_time(tm*);
    virtual void set_visit_time(tm*);
    virtual void set_parent(filesystem_c*);

    virtual bool permission(user_c&,ATTRIBUTE) const;

    virtual long get_size() const;
    virtual vector<string>& get_mem();
    virtual map<string,filesystem_c*>& get_contents();
};

class file_c:filesystem_c{
private:
    long size;                  // 文件大小
    vector<string> mem;         //存储 二进制base64_encode(data,sizeof(data)) 解码base64_encode(data,sizeof(data))
    
public:
    file_c(
        const user_c& user,string name,
        unsigned char owner_permission = 63,
        unsigned char group_permission = 63,
        unsigned char other_permission = 5);
    ~file_c();
    long get_size() const;
    vector<string>& get_mem();
};

class dir_c:filesystem_c{
private:
    map<string,filesystem_c*> contents;
public:
    dir_c(const user_c& user,string name,
        unsigned char owner_permission = 63,
        unsigned char group_permission = 63,
        unsigned char other_permission = 5);
    ~dir_c();
    map<string,filesystem_c*>& get_contents();
};


#endif