#include "user.h"
#include "filesystem.h"

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

    functions = *new map<string,bool (user_c::*)(std::vector<std::string>&)>;
    functions["ls"] = &user_c::ls;functions["pwd"] = &user_c::pwd;functions["cd"] = &user_c::cd;functions["mkdir"] = &user_c::mkdir;
    functions["rm"] = &user_c::rm;functions["cp"] = &user_c::cp;functions["touch"] = &user_c::touch;functions["rmdir"] = &user_c::rmdir;
    functions["mv"] = &user_c::mv;functions["cat"] = &user_c::cat;functions["more"] = &user_c::more;functions["less"] = &user_c::less;
    functions["echo"] = &user_c::echo;functions["head"] = &user_c::head;functions["tail"] = &user_c::tail;functions["dup2"]=&user_c::dup2;
    functions["ln"] = &user_c::ln;functions["history"] = &user_c::history;

}

user_c::~user_c(){

}

string user_c::get_name() const{return name;}
unsigned char user_c::get_uid() const{return uid;}
unsigned char user_c::get_gid() const{return gid;}
ERROR user_c::get_error() const{return error;}
dir_c* user_c::get_current_dir(){return current_dir;}


void user_c::set_name(string _name){name = _name;}
bool user_c::set_uid(unsigned char _uid){if(gid == 1){uid = _uid;return true;}  else{error = PERMISSION;return false;}}
bool user_c::set_gid(unsigned char _gid){if(gid == 1){gid = _gid;return true;}  else{error = PERMISSION;return false;}}
bool user_c::set_uid(unsigned char _uid,user_c& user){if(gid == 1){user.uid = _uid;return true;} else{error = PERMISSION;return false;}};
bool user_c::set_gid(unsigned char _gid,user_c& user){if(gid == 1){user.gid = _gid;return true;} else{error = PERMISSION;return false;}};
void user_c::set_error(ERROR _error){error = _error;}
void user_c::set_current_dir(dir_c* dir){current_dir = dir;}

extern dir_c* root;
extern user_c* user;

