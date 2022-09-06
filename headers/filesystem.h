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

    inline FILETYPE get_filetype() const;
    inline attribute_u get_owner_permission() const;
    inline attribute_u get_group_permission() const;
    inline attribute_u get_other_permission() const;
    inline string get_name() const;
    inline unsigned char get_createid() const;
    inline unsigned char get_ownerid() const;
    inline unsigned char get_ownergid() const;
    inline tm get_create_time() const;
    inline tm get_update_time() const;
    inline tm get_visit_time() const;

    void set_filetype(FILETYPE);
    void set_filetype();
    void set_owner_permission(attribute_u);
    void set_owner_permission(unsigned char);
    void set_group_permission(attribute_u);
    void set_group_permission(unsigned char);
    void set_other_permission(attribute_u);
    void set_other_permission(unsigned char);
    void set_name(string);
    void set_createid(unsigned char);
    void set_ownerid(unsigned char);
    void set_ownergid(unsigned char);
    void set_create_time(tm*);
    void set_update_time(tm*);
    void set_visit_time(tm*);

    bool permission(user_c&,ATTRIBUTE) const;
};

class file_c:filesystem_c{
private:
    long size;                  // 文件大小
    vector<list<char>> memlist; //存储
    
public:
    file_c(
        const user_c& user,string name,
        unsigned char owner_permission = 63,
        unsigned char group_permission = 63,
        unsigned char other_permission = 5);
    ~file_c();
};

class dir_c:filesystem_c{
private:

public:
    dir_c(const user_c& user,string name,
        unsigned char owner_permission = 63,
        unsigned char group_permission = 63,
        unsigned char other_permission = 5);
    ~dir_c();

};


#endif