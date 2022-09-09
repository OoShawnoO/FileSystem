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
    gcount ++;
    ucount ++;
    error = NO;
}

user_c::~user_c(){

}

string user_c::get_name() const{return name;}
unsigned char user_c::get_uid() const{return uid;}
unsigned char user_c::get_gid() const{return gid;}
ERROR user_c::get_error() const{return error;}

void user_c::set_name(string _name){name = _name;}
bool user_c::set_uid(unsigned char _uid){if(gid == 1){uid = _uid;return true;}  else{error = PERMISSION;return false;}}
bool user_c::set_gid(unsigned char _gid){if(gid == 1){gid = _gid;return true;}  else{error = PERMISSION;return false;}}
bool user_c::set_uid(unsigned char _uid,user_c& user){if(gid == 1){user.uid = _uid;return true;} else{error = PERMISSION;return false;}};
bool user_c::set_gid(unsigned char _gid,user_c& user){if(gid == 1){user.gid = _gid;return true;} else{error = PERMISSION;return false;}};
void user_c::set_error(ERROR _error){error = _error;}

//（1）pwd - 显示当前目录的绝对路径
void user_c::pwd() const{

}
//（2）ls - 显示当前路径下的文件和目录
void user_c::ls(vector<string>&) const{

}
//（3）cd - 切换至指定目录
void user_c::cd(vector<string>&){

}
//（4）mkdir - 创建目录
bool user_c::mkdir(vector<string>&){

}
//（5）rmdir - 删除目录(空目录)
bool user_c::rmdir(vector<string>&){

}
//（6）touch - 创建空文件
bool user_c::touch(vector<string>&){

}
//（7）cp - 拷贝文件或目录到指定文件或目录
bool user_c::cp(vector<string>&){

}
//（8）rm - 删除文件或目录
bool user_c::rm(vector<string>&){

}
//（9）mv - 移动文件与目录或重命名
bool user_c::mv(vector<string>&){

}
//（10）cat - 查看文件内容
bool user_c::cat(vector<string>&){

}
//（11）more - 文本过滤器
void user_c::more(vector<string>&){

}
//（12）less - 分屏查看文件内容
void user_c::less(vector<string>&){

}
//（13）echo - 输出内容到控制台
void user_c::echo(vector<string>&){

}
//（14）head - 显示文件开头部分
bool user_c::head(vector<string>&){

}
//（15）tail - 显示文件尾部的部分
bool user_c::tail(vector<string>&){

}
//（16）> / >> - 输出重定向/追加
bool user_c::dup2(vector<string>&){

}
//（17）ln - 软链接
bool user_c::ln(vector<string>&){

}
//（18）history - 查看执行过的的历史命令
void user_c::history(vector<string>&){
    
}