//（1）pwd - 显示当前目录的绝对路径
bool user_c::pwd(vector<string>&){
    auto dir = (filesystem_c*)current_dir;
    if((dir_c*)dir == root){cout << "/" << endl; return true;}
    vector<string> v;
    while(dir->get_name() != "/"){
        v.push_back(dir->get_name());
        dir = (filesystem_c*)dir->parent;
        v.push_back("/");
    }
    for(vector<string>::reverse_iterator it = v.rbegin();it!=v.rend();it++){
        cout << *it;
    }
    cout << endl;
}
//（2）ls - 显示当前路径下的文件和目录
bool user_c::ls(vector<string>& args){
    for(auto x : get_current_dir()->get_contents()){
        switch(x.second->get_filetype()){
            case DIR : {
                color_cout(HIGHTLIGHT,F_YELLOW,x.first);
                break;
            }
            case BINARY : {
                color_cout(HIGHTLIGHT,F_CYAN,x.first);
                break;
            }
            case TEXT : {
                color_cout(HIGHTLIGHT,F_GREEN,x.first);
                break;
            }
            default : {
                color_cout(HIGHTLIGHT,F_MAGENTA,x.first);
                break;
            }
        }
        cout << " ";
    }
    cout << endl;
}
//（3）cd - 切换至指定目录
bool user_c::cd(vector<string>& args){
    auto pos = get_current_dir();
    vector<string> vc;
    switch(args[0][0]){
        case '/' : {
            pos = root;
            args[0].erase(0,1);
            split(args[0],'/',vc);
            break;
        }
        case '.' : {
            if(args[0][1] == '.' && args[0].size() > 2){
                pos = (dir_c*)(((filesystem_c*)pos)->get_parent());
                args[0].erase(0,3);
                split(args[0],'/',vc);
            }else if(args[0][1] == '.'){
                pos = (dir_c*)(((filesystem_c*)pos)->get_parent());
            }else{
                args[0].erase(0,2);
                split(args[0],'/',vc);
            }
            break;
        }
        default : {
            split(args[0],'/',vc);
            break;
        }
    }
    for(auto x : vc){
        char a = x.c_str()[0];
        if(a == NULL){
            break;
        }

        if(pos->get_contents().find(x)!=pos->get_contents().end()){
            if(pos->get_contents()[x]->get_filetype() == DIR){
                pos = (dir_c*)pos->get_contents()[x];
            }else{
                cout << "NOT DIR" <<endl;
                set_error(NOTDIR);
            } 
        }else{
            cout << "NOT FOUND" <<endl;
            set_error(NOTFOUND);
        }
    }
    if(get_error() == NO){
        set_current_dir(pos);
        return true;
    }else{
        return false;
    }
    
}
//（4）mkdir - 创建目录
bool user_c::mkdir(vector<string>& args){
    auto dir = get_current_dir();
    string name = args[0];
    if(args[0].find("/")!=string::npos){
        name = args[0].substr(args[0].rfind("/")+1);
        args[0] = args[0].substr(0,args[0].rfind("/"));

        if(cd(args)){
            if(((filesystem_c*)get_current_dir())->permission(this,WRITE)){
                dir_c* newdir = new dir_c(this,name,(filesystem_c*)this->get_current_dir());
                set_current_dir(dir);
                return true;
            }else{
                set_error(PERMISSION);
                return false;
            }
        }else{
            return false;
        }
    }else{
        if(((filesystem_c*)dir)->permission(this,WRITE)){
            dir_c* newdir = new dir_c(this,name,(filesystem_c*)this->get_current_dir());
            return true;
        }else{
                set_error(PERMISSION);
                return false;
        }
    }
}
//（5）rmdir - 删除目录(空目录)
bool user_c::rmdir(vector<string>& args){
    auto dir = get_current_dir();
    string name = args[0];
    if(args[0].find("/")!=string::npos){
        name = args[0].substr(args[0].rfind("/")+1);
        args[0] = args[0].substr(0,args[0].rfind("/"));
        if(cd(args)){
            if(((filesystem_c*)get_current_dir())->permission(this,WRITE)){
                delete dynamic_cast<dir_c*>(get_current_dir()->get_contents()[name]);
                set_current_dir(dir);
                return true;
            }else{
                set_error(PERMISSION);
                return false;
            }
        }else{
            return false;
        }
    }else{
        if(((filesystem_c*)dir)->permission(this,WRITE)){
            delete dynamic_cast<dir_c*>(get_current_dir()->get_contents()[name]);
            return true;
        }else{
                set_error(PERMISSION);
                return false;
        }
    }
}
//（6）touch - 创建空文件
bool user_c::touch(vector<string>& args){
    auto dir = get_current_dir();
    string name = args[0];
    if(args[0].find("/")!=string::npos){
        name = args[0].substr(args[0].rfind("/")+1);
        args[0] = args[0].substr(0,args[0].rfind("/"));

        if(cd(args)){
            if(((filesystem_c*)get_current_dir())->permission(this,WRITE)){
                file_c* newfile = new file_c(this,name,(filesystem_c*)this->get_current_dir());
                set_current_dir(dir);
                return true;
            }else{
                set_error(PERMISSION);
                return false;
            }
        }else{
            return false;
        }
    }else{
        if(((filesystem_c*)dir)->permission(this,WRITE)){
            file_c* newfile = new file_c(this,name,(filesystem_c*)this->get_current_dir());
            return true;
        }else{
                set_error(PERMISSION);
                return false;
        }
    }
}
//（7）cp - 拷贝文件或目录到指定文件或目录
bool user_c::cp(vector<string>& args){

}
//（8）rm - 删除文件或目录
bool user_c::rm(vector<string>& args){

}
//（9）mv - 移动文件与目录或重命名
bool user_c::mv(vector<string>& args){

}
//（10）cat - 查看文件内容
bool user_c::cat(vector<string>& args){

}
//（11）more - 文本过滤器
bool user_c::more(vector<string>& args){

}
//（12）less - 分屏查看文件内容
bool user_c::less(vector<string>& args){

}
//（13）echo - 输出内容到控制台
bool user_c::echo(vector<string>& args){

}
//（14）head - 显示文件开头部分
bool user_c::head(vector<string>& args){

}
//（15）tail - 显示文件尾部的部分
bool user_c::tail(vector<string>& args){

}
//（16）> / >> - 输出重定向/追加
bool user_c::dup2(vector<string>& args){

}
//（17）ln - 软链接
bool user_c::ln(vector<string>& args){

}
//（18）history - 查看执行过的的历史命令
bool user_c::history(vector<string>& args){
    
}
