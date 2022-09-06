#include "user.h"

unsigned char user_c::ucount = 0;
unsigned char user_c::gcount = 0;

unsigned char user_c::get_ucount(){
    return ucount;
}
unsigned char user_c::get_gcount(){
    return gcount;
}

user_c::user_c(string _name)
{
    name = _name;
    gid = get_gcount();
    uid = get_ucount();
    error = NO;
}

inline string user_c::get_name() const{return name;}
inline unsigned char user_c::get_uid() const{return uid;}
inline unsigned char user_c::get_gid() const{return gid;}
inline ERROR user_c::get_error() const{return error;}

void user_c::set_name(string _name){name = _name;}
bool user_c::set_uid(unsigned char _uid){if(gid == 1){uid = _uid;return true;}  else{error = PERMISSION;return false;}}
bool user_c::set_gid(unsigned char _gid){if(gid == 1){gid = _gid;return true;}  else{error = PERMISSION;return false;}}
bool user_c::set_uid(unsigned char _uid,user_c& user){if(gid == 1){user.uid = _uid;return true;} else{error = PERMISSION;return false;}};
bool user_c::set_gid(unsigned char _gid,user_c& user){if(gid == 1){user.gid = _gid;return true;} else{error = PERMISSION;return false;}};
void user_c::set_error(ERROR _error){error = _error;}


