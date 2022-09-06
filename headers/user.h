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
};

#endif